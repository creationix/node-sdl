#include "sdl_gl.h"
#include "window.h"
#include "helpers.h"
#include "texture.h"

using namespace v8;
using namespace node;


void sdl::gl::Init(Handle<Object> exports) {
	Local<Object> GL = Object::New();
	exports->Set(String::New("GL"), GL);
	ContextWrapper::Init(GL);

	NODE_SET_METHOD(GL, "bindTexture", BindTexture);
	NODE_SET_METHOD(GL, "unbindTexture", UnbindTexture);

	NODE_SET_METHOD(GL, "extensionSupported", ExtensionSupported);

	NODE_SET_METHOD(GL, "loadLibrary", LoadLibrary);
	NODE_SET_METHOD(GL, "unloadLibrary", UnloadLibrary);

	NODE_SET_METHOD(GL, "setAttribute", SetAttribute);
	NODE_SET_METHOD(GL, "makeCurrent", MakeCurrent);
	NODE_SET_METHOD(GL, "setSwapInterval", SetSwapInterval);

	NODE_SET_METHOD(GL, "getAttribute", GetAttribute);
	NODE_SET_METHOD(GL, "getCurrentContext", GetCurrentContext);
	NODE_SET_METHOD(GL, "getCurrentWindow", GetCurrentWindow);
	NODE_SET_METHOD(GL, "getDrawableSize", GetDrawableSize);
	NODE_SET_METHOD(GL, "getSwapInterval", GetSwapInterval);

	// SDL_GLattr enum.
	GL->Set(String::New("RED_SIZE"), Number::New(SDL_GL_RED_SIZE));
	GL->Set(String::New("GREEN_SIZE"), Number::New(SDL_GL_GREEN_SIZE));
	GL->Set(String::New("BLUE_SIZE"), Number::New(SDL_GL_BLUE_SIZE));
	GL->Set(String::New("ALPHA_SIZE"), Number::New(SDL_GL_ALPHA_SIZE));
	GL->Set(String::New("BUFFER_SIZE"), Number::New(SDL_GL_BUFFER_SIZE));
	GL->Set(String::New("DOUBLEBUFFER"), Number::New(SDL_GL_DOUBLEBUFFER));
	GL->Set(String::New("DEPTH_SIZE"), Number::New(SDL_GL_DEPTH_SIZE));
	GL->Set(String::New("STENCIL_SIZE"), Number::New(SDL_GL_STENCIL_SIZE));
	GL->Set(String::New("ACCUM_RED_SIZE"), Number::New(SDL_GL_ACCUM_RED_SIZE));
	GL->Set(String::New("ACCUM_GREEN_SIZE"), Number::New(SDL_GL_ACCUM_GREEN_SIZE));
	GL->Set(String::New("ACCUM_BLUE_SIZE"), Number::New(SDL_GL_ACCUM_BLUE_SIZE));
	GL->Set(String::New("ACCUM_ALPHA_SIZE"), Number::New(SDL_GL_ACCUM_ALPHA_SIZE));
	GL->Set(String::New("STEREO"), Number::New(SDL_GL_STEREO));
	GL->Set(String::New("MULTISAMPLEBUFFERS"), Number::New(SDL_GL_MULTISAMPLEBUFFERS));
	GL->Set(String::New("MULTISAMPLESAMPLES"), Number::New(SDL_GL_MULTISAMPLESAMPLES));
	GL->Set(String::New("ACCELERATED_VISUAL"), Number::New(SDL_GL_ACCELERATED_VISUAL));
	GL->Set(String::New("RETAINED_BACKING"), Number::New(SDL_GL_RETAINED_BACKING));
	GL->Set(String::New("CONTEXT_MAJOR_VERSION"), Number::New(SDL_GL_CONTEXT_MAJOR_VERSION));
	GL->Set(String::New("CONTEXT_MINOR_VERSION"), Number::New(SDL_GL_CONTEXT_MINOR_VERSION));
	GL->Set(String::New("CONTEXT_EGL"), Number::New(SDL_GL_CONTEXT_EGL));
	GL->Set(String::New("CONTEXT_FLAGS"), Number::New(SDL_GL_CONTEXT_FLAGS));
	GL->Set(String::New("CONTEXT_PROFILE_MASK"), Number::New(SDL_GL_CONTEXT_PROFILE_MASK));
	GL->Set(String::New("SHARE_WITH_CURRENT_CONTEXT"), Number::New(SDL_GL_SHARE_WITH_CURRENT_CONTEXT));
	GL->Set(String::New("FRAMEBUFFER_SRGB_CAPABLE"), Number::New(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE));
	// SDL_GLprofile enum.
	GL->Set(String::New("CONTEXT_PROFILE_CORE"), Number::New(SDL_GL_CONTEXT_PROFILE_CORE));
	GL->Set(String::New("CONTEXT_PROFILE_COMPATIBILITY"), Number::New(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY));
	GL->Set(String::New("PROFILE_ES"), Number::New(SDL_GL_CONTEXT_PROFILE_ES));
	// SDL_GLcontextFlag enum.
	GL->Set(String::New("CONTEXT_DEBUG_FLAG"), Number::New(SDL_GL_CONTEXT_DEBUG_FLAG));
	GL->Set(String::New("CONTEXT_FORWARD_COMPATIBLE_FLAG"), Number::New(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));
	GL->Set(String::New("CONTEXT_ROBUST_ACCESS_FLAG"), Number::New(SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG));
	GL->Set(String::New("CONTEXT_RESET_ISOLATION_FLAG"), Number::New(SDL_GL_CONTEXT_RESET_ISOLATION_FLAG));
}

