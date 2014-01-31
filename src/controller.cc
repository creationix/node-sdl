#include "controller.h"
#include "joystick.h"
#include "helpers.h"

using namespace v8;
using namespace node;


static Handle<Object> ControllerButtonBindToObject(SDL_GameControllerButtonBind bind) {
	Handle<Object> ret = Object::New();
	ret->Set(String::NewSymbol("type"), Number::New(bind.bindType));
	switch(bind.bindType) {
	case SDL_CONTROLLER_BINDTYPE_NONE:
		// Nothing to do here. Just wanted to shut the compiler up :)
		break;
	case SDL_CONTROLLER_BINDTYPE_BUTTON:
		ret->Set(String::NewSymbol("button"), Number::New(bind.value.button));
		break;
	case SDL_CONTROLLER_BINDTYPE_AXIS:
		ret->Set(String::NewSymbol("axis"), Number::New(bind.value.axis));
		break;
	case SDL_CONTROLLER_BINDTYPE_HAT:
		Handle<Object> hat = Object::New();
		hat->Set(String::NewSymbol("hat"), Number::New(bind.value.hat.hat));
		hat->Set(String::NewSymbol("hatMask"), Number::New(bind.value.hat.hat_mask));
		ret->Set(String::NewSymbol("hat"), hat);
		break;
	}
	return ret;
}

