#include "mouse.h"
#include "surface.h"
#include "helpers.h"
#include "window.h"

using namespace v8;
using namespace node;


void sdl::mouse::Init(Handle<Object> exports) {
	CursorWrapper::Init(exports);

	NODE_SET_METHOD(exports, "showCursor", ShowCursor);
	NODE_SET_METHOD(exports, "getCursor", GetCursor);
	NODE_SET_METHOD(exports, "getDefaultCursor", GetDefaultCursor);
	NODE_SET_METHOD(exports, "getMouseFocus", GetMouseFocus);
	NODE_SET_METHOD(exports, "getMouseState", GetMouseState);
	NODE_SET_METHOD(exports, "getRelativeMouseMode", GetRelativeMouseMode);
	NODE_SET_METHOD(exports, "getRelativeMouseState", GetRelativeMouseState);

	NODE_SET_METHOD(exports, "setRelativeMouseMode", SetRelativeMouseMode);

	NODE_SET_METHOD(exports, "warpMouseInWindow", WarpMouseInWindow);

	NODE_SET_METHOD(exports, "button", ButtonMacroWrapper);

	Handle<Object> SYSTEM_CURSOR = Object::New();
	exports->Set(String::NewSymbol("SYSTEM_CURSOR"), SYSTEM_CURSOR);
	SYSTEM_CURSOR->Set(String::NewSymbol("ARROW"), Number::New(SDL_SYSTEM_CURSOR_ARROW));
	SYSTEM_CURSOR->Set(String::NewSymbol("IBEAM"), Number::New(SDL_SYSTEM_CURSOR_IBEAM));
	SYSTEM_CURSOR->Set(String::NewSymbol("WAIT"), Number::New(SDL_SYSTEM_CURSOR_WAIT));
	SYSTEM_CURSOR->Set(String::NewSymbol("CROSSHAIR"), Number::New(SDL_SYSTEM_CURSOR_CROSSHAIR));
	SYSTEM_CURSOR->Set(String::NewSymbol("WAITARROW"), Number::New(SDL_SYSTEM_CURSOR_WAITARROW));
	SYSTEM_CURSOR->Set(String::NewSymbol("SIZENWSE"), Number::New(SDL_SYSTEM_CURSOR_SIZENWSE));
	SYSTEM_CURSOR->Set(String::NewSymbol("SIZENESW"), Number::New(SDL_SYSTEM_CURSOR_SIZENESW));
	SYSTEM_CURSOR->Set(String::NewSymbol("SIZEWE"), Number::New(SDL_SYSTEM_CURSOR_SIZEWE));
	SYSTEM_CURSOR->Set(String::NewSymbol("SIZENS"), Number::New(SDL_SYSTEM_CURSOR_SIZENS));
	SYSTEM_CURSOR->Set(String::NewSymbol("SIZEALL"), Number::New(SDL_SYSTEM_CURSOR_SIZEALL));
	SYSTEM_CURSOR->Set(String::NewSymbol("NO"), Number::New(SDL_SYSTEM_CURSOR_NO));
	SYSTEM_CURSOR->Set(String::NewSymbol("HAND"), Number::New(SDL_SYSTEM_CURSOR_HAND));

	Handle<Object> BUTTON = Object::New();
	exports->Set(String::NewSymbol("BUTTON"), BUTTON);
	BUTTON->Set(String::NewSymbol("LEFT"), Number::New(SDL_BUTTON_LEFT));
	BUTTON->Set(String::NewSymbol("MIDDLE"), Number::New(SDL_BUTTON_MIDDLE));
	BUTTON->Set(String::NewSymbol("RIGHT"), Number::New(SDL_BUTTON_RIGHT));
	BUTTON->Set(String::NewSymbol("X1"), Number::New(SDL_BUTTON_X1));
	BUTTON->Set(String::NewSymbol("X2"), Number::New(SDL_BUTTON_X2));
	BUTTON->Set(String::NewSymbol("LMASK"), Number::New(SDL_BUTTON_LMASK));
	BUTTON->Set(String::NewSymbol("MMASK"), Number::New(SDL_BUTTON_MMASK));
	BUTTON->Set(String::NewSymbol("RMASK"), Number::New(SDL_BUTTON_RMASK));
	BUTTON->Set(String::NewSymbol("X1MASK"), Number::New(SDL_BUTTON_X1MASK));
	BUTTON->Set(String::NewSymbol("X2MASK"), Number::New(SDL_BUTTON_X2MASK));
}

Persistent<FunctionTemplate> sdl::CursorWrapper::wrap_template_;
Persistent<FunctionTemplate> sdl::CursorWrapper::wrap_template_system_;

sdl::CursorWrapper::CursorWrapper() {
}
sdl::CursorWrapper::CursorWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}
sdl::CursorWrapper::~CursorWrapper() {
	if(NULL != cursor_) {
		SDL_FreeCursor(cursor_);
	}
}

