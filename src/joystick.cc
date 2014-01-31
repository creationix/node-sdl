#include "joystick.h"
#include "helpers.h"
#include "struct_wrappers.h"


using namespace v8;
using namespace node;


void sdl::joystick::Init(Handle<Object> exports) {
	JoystickWrapper::Init(exports);

	NODE_SET_METHOD(exports, "numJoysticks", NumJoysticks);

	NODE_SET_METHOD(exports, "joystickNameForIndex", JoystickNameForIndex);
	NODE_SET_METHOD(exports, "joystickGetDeviceGUID", JoystickGetDeviceGUID);
	NODE_SET_METHOD(exports, "joystickGetGUIDFromString", JoystickGetGUIDFromString);
	NODE_SET_METHOD(exports, "joystickGetGUIDString", JoystickGetGUIDString);

	NODE_SET_METHOD(exports, "joystickUpdate", JoystickUpdate);
	NODE_SET_METHOD(exports, "joystickEventState", JoystickEventState);
}

Persistent<FunctionTemplate> sdl::JoystickWrapper::wrap_template_;

sdl::JoystickWrapper::JoystickWrapper() {
}
sdl::JoystickWrapper::JoystickWrapper(bool owned) {
	owned_ = owned;
}
sdl::JoystickWrapper::~JoystickWrapper() {
	if(NULL != joystick_ && !owned_) {
		SDL_JoystickClose(joystick_);
	}
}

void sdl::JoystickWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("JoystickWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getAttached", GetAttached);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getAxis", GetAxis);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getBall", GetBall);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getButton", GetButton);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getGUID", GetGUID);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getHat", GetHat);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getName", GetName);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getNumAxes", GetNumAxes);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getNumButtons", GetNumButtons);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getNumBalls", GetNumBalls);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getNumHats", GetNumHats);

	exports->Set(String::NewSymbol("Joystick"), wrap_template_->GetFunction());
}
Handle<Value> sdl::JoystickWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Must create an sdl.Joystick with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected new sdl.Joystick(Number)")));
	}

	if(args[0]->IsExternal()) {
		JoystickWrapper* wrap;
		if(args.Length() > 1) {
			wrap = new JoystickWrapper(args[1]->BooleanValue());
		}
		else {
			wrap = new JoystickWrapper();
		}
		wrap->joystick_ = static_cast<SDL_Joystick*>(Handle<External>::Cast(args[0])->Value());
		wrap->Wrap(args.This());
	}
	else {
		int index = args[0]->Int32Value();
		SDL_Joystick* joystick = SDL_JoystickOpen(index);
		if(NULL == joystick) {
			return ThrowSDLException(__func__);
		}

		JoystickWrapper* wrap = new JoystickWrapper();
		wrap->joystick_ = joystick;
		wrap->Wrap(args.This());
	}

	return args.This();
}

Handle<Value> sdl::JoystickWrapper::GetAttached(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	SDL_bool attached = SDL_JoystickGetAttached(wrap->joystick_);

	return scope.Close(Boolean::New(attached ? true : false));
}
Handle<Value> sdl::JoystickWrapper::GetAxis(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetAxis(Number)")));
	}

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int axis = args[0]->Int32Value();
	int16_t position = SDL_JoystickGetAxis(wrap->joystick_, axis);
	if(0 == position) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(position));
}
Handle<Value> sdl::JoystickWrapper::GetBall(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetBall(Number)")));
	}

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int ball = args[0]->Int32Value();
	int dx, dy;
	int err = SDL_JoystickGetBall(wrap->joystick_, ball, &dx, &dy);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	ret->Set(String::NewSymbol("dx"), Number::New(dx));
	ret->Set(String::NewSymbol("dy"), Number::New(dy));

	return scope.Close(ret);
}
Handle<Value> sdl::JoystickWrapper::GetButton(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetButton(Number)")));
	}

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int button = args[0]->Int32Value();
	uint8_t ret = SDL_JoystickGetButton(wrap->joystick_, button);

	return scope.Close(Boolean::New(ret == 1 ? true : false));
}
Handle<Value> sdl::JoystickWrapper::GetGUID(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	SDL_JoystickGUID guid = SDL_JoystickGetGUID(wrap->joystick_);

	// SDL_JoystickGUID is defined as a struct holding a single array of 16 elements.
	Handle<Array> ret = Array::New(16);
	for(int i = 0; i < 16; i++) {
		ret->Set(i, Number::New(guid.data[i]));
	}

	return scope.Close(ret);
}
Handle<Value> sdl::JoystickWrapper::GetHat(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetHat(Number)")));
	}

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int hat = args[0]->Int32Value();
	uint8_t ret = SDL_JoystickGetHat(wrap->joystick_, hat);

	return scope.Close(Number::New(ret));
}