void sdl::controller::Init(Handle<Object> exports) {
	GameControllerWrapper::Init(exports);

	NODE_SET_METHOD(exports, "addMapping", AddMapping);
	NODE_SET_METHOD(exports, "addMappingsFromFile", AddMappingsFromFile);

	NODE_SET_METHOD(exports, "mappingForGuid", MappingForGUID);
	NODE_SET_METHOD(exports, "nameForIndex", NameForIndex);

	NODE_SET_METHOD(exports, "eventState", EventState);
	NODE_SET_METHOD(exports, "getAxisFromString", GetAxisFromString);
	NODE_SET_METHOD(exports, "getButtonFromString", GetButtonFromString);

	Handle<Object> BINDTYPE = Object::New();
	exports->Set(String::NewSymbol("BINDTYPE"), BINDTYPE);
	BINDTYPE->Set(String::NewSymbol("NONE"), Number::New(SDL_CONTROLLER_BINDTYPE_NONE));
	BINDTYPE->Set(String::NewSymbol("BUTTON"), Number::New(SDL_CONTROLLER_BINDTYPE_BUTTON));
	BINDTYPE->Set(String::NewSymbol("AXIS"), Number::New(SDL_CONTROLLER_BINDTYPE_AXIS));
	BINDTYPE->Set(String::NewSymbol("HAT"), Number::New(SDL_CONTROLLER_BINDTYPE_HAT));

	Handle<Object> AXIS = Object::New();
	exports->Set(String::NewSymbol("AXIS"), AXIS);
	AXIS->Set(String::NewSymbol("INVALID"), Number::New(SDL_CONTROLLER_AXIS_INVALID));
	AXIS->Set(String::NewSymbol("LEFTX"), Number::New(SDL_CONTROLLER_AXIS_LEFTX));
	AXIS->Set(String::NewSymbol("LEFTY"), Number::New(SDL_CONTROLLER_AXIS_LEFTY));
	AXIS->Set(String::NewSymbol("RIGHTX"), Number::New(SDL_CONTROLLER_AXIS_RIGHTX));
	AXIS->Set(String::NewSymbol("RIGHTY"), Number::New(SDL_CONTROLLER_AXIS_RIGHTY));
	AXIS->Set(String::NewSymbol("TRIGGERLEFT"), Number::New(SDL_CONTROLLER_AXIS_TRIGGERLEFT));
	AXIS->Set(String::NewSymbol("TRIGGERRIGHT"), Number::New(SDL_CONTROLLER_AXIS_TRIGGERRIGHT));
	AXIS->Set(String::NewSymbol("MAX"), Number::New(SDL_CONTROLLER_AXIS_MAX));

	Handle<Object> BUTTON = Object::New();
	exports->Set(String::NewSymbol("BUTTON"), BUTTON);
	BUTTON->Set(String::NewSymbol("INVALID"), Number::New(SDL_CONTROLLER_BUTTON_INVALID));
	BUTTON->Set(String::NewSymbol("A"), Number::New(SDL_CONTROLLER_BUTTON_A));
	BUTTON->Set(String::NewSymbol("B"), Number::New(SDL_CONTROLLER_BUTTON_B));
	BUTTON->Set(String::NewSymbol("X"), Number::New(SDL_CONTROLLER_BUTTON_X));
	BUTTON->Set(String::NewSymbol("Y"), Number::New(SDL_CONTROLLER_BUTTON_Y));
	BUTTON->Set(String::NewSymbol("BACK"), Number::New(SDL_CONTROLLER_BUTTON_BACK));
	BUTTON->Set(String::NewSymbol("GUIDE"), Number::New(SDL_CONTROLLER_BUTTON_GUIDE));
	BUTTON->Set(String::NewSymbol("START"), Number::New(SDL_CONTROLLER_BUTTON_START));
	BUTTON->Set(String::NewSymbol("LEFTSTICK"), Number::New(SDL_CONTROLLER_BUTTON_LEFTSTICK));
	BUTTON->Set(String::NewSymbol("RIGHTSTICK"), Number::New(SDL_CONTROLLER_BUTTON_RIGHTSTICK));
	BUTTON->Set(String::NewSymbol("LEFTSHOULDER"), Number::New(SDL_CONTROLLER_BUTTON_LEFTSHOULDER));
	BUTTON->Set(String::NewSymbol("RIGHTSHOULDER"), Number::New(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER));
	BUTTON->Set(String::NewSymbol("DPAD_UP"), Number::New(SDL_CONTROLLER_BUTTON_DPAD_UP));
	BUTTON->Set(String::NewSymbol("DPAD_DOWN"), Number::New(SDL_CONTROLLER_BUTTON_DPAD_DOWN));
	BUTTON->Set(String::NewSymbol("DPAD_LEFT"), Number::New(SDL_CONTROLLER_BUTTON_DPAD_LEFT));
	BUTTON->Set(String::NewSymbol("DPAD_RIGHT"), Number::New(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
	BUTTON->Set(String::NewSymbol("MAX"), Number::New(SDL_CONTROLLER_BUTTON_MAX));
}

Persistent<FunctionTemplate> sdl::controller::GameControllerWrapper::wrap_template_;

sdl::controller::GameControllerWrapper::GameControllerWrapper() {
}
sdl::controller::GameControllerWrapper::~GameControllerWrapper() {
	if(NULL != controller_) {
		SDL_GameControllerClose(controller_);
	}
}

void sdl::controller::GameControllerWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("GameControllerWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getAttached", GetAttached);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getAxis", GetAxis);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getBindForAxis", GetBindForAxis);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getBindForButton", GetBindForButton);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getButton", GetButton);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getJoystick", GetJoystick);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getStringForAxis", GetStringForAxis);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getStringForButton", GetStringForButton);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getMapping", GetMapping);
  	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getName", GetName);

	exports->Set(String::NewSymbol("GameController"), wrap_template_->GetFunction());
}

Handle<Value> sdl::controller::GameControllerWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Must create a GameController with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Exepcted new sdl.GameController(Number)")));
	}

	int index = args[0]->Int32Value();
	SDL_GameController* controller = SDL_GameControllerOpen(index);
	if(NULL == controller) {
		return ThrowSDLException(__func__);
	}

	GameControllerWrapper* wrap = new GameControllerWrapper();
	wrap->controller_ = controller;
	wrap->Wrap(args.This());

	return args.This();
}