Persistent<FunctionTemplate> sdl::gl::ContextWrapper::wrap_template_;

sdl::gl::ContextWrapper::ContextWrapper() {
}
sdl::gl::ContextWrapper::ContextWrapper(Handle<Object> obj) {
	Wrap(obj);
}
sdl::gl::ContextWrapper::~ContextWrapper() {
	if(NULL != context_) {
		SDL_GL_DeleteContext(context_);
	}
}

void sdl::gl::ContextWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("ContextWrapper"));
}

Handle<Value> sdl::gl::ContextWrapper::New(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected new sdl.ContextWrapper(Window)")));
	}

	WindowWrapper* window = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	SDL_GLContext context = SDL_GL_CreateContext(window->window_);
	if(NULL == context) {
		return ThrowSDLException(__func__);
	}

	ContextWrapper* obj = new ContextWrapper();
	obj->context_ = context;
	obj->Wrap(args.This());
	return args.This();
}

Handle<Value> sdl::gl::BindTexture(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Must create a Context with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected BindTexture(Texture)")));
	}

	TextureWrapper* obj = ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(args[0]));
	float texw, texh;
	int err = SDL_GL_BindTexture(obj->texture_, &texw, &texh);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Array> ret = Array::New(2);
	ret->Set(0, Number::New(texw));
	ret->Set(1, Number::New(texh));
	return scope.Close(ret);
}
Handle<Value> sdl::gl::UnbindTexture(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected BindTexture(Texture)")));
	}

	TextureWrapper* obj = ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(args[0]));
	int err = SDL_GL_UnbindTexture(obj->texture_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

Handle<Value> sdl::gl::ExtensionSupported(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected ExtensionSupported(String)")));
	}

	String::Utf8Value name(args[0]);
	SDL_bool ret = SDL_GL_ExtensionSupported(*name);
	return scope.Close(Boolean::New(ret ? true : false));
}

Handle<Value> sdl::gl::LoadLibrary(const Arguments& args) {
	HandleScope scope;

	int err;
	if(args[0]->IsUndefined()) {
		err = SDL_GL_LoadLibrary(NULL);
	}
	else {
		String::Utf8Value name(args[0]);
		err = SDL_GL_LoadLibrary(*name);
	}
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::gl::UnloadLibrary(const Arguments& args) {
	HandleScope scope;

	SDL_GL_UnloadLibrary();
	
	return Undefined();
}

Handle<Value> sdl::gl::SetAttribute(const Arguments& args) {
	HandleScope scope;

	if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber())) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetAttribute(Number, Number)")));
	}

	int attr = args[0]->Int32Value();
	int value = args[1]->Int32Value();

	if (SDL_GL_SetAttribute((SDL_GLattr)attr, value)) return ThrowSDLException(__func__);
	return Undefined();
}
Handle<Value> sdl::gl::MakeCurrent(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected MakeCurrent(Window, GLContext)")));
	}

	WindowWrapper* window = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	ContextWrapper* context = ObjectWrap::Unwrap<ContextWrapper>(Handle<Object>::Cast(args[1]));
	int err = SDL_GL_MakeCurrent(window->window_, context->context_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::gl::SetSwapInterval(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Exception SetSwapInterval(Number)")));
	}

	int interval = args[0]->Int32Value();
	int err = SDL_GL_SetSwapInterval(interval);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

Handle<Value> sdl::gl::GetAttribute(const Arguments& args) {
	HandleScope scope;

	if (!(args.Length() == 1 && args[0]->IsNumber())) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetAttribute(Number)")));
	}

	int attr = args[0]->Int32Value();
	int value;

	if (SDL_GL_GetAttribute((SDL_GLattr)attr, &value)) return ThrowSDLException(__func__);

	return Number::New(value);
}
Handle<Value> sdl::gl::GetCurrentContext(const Arguments& args) {
	HandleScope scope;

	SDL_GLContext context = SDL_GL_GetCurrentContext();
	if(NULL == context) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> newObj = Object::New();
	ContextWrapper* wrap = new ContextWrapper(newObj);
	wrap->context_ = context;
	return scope.Close(newObj);
}
Handle<Value> sdl::gl::GetCurrentWindow(const Arguments& args) {
	HandleScope scope;

	SDL_Window* window = SDL_GL_GetCurrentWindow();
	if(NULL == window) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> newObj = Object::New();
	WindowWrapper* wrap = new WindowWrapper(newObj);
	wrap->window_ = window;
	return scope.Close(newObj);
}
// TODO: Migrate to WindowWrapper object.
Handle<Value> sdl::gl::GetDrawableSize(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected GetDrawableSize(Window)")));
	}

	WindowWrapper* wrap = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	int w, h;
	SDL_GL_GetDrawableSize(wrap->window_, &w, &h);

	Handle<Array> ret = Array::New(2);
	ret->Set(0, Number::New(w));
	ret->Set(0, Number::New(h));
	return scope.Close(ret);
}
// TODO: Handle this somehow?
// Theoretically handleable with Handle<External>, but I don't see any use in wrapping a function
// pointer for Javascript. Better off loading addresses invisibly when SDL/OpenGl gets initialized
// and providing an API to access the functions.
// Handle<Value> sdl::gl::GetProcAddress(const Arguments& args) {
// }
Handle<Value> sdl::gl::GetSwapInterval(const Arguments& args) {
	HandleScope scope;

	int interval = SDL_GL_GetSwapInterval();
	return scope.Close(Number::New(interval));
}