Handle<Value> sdl::JoystickWrapper::GetName(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	const char* name = SDL_JoystickName(wrap->joystick_);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(name));
}
Handle<Value> sdl::JoystickWrapper::GetNumAxes(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int axes = SDL_JoystickNumAxes(wrap->joystick_);
	if(axes < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(axes));
}
Handle<Value> sdl::JoystickWrapper::GetNumButtons(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int buttons = SDL_JoystickNumButtons(wrap->joystick_);
	if(buttons < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(buttons));
}
Handle<Value> sdl::JoystickWrapper::GetNumBalls(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int balls = SDL_JoystickNumBalls(wrap->joystick_);
	if(balls < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(balls));
}
Handle<Value> sdl::JoystickWrapper::GetNumHats(const Arguments& args) {
	HandleScope scope;

	JoystickWrapper* wrap = ObjectWrap::Unwrap<JoystickWrapper>(Handle<Object>::Cast(args.This()));
	int hats = SDL_JoystickNumHats(wrap->joystick_);
	if(hats < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(hats));
}

Handle<Value> sdl::NumJoysticks(const Arguments& args) {
	HandleScope scope;

	return scope.Close(Number::New(SDL_NumJoysticks()));
}

Handle<Value> sdl::JoystickNameForIndex(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected JoystickNameForIndex(Number)")));
	}

	int index = args[0]->Int32Value();
	const char* name = SDL_JoystickNameForIndex(index);
	if(NULL == name) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(name));
}

Handle<Value> sdl::JoystickGetDeviceGUID(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected JoystickGetDeviceGUID(Number)")));
	}

	int index = args[0]->Int32Value();
	SDL_JoystickGUID guid = SDL_JoystickGetDeviceGUID(index);

	// SDL_JoystickGUID is defined as a struct holding a single array of 16 elements.
	Handle<Array> ret = Array::New(16);
	for(int i = 0; i < 16; i++) {
		ret->Set(i, Number::New(guid.data[i]));
	}

	return scope.Close(ret);
}
Handle<Value> sdl::JoystickGetGUIDFromString(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected JoystickGetGUIDFromString(String)")));
	}

	String::Utf8Value pchGuid(args[0]);
	SDL_JoystickGUID guid = SDL_JoystickGetGUIDFromString(*pchGuid);

	// SDL_JoystickGUID is defined as a struct holding a single array of 16 uint8_t elements.
	Handle<Array> ret = Array::New(16);
	for(int i = 0; i < 16; i++) {
		ret->Set(i, Number::New(guid.data[i]));
	}

	return scope.Close(ret);
}
Handle<Value> sdl::JoystickGetGUIDString(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected JoystickGetGUIDString(String)")));
	}

	Handle<Array> guidArr = Handle<Array>::Cast(args[0]);
	SDL_JoystickGUID guid;
	for(int i = 0; i < 16; i++) {
		guid.data[i] = static_cast<uint8_t>(guidArr->Get(i)->Uint32Value());
	}
	// Hopefully 200 characters is enough to hold the string. Should this be a higher number?
	char pszGuid[200];
	SDL_JoystickGetGUIDString(guid, pszGuid, 200);

	return scope.Close(String::New(pszGuid));
}

Handle<Value> sdl::JoystickUpdate(const Arguments& args) {
	HandleScope scope;

	SDL_JoystickUpdate();

	return Undefined();
}

Handle<Value> sdl::JoystickEventState(const Arguments& args) {
	HandleScope scope;

	int state;
	if (args.Length() == 0) {
		state = SDL_QUERY;
	} else {
		if (!(args.Length() == 1 && args[0]->IsBoolean())) {
			return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickEventState([Boolean])")));
		}
		state = args[0]->BooleanValue() ? SDL_ENABLE : SDL_IGNORE;
	}
	return Boolean::New(SDL_JoystickEventState(state));
}