Handle<Value> sdl::controller::GameControllerWrapper::GetAttached(const Arguments& args) {
	HandleScope scope;

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	SDL_bool ret = SDL_GameControllerGetAttached(wrap->controller_);

	return scope.Close(Boolean::New(ret == SDL_TRUE ? true : false));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetAxis(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetAxis(Number)")));
	}

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(args[0]->Int32Value());
	int16_t ret = SDL_GameControllerGetAxis(wrap->controller_, axis);

	return scope.Close(Number::New(ret));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetBindForAxis(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetBindForAxis(Number)")));
	}

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(args[0]->Int32Value());
	SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForAxis(wrap->controller_, axis);

	return scope.Close(ControllerButtonBindToObject(bind));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetBindForButton(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetBindForButton(Number)")));
	}

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(args[0]->Int32Value());
	SDL_GameControllerButtonBind bind = SDL_GameControllerGetBindForButton(wrap->controller_, button);

	return scope.Close(ControllerButtonBindToObject(bind));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetButton(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetButton(Number)")));
	}

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(args[0]->Int32Value());
	uint8_t ret = SDL_GameControllerGetButton(wrap->controller_, button);

	return scope.Close(Boolean::New(ret == 1 ? true : false));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetJoystick(const Arguments& args) {
	HandleScope scope;

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	SDL_Joystick* joystick = SDL_GameControllerGetJoystick(wrap->controller_);

	Handle<Value> arg = External::New(joystick);
	Handle<Value> argv[] = {arg, Boolean::New(false)};
	return scope.Close(JoystickWrapper::wrap_template_->GetFunction()->NewInstance(2, argv));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetMapping(const Arguments& args) {
	HandleScope scope;

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	char* mapping = SDL_GameControllerMapping(wrap->controller_);
	if(NULL == mapping) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(mapping));
}
Handle<Value> sdl::controller::GameControllerWrapper::GetName(const Arguments& args) {
	HandleScope scope;

	GameControllerWrapper* wrap = ObjectWrap::Unwrap<GameControllerWrapper>(args.This());
	const char* name = SDL_GameControllerName(wrap->controller_);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(name));
}

Handle<Value> sdl::controller::AddMapping(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected AddMapping(String)")));
	}

	String::Utf8Value mapping(args[0]);
	int err = SDL_GameControllerAddMapping(*mapping);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(err));
}
Handle<Value> sdl::controller::AddMappingsFromFile(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected AddMappingFromFile(String)")));
	}

	String::Utf8Value file(args[0]);
	int err = SDL_GameControllerAddMappingsFromFile(*file);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(err));
}

Handle<Value> sdl::controller::MappingForGUID(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected MappingForGUID(String)")));
	}

	String::Utf8Value jsGuid(args[0]);
	SDL_JoystickGUID guid;
	for(int i = 0; i < 16; i++) {
		guid.data[i] = static_cast<uint8_t>((*jsGuid)[i]);
	}
	char* mapping = SDL_GameControllerMappingForGUID(guid);
	if(NULL == mapping) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(mapping));
}
Handle<Value> sdl::controller::NameForIndex(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected NameForIndex(Number)")));
	}

	int index = args[0]->Int32Value();
	const char* name = SDL_GameControllerNameForIndex(index);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(name));
}

Handle<Value> sdl::controller::EventState(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected EventState(Number)")));
	}

	int state = args[0]->Int32Value();
	int ret = SDL_GameControllerEventState(state);

	return scope.Close(Number::New(ret));
}
Handle<Value> sdl::controller::GetAxisFromString(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetAxisFromString(String)")));
	}

	String::Utf8Value pchString(args[0]);
	SDL_GameControllerAxis axis = SDL_GameControllerGetAxisFromString(*pchString);

	return scope.Close(Number::New(axis));
}
Handle<Value> sdl::controller::GetButtonFromString(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetButtonFromString(String)")));
	}

	String::Utf8Value pchString(args[0]);
	SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString(*pchString);

	return scope.Close(Number::New(button));
}
Handle<Value> sdl::controller::GetStringForAxis(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetStringForAxis(Number)")));
	}

	SDL_GameControllerAxis axis = static_cast<SDL_GameControllerAxis>(args[0]->Int32Value());
	const char* axisString = SDL_GameControllerGetStringForAxis(axis);

	return scope.Close(String::New(axisString));
}
Handle<Value> sdl::controller::GetStringForButton(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetStringForButton(Number)")));
	}

	SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(args[0]->Int32Value());
	const char* buttonString = SDL_GameControllerGetStringForButton(button);

	return scope.Close(String::New(buttonString));
}

Handle<Value> Update(const Arguments& args) {
	HandleScope scope;

	SDL_GameControllerUpdate();

	return Undefined();
}
Handle<Value> IsController(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected IsController(Number)")));
	}

	int index = args[0]->Int32Value();
	SDL_bool ret = SDL_IsGameController(index);

	return scope.Close(Boolean::New(ret == SDL_TRUE ? true : false));
}