void sdl::CursorWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("CursorWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "free", FreeCursor);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "set", SetCursor);

	exports->Set(String::NewSymbol("Cursor"), wrap_template_->GetFunction());

	// System cursor wrapper.
	tpl = FunctionTemplate::New(NewSystem);
	wrap_template_system_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_system_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_system_->SetClassName(String::NewSymbol("SystemCursorWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_system_, "free", FreeCursor);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_system_, "set", SetCursor);

	exports->Set(String::NewSymbol("SystemCursor"), wrap_template_system_->GetFunction());
}

Handle<Value> sdl::CursorWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("A new Cursor must be created with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected CreateColorCursor(Surface, Number, Number)")));
	}

	SurfaceWrapper* surface = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	int x = args[1]->Int32Value();
	int y = args[2]->Int32Value();
	SDL_Cursor* cursor = SDL_CreateColorCursor(surface->surface_, x, y);
	if(NULL == cursor) {
		return ThrowSDLException(__func__);
	}

	CursorWrapper* wrap = new CursorWrapper();
	wrap->cursor_ = cursor;
	wrap->Wrap(args.This());

	return args.This();
}
Handle<Value> sdl::CursorWrapper::NewSystem(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("A new Cursor must be created with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected CreateColorCursor(Number)")));
	}

	SDL_SystemCursor id = static_cast<SDL_SystemCursor>(args[0]->Int32Value());
	SDL_Cursor* cursor = SDL_CreateSystemCursor(id);
	if(NULL == cursor) {
		return ThrowSDLException(__func__);
	}

	CursorWrapper* wrap = new CursorWrapper();
	wrap->cursor_ = cursor;
	wrap->Wrap(args.This());

	return args.This();
}

// TODO: Implement this function. See:
//       http://wiki.libsdl.org/SDL_CreateCursor?highlight=%28\bCategoryMouse\b%29|%28CategoryEnum%29|%28CategoryStruct%29
// Handle<Value> sdl::CreateCursor(const Arguments& args) {
// 	HandleScope scope;

// 	return Undefined();
// }
Handle<Value> sdl::CursorWrapper::FreeCursor(const Arguments& args) {
	HandleScope scope;

	CursorWrapper* wrap = ObjectWrap::Unwrap<CursorWrapper>(args.This());
	SDL_FreeCursor(wrap->cursor_);
	wrap->cursor_ = NULL;

	return Undefined();
}
Handle<Value> sdl::CursorWrapper::SetCursor(const Arguments& args) {
	HandleScope scope;

	CursorWrapper* wrap = ObjectWrap::Unwrap<CursorWrapper>(args.This());
	SDL_SetCursor(wrap->cursor_);

	return Undefined();
}

Handle<Value> sdl::ShowCursor(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected Cursor.show(Number)")));
	}

	int toggle = args[0]->Int32Value();
	int err = SDL_ShowCursor(toggle);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(err));
}
Handle<Value> sdl::GetCursor(const Arguments& args) {
	HandleScope scope;

	SDL_Cursor* cursor = SDL_GetCursor();
	if(NULL == cursor) {
		return Null();
	}

	Handle<Object> toWrap = Object::New();
	CursorWrapper* wrap = new CursorWrapper(toWrap);
	wrap->cursor_ = cursor;

	return scope.Close(toWrap);
}
Handle<Value> sdl::GetDefaultCursor(const Arguments& args) {
	HandleScope scope;

	SDL_Cursor* cursor = SDL_GetDefaultCursor();
	if(NULL == cursor) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> toWrap = Object::New();
	CursorWrapper* wrap = new CursorWrapper(toWrap);
	wrap->cursor_ = cursor;

	return scope.Close(toWrap);

	return Undefined();
}
Handle<Value> sdl::GetMouseFocus(const Arguments& args) {
	HandleScope scope;

	SDL_Window* window = SDL_GetMouseFocus();
	if(NULL == window) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> toWrap = Object::New();
	WindowWrapper* wrap = new WindowWrapper(toWrap);
	wrap->window_ = window;

	return scope.Close(toWrap);
}
Handle<Value> sdl::GetMouseState(const Arguments& args) {
	HandleScope scope;

	int x, y;
	uint32_t mask = SDL_GetMouseState(&x, &y);

	Handle<Array> ret = Array::New(3);
	ret->Set(0, Number::New(x));
	ret->Set(1, Number::New(y));
	ret->Set(2, Number::New(mask));

	return scope.Close(ret);
}
Handle<Value> sdl::GetRelativeMouseMode(const Arguments& args) {
	HandleScope scope;

	SDL_bool ret = SDL_GetRelativeMouseMode();

	return scope.Close(Boolean::New(ret ? true : false));
}
Handle<Value> sdl::GetRelativeMouseState(const Arguments& args) {
	HandleScope scope;

	int x, y;
	uint32_t mask = SDL_GetRelativeMouseState(&x, &y);

	Handle<Array> ret = Array::New(3);
	ret->Set(0, Number::New(x));
	ret->Set(1, Number::New(y));
	ret->Set(2, Number::New(mask));

	return scope.Close(ret);
}

Handle<Value> sdl::SetRelativeMouseMode(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected SetRelativeMouseMode(Boolean)")));
	}

	bool enabled = args[0]->BooleanValue();
	int err = SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

Handle<Value> sdl::WarpMouseInWindow(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected WarpMouseInWindow(Window, Number, Number)")));
	}

	WindowWrapper* window = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	int x = args[1]->Int32Value();
	int y = args[2]->Int32Value();
	SDL_WarpMouseInWindow(window->window_, x, y);

	return Undefined();
}

Handle<Value> sdl::ButtonMacroWrapper(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected button(Number)")));
	}

	int button = args[0]->Int32Value();
	int ret = SDL_BUTTON(button);

	return scope.Close(Number::New(ret));
}
