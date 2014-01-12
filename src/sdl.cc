#ifdef __APPLE__
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#endif

#include "SDL.h"
#include "sdl.h"
#include "struct_wrappers.h"
#include <v8.h>
#include <string>
#include <iostream>

using namespace v8;

static uv_loop_t *video_loop;

static std::map<uint32_t, std::string> event_type_to_string_;
static std::map<SDL_WindowEventID, std::string> window_event_to_string_;

extern "C" void
init(Handle<Object> target)
{
// #ifdef __APPLE__
//   // on the mac it is necessary to create to call [NSApplication sharedApplication]
//   // before we can create a rendering window
//   objc_msgSend(objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
// #endif
  std::cout << "Starting init." << std::endl;

  // Initialize the SDL event type to string mappings.
  std::cout << "Initializing SDL event type to string mappings." << std::endl;
  event_type_to_string_[SDL_DOLLARGESTURE] = "dollarGesture";
  event_type_to_string_[SDL_DROPFILE] = "dropFile";
  event_type_to_string_[SDL_FINGERMOTION] = "fingerMotion";
  event_type_to_string_[SDL_FINGERDOWN] = "fingerDown";
  event_type_to_string_[SDL_FINGERUP] = "fingerUp";
  event_type_to_string_[SDL_KEYDOWN] = "keyDown";
  event_type_to_string_[SDL_KEYUP] = "keyUp";
  event_type_to_string_[SDL_JOYAXISMOTION] = "joyAxisMotion";
  event_type_to_string_[SDL_JOYBALLMOTION] = "joyBallMotion";
  event_type_to_string_[SDL_JOYHATMOTION] = "joyHatMotion";
  event_type_to_string_[SDL_JOYBUTTONDOWN] = "joyButtonDown";
  event_type_to_string_[SDL_JOYBUTTONUP] = "joyButtonUp";
  event_type_to_string_[SDL_MOUSEMOTION] = "mouseMotion";
  event_type_to_string_[SDL_MOUSEBUTTONDOWN] = "mouseButtonDown";
  event_type_to_string_[SDL_MOUSEBUTTONUP] = "mouseButtonUp";
  event_type_to_string_[SDL_MOUSEWHEEL] = "mouseWheel";
  event_type_to_string_[SDL_MULTIGESTURE] = "multiGesture";
  event_type_to_string_[SDL_QUIT] = "quit";
  event_type_to_string_[SDL_SYSWMEVENT] = "sysWMEvent";
  event_type_to_string_[SDL_TEXTEDITING] = "textEditing";
  event_type_to_string_[SDL_TEXTINPUT] = "textInput";
  event_type_to_string_[SDL_USEREVENT] = "userEvent";
  event_type_to_string_[SDL_WINDOWEVENT] = "windowEvent";
  std::cout << "Finished initializing event mappings." << std::endl;

  // Initialize the SDL WindowEvent type to string mappings.
  std::cout << "Initializing SDL window event type to string mappings." << std::endl;
  window_event_to_string_[SDL_WINDOWEVENT_SHOWN] = "shown";
  window_event_to_string_[SDL_WINDOWEVENT_HIDDEN] = "hidden";
  window_event_to_string_[SDL_WINDOWEVENT_EXPOSED] = "exposed";
  window_event_to_string_[SDL_WINDOWEVENT_MOVED] = "moved";
  window_event_to_string_[SDL_WINDOWEVENT_RESIZED] = "resized";
  window_event_to_string_[SDL_WINDOWEVENT_SIZE_CHANGED] = "sizeChanged";
  window_event_to_string_[SDL_WINDOWEVENT_MINIMIZED] = "minimized";
  window_event_to_string_[SDL_WINDOWEVENT_MAXIMIZED] = "maximized";
  window_event_to_string_[SDL_WINDOWEVENT_RESTORED] = "restored";
  window_event_to_string_[SDL_WINDOWEVENT_ENTER] = "enter";
  window_event_to_string_[SDL_WINDOWEVENT_LEAVE] = "leave";
  window_event_to_string_[SDL_WINDOWEVENT_FOCUS_GAINED] = "focusGained";
  window_event_to_string_[SDL_WINDOWEVENT_FOCUS_LOST] = "focusLost";
  window_event_to_string_[SDL_WINDOWEVENT_CLOSE] = "close";
  std::cout << "Finished initializing window event mappings." << std::endl;

  sdl::InitWrappers(target);
  sdl::WindowWrapper::Init(target);

  // Initialization and Shutdown.
  NODE_SET_METHOD(target, "init", sdl::Init);
  NODE_SET_METHOD(target, "initSubSystem", sdl::InitSubSystem);
  NODE_SET_METHOD(target, "wasInit", sdl::WasInit);
  NODE_SET_METHOD(target, "quit", sdl::Quit);
  NODE_SET_METHOD(target, "quitSubSystem", sdl::QuitSubSystem);

  // Display and Window Management.

  NODE_SET_METHOD(target, "clearError", sdl::ClearError);
  NODE_SET_METHOD(target, "getError", sdl::GetError);
  NODE_SET_METHOD(target, "setError", sdl::SetError);

  NODE_SET_METHOD(target, "waitEvent", sdl::WaitEvent);
  NODE_SET_METHOD(target, "pollEvent", sdl::PollEvent);

  NODE_SET_METHOD(target, "numJoysticks", sdl::NumJoysticks);
  NODE_SET_METHOD(target, "joystickOpen", sdl::JoystickOpen);
  NODE_SET_METHOD(target, "joystickName", sdl::JoystickName);
  NODE_SET_METHOD(target, "joystickNumAxes", sdl::JoystickNumAxes);
  NODE_SET_METHOD(target, "joystickNumButtons", sdl::JoystickNumButtons);
  NODE_SET_METHOD(target, "joystickNumBalls", sdl::JoystickNumBalls);
  NODE_SET_METHOD(target, "joystickNumHats", sdl::JoystickNumHats);
  NODE_SET_METHOD(target, "joystickClose", sdl::JoystickClose);
  NODE_SET_METHOD(target, "joystickUpdate", sdl::JoystickUpdate);
  NODE_SET_METHOD(target, "joystickEventState", sdl::JoystickEventState);

  NODE_SET_METHOD(target, "fillRect", sdl::FillRect);
  NODE_SET_METHOD(target, "createRGBSurface", sdl::CreateRGBSurface);
  NODE_SET_METHOD(target, "blitSurface", sdl::BlitSurface);
  NODE_SET_METHOD(target, "freeSurface", sdl::FreeSurface);
  NODE_SET_METHOD(target, "setColorKey", sdl::SetColorKey);

  NODE_SET_METHOD(target, "mapRGB", sdl::MapRGB);
  NODE_SET_METHOD(target, "mapRGBA", sdl::MapRGBA);
  NODE_SET_METHOD(target, "getRGB", sdl::GetRGB);
  NODE_SET_METHOD(target, "getRGBA", sdl::GetRGBA);
  NODE_SET_METHOD(target, "setClipRect",sdl::SetClipRect);

  Local<Object> INIT = Object::New();
  target->Set(String::New("INIT"), INIT);
  INIT->Set(String::New("TIMER"), Number::New(SDL_INIT_TIMER));
  INIT->Set(String::New("AUDIO"), Number::New(SDL_INIT_AUDIO));
  INIT->Set(String::New("VIDEO"), Number::New(SDL_INIT_VIDEO));
  INIT->Set(String::New("JOYSTICK"), Number::New(SDL_INIT_JOYSTICK));
  INIT->Set(String::New("HAPTIC"), Number::New(SDL_INIT_HAPTIC));
  INIT->Set(String::New("GAMECONTROLLER"), Number::New(SDL_INIT_GAMECONTROLLER));
  INIT->Set(String::New("EVENTS"), Number::New(SDL_INIT_EVENTS));
  INIT->Set(String::New("EVERYTHING"), Number::New(SDL_INIT_EVERYTHING));
  INIT->Set(String::New("NOPARACHUTE"), Number::New(SDL_INIT_NOPARACHUTE));

  Local<Object> EVENT = Object::New();
  target->Set(String::New("EVENT"), EVENT);
  EVENT->Set(String::New("DOLLARGESTURE"), Number::New(SDL_DOLLARGESTURE));
  EVENT->Set(String::New("DROPFILE"), Number::New(SDL_DROPFILE));
  EVENT->Set(String::New("FINGERMOTION"), Number::New(SDL_FINGERMOTION));
  EVENT->Set(String::New("FINGERUP"), Number::New(SDL_FINGERUP));
  EVENT->Set(String::New("KEYDOWN"), Number::New(SDL_KEYDOWN));
  EVENT->Set(String::New("KEYUP"), Number::New(SDL_KEYUP));
  EVENT->Set(String::New("JOYAXISMOTION"), Number::New(SDL_JOYAXISMOTION));
  EVENT->Set(String::New("JOYBALLMOTION"), Number::New(SDL_JOYBALLMOTION));
  EVENT->Set(String::New("JOYHATMOTION"), Number::New(SDL_JOYHATMOTION));
  EVENT->Set(String::New("JOYBUTTONDOWN"), Number::New(SDL_JOYBUTTONDOWN));
  EVENT->Set(String::New("JOYBUTTONUP"), Number::New(SDL_JOYBUTTONUP));
  EVENT->Set(String::New("MOUSEMOTION"), Number::New(SDL_MOUSEMOTION));
  EVENT->Set(String::New("MOUSEBUTTONDOWN"), Number::New(SDL_MOUSEBUTTONDOWN));
  EVENT->Set(String::New("MOUSEBUTTONUP"), Number::New(SDL_MOUSEBUTTONUP));
  EVENT->Set(String::New("MOUSEWHEEL"), Number::New(SDL_MOUSEWHEEL));
  EVENT->Set(String::New("MULTIGESTURE"), Number::New(SDL_MULTIGESTURE));
  EVENT->Set(String::New("QUIT"), Number::New(SDL_QUIT));
  EVENT->Set(String::New("SYSWMEVENT"), Number::New(SDL_SYSWMEVENT));
  EVENT->Set(String::New("TEXTEDITING"), Number::New(SDL_TEXTEDITING));
  EVENT->Set(String::New("TEXTINPUT"), Number::New(SDL_TEXTINPUT));
  EVENT->Set(String::New("USEREVENT"), Number::New(SDL_USEREVENT));
  EVENT->Set(String::New("WINDOWEVENT"), Number::New(SDL_WINDOWEVENT));

  Local<Object> SURFACE = Object::New();
  target->Set(String::New("SURFACE"), SURFACE);
  // SURFACE->Set(String::New("ANYFORMAT"), Number::New(SDL_ANYFORMAT));
  // SURFACE->Set(String::New("ASYNCBLIT"), Number::New(SDL_ASYNCBLIT));
  // SURFACE->Set(String::New("DOUBLEBUF"), Number::New(SDL_DOUBLEBUF));
  // SURFACE->Set(String::New("HWACCEL"), Number::New(SDL_HWACCEL));
  // SURFACE->Set(String::New("HWPALETTE"), Number::New(SDL_HWPALETTE));
  // SURFACE->Set(String::New("HWSURFACE"), Number::New(SDL_HWSURFACE));
  // SURFACE->Set(String::New("FULLSCREEN"), Number::New(SDL_FULLSCREEN));
  // SURFACE->Set(String::New("OPENGL"), Number::New(SDL_OPENGL));
  // SURFACE->Set(String::New("RESIZABLE"), Number::New(SDL_RESIZABLE));
  SURFACE->Set(String::New("RLEACCEL"), Number::New(SDL_RLEACCEL));
  // SURFACE->Set(String::New("SRCALPHA"), Number::New(SDL_SRCALPHA));
  // SURFACE->Set(String::New("SRCCOLORKEY"), Number::New(SDL_SRCCOLORKEY));
  SURFACE->Set(String::New("SWSURFACE"), Number::New(SDL_SWSURFACE));
  SURFACE->Set(String::New("PREALLOC"), Number::New(SDL_PREALLOC));

  // SDL Enumerations start:

  Local<Object> AUDIOFORMAT = Object::New();
  target->Set(String::New("AUDIOFORMAT"), AUDIOFORMAT);
  AUDIOFORMAT->Set(String::New("MASK_BITSIZE"), Number::New(SDL_AUDIO_MASK_BITSIZE));
  AUDIOFORMAT->Set(String::New("MASK_DATATYPE"), Number::New(SDL_AUDIO_MASK_DATATYPE));
  AUDIOFORMAT->Set(String::New("MASK_ENDIAN"), Number::New(SDL_AUDIO_MASK_ENDIAN));
  AUDIOFORMAT->Set(String::New("MASK_SIGNED"), Number::New(SDL_AUDIO_MASK_SIGNED));

  Local<Object> TEXTUREACCESS = Object::New();
  target->Set(String::New("TEXTUREACCESS"), TEXTUREACCESS);
  TEXTUREACCESS->Set(String::New("STATIC"), Number::New(SDL_TEXTUREACCESS_STATIC));
  TEXTUREACCESS->Set(String::New("STREAMING"), Number::New(SDL_TEXTUREACCESS_STREAMING));

  Local<Object> TTF = Object::New();
  target->Set(String::New("TTF"), TTF);
  NODE_SET_METHOD(TTF, "init", sdl::TTF::Init);
  NODE_SET_METHOD(TTF, "openFont", sdl::TTF::OpenFont);
  // NODE_SET_METHOD(TTF, "renderTextBlended", sdl::TTF::RenderTextBlended);

  Local<Object> IMG = Object::New();
  target->Set(String::New("IMG"), IMG);

  NODE_SET_METHOD(IMG, "load", sdl::IMG::Load);

  Local<Object> WM = Object::New();
  target->Set(String::New("WM"), WM);

  Local<Object> GL = Object::New();
  target->Set(String::New("GL"), GL);


  NODE_SET_METHOD(GL, "setAttribute", sdl::GL::SetAttribute);
  NODE_SET_METHOD(GL, "getAttribute", sdl::GL::GetAttribute);

  GL->Set(String::New("RED_SIZE"), Number::New(SDL_GL_RED_SIZE));
  GL->Set(String::New("GREEN_SIZE"), Number::New(SDL_GL_GREEN_SIZE));
  GL->Set(String::New("BLUE_SIZE"), Number::New(SDL_GL_BLUE_SIZE));
  GL->Set(String::New("ALPHA_SIZE"), Number::New(SDL_GL_ALPHA_SIZE));
  GL->Set(String::New("DOUBLEBUFFER"), Number::New(SDL_GL_DOUBLEBUFFER));
  GL->Set(String::New("BUFFER_SIZE"), Number::New(SDL_GL_BUFFER_SIZE));
  GL->Set(String::New("DEPTH_SIZE"), Number::New(SDL_GL_DEPTH_SIZE));
  GL->Set(String::New("STENCIL_SIZE"), Number::New(SDL_GL_STENCIL_SIZE));
  GL->Set(String::New("ACCUM_RED_SIZE"), Number::New(SDL_GL_ACCUM_RED_SIZE));
  GL->Set(String::New("ACCUM_GREEN_SIZE"), Number::New(SDL_GL_ACCUM_GREEN_SIZE));
  GL->Set(String::New("ACCUM_BLUE_SIZE"), Number::New(SDL_GL_ACCUM_BLUE_SIZE));
  GL->Set(String::New("ACCUM_ALPHA_SIZE"), Number::New(SDL_GL_ACCUM_ALPHA_SIZE));

  Local<Object> HINT = Object::New();
  target->Set(String::New("HINT"), HINT);

  HINT->Set(String::New("FRAMEBUFFER_ACCELERATION"), String::New(SDL_HINT_FRAMEBUFFER_ACCELERATION));
  HINT->Set(String::New("IDLE_TIMER_DISABLED"), String::New(SDL_HINT_IDLE_TIMER_DISABLED));
  HINT->Set(String::New("ORIENTATIONS"), String::New(SDL_HINT_ORIENTATIONS));
  HINT->Set(String::New("RENDER_DRIVER"), String::New(SDL_HINT_RENDER_DRIVER));
  HINT->Set(String::New("RENDER_OPENGL_SHADERS"), String::New(SDL_HINT_RENDER_OPENGL_SHADERS));
  HINT->Set(String::New("SCALE_QUALITY"), String::New(SDL_HINT_RENDER_SCALE_QUALITY));
  HINT->Set(String::New("RENDER_VSYNC"), String::New(SDL_HINT_RENDER_VSYNC));
}


////////////////////////////////////////////////////////////////////////////////
// Initialization and Shutdown.

Handle<Value> sdl::Init(const Arguments& args) {
  HandleScope scope;

  SDL_SetMainReady();
  int init = (args[0]->IsUndefined() || !args[0]->IsNumber()) ? SDL_INIT_EVERYTHING : args[0]->Int32Value();
  std::cout << "sdl::Init got: " << init << std::endl;
  if (SDL_Init(init) < 0) {
    return ThrowSDLException(__func__);
  }

  return Undefined();
}

Handle<Value> sdl::InitSubSystem(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected InitSubSystem(Number)")));
  }

  if (SDL_InitSubSystem(args[0]->Int32Value()) < 0) return ThrowSDLException(__func__);

  return Undefined();
}

Handle<Value> sdl::Quit(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Quit()")));
  }

  SDL_Quit();

  return Undefined();
}

Handle<Value> sdl::QuitSubSystem(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected QuitSubSystem(Number)")));
  }

  SDL_QuitSubSystem(args[0]->Int32Value());

  return Undefined();
}

Handle<Value> sdl::WasInit(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WasInit(Number)")));
  }

  return Number::New(SDL_WasInit(args[0]->Int32Value()));
}

////////////////////////////////////////////////////////////////////////////////
// Display and Window Management.

Persistent<FunctionTemplate> sdl::WindowWrapper::window_wrap_template_;

sdl::WindowWrapper::WindowWrapper(std::string title, int x, int y, int w, int h, uint32_t flags) {
  window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
}

sdl::WindowWrapper::~WindowWrapper() {
  if(NULL != window_) {
    SDL_DestroyWindow(window_);
  }
}

void sdl::WindowWrapper::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  window_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

  window_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
  window_wrap_template_->SetClassName(String::NewSymbol("WindowWrapper"));

  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getBrightness", GetBrightness);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getDisplayIndex", GetDisplayIndex);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getFlags", GetFlags);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getGammaRamp", GetGammaRamp);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getGrab", GetGrab);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getWindowID", GetWindowID);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getMaximumSize", GetMaximumSize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getMinimumSize", GetMinimumSize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getPixelFormat", GetPixelFormat);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getPosition", GetPosition);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getSize", GetSize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getSurface", GetSurface);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "getTitle", GetTitle);

  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "hide", Hide);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "show", Show);

  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "minimize", Minimize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "maximize", Maximize);

  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "raise", Raise);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "restore", Restore);

  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setBordered", SetBordered);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setBrightness", SetBrightness);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setDisplayMode", SetDisplayMode);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setFullscreen", SetFullscreen);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setGammeRamp", SetGammaRamp);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setGrab", SetGrab);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setIcon", SetIcon);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setMaximumSize", SetMaximumSize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setMinimumSize", SetMinimumSize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setPosition", SetPosition);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setSize", SetSize);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "setTitle", SetTitle);

  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "updateWindowSurface", UpdateWindowSurface);
  NODE_SET_PROTOTYPE_METHOD(window_wrap_template_, "updateWindowSurfaceRects", UpdateWindowSurfaceRects);

  exports->Set(String::NewSymbol("Window"), window_wrap_template_->GetFunction());
}

Handle<Value> sdl::WindowWrapper::New(const Arguments& args) {
  if(!args.IsConstructCall()) {
    return ThrowException(Exception::TypeError(
      String::New("Use the new operator to create instances of this object.")));
  }

  HandleScope scope;

  std::string title = args[0]->IsUndefined() ? "" : *(String::Utf8Value(args[0]));
  int x = args[1]->IsUndefined() ? SDL_WINDOWPOS_UNDEFINED : args[1]->Int32Value();
  int y = args[2]->IsUndefined() ? SDL_WINDOWPOS_UNDEFINED : args[2]->Int32Value();
  int w = args[3]->IsUndefined() ? 640 : args[3]->Int32Value();
  int h = args[4]->IsUndefined() ? 480 : args[4]->Int32Value();
  uint32_t flags = args[5]->IsUndefined() ? 0 : args[5]->Int32Value();

  WindowWrapper* obj = new WindowWrapper(title, x, y, w, h, flags);
  if(NULL == obj->window_) {
    delete obj;
    return ThrowSDLException("Window->New");
  }

  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> sdl::WindowWrapper::GetBrightness(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  // SDL documentation does not say this function can return an erroneous value, so
  // we don't do any error checking.
  return scope.Close(Number::New(SDL_GetWindowBrightness(obj->window_)));
}

Handle<Value> sdl::WindowWrapper::GetDisplayIndex(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  int ret = SDL_GetWindowDisplayIndex(obj->window_);
  // SDL documentation says that values less than 0 are returned upon error.
  if(ret < 0) {
    return ThrowSDLException("Window->GetDisplayIndex");
  }

  return scope.Close(Number::New(ret));
}

Handle<Value> sdl::WindowWrapper::GetDisplayMode(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_DisplayMode mode;
  int err = SDL_GetWindowDisplayMode(obj->window_, &mode);
  // SDL documentation says that 0 is success, and less than 0 is when an error
  // occurred.
  if(err < 0) {
    return ThrowSDLException("Window->GetDisplayMode");
  }

  return scope.Close(SDLDisplayModeToJavascriptObject(mode));
}

Handle<Value> sdl::WindowWrapper::GetFlags(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  // TODO: Return an array of human-readable strings denoting each flag instead?
  // SDL documentation does not say this function can return an error code.
  return scope.Close(Number::New(SDL_GetWindowFlags(obj->window_)));
}

Handle<Value> sdl::WindowWrapper::GetGammaRamp(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  uint16_t redArr[256];
  uint16_t greenArr[256];
  uint16_t blueArr[256];
  int err = SDL_GetWindowGammaRamp(obj->window_, redArr, greenArr, blueArr);
  if(err < 0) {
    return ThrowSDLException("Window->GetGammaRamp");
  }

  Handle<Array> ret = Array::New(3);
  Handle<Array> jsRedArr = Array::New(256);
  Handle<Array> jsGreenArr = Array::New(256);
  Handle<Array> jsBlueArr = Array::New(256);
  for(int i = 0; i < 256; i++) {
    jsRedArr->Set(i, Number::New(redArr[i]));
    jsGreenArr->Set(i, Number::New(greenArr[i]));
    jsBlueArr->Set(i, Number::New(blueArr[i]));
  }

  ret->Set(0, jsRedArr);
  ret->Set(1, jsGreenArr);
  ret->Set(2, jsBlueArr);
  return scope.Close(ret);
}

Handle<Value> sdl::WindowWrapper::GetGrab(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  return scope.Close(Boolean::New(SDL_GetWindowGrab(obj->window_)));
}

Handle<Value> sdl::WindowWrapper::GetWindowID(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  return scope.Close(Number::New(SDL_GetWindowID(obj->window_)));
}

Handle<Value> sdl::WindowWrapper::GetMaximumSize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  int w, h;
  SDL_GetWindowMaximumSize(obj->window_, &w, &h);
  Handle<Array> ret = Array::New(2);
  ret->Set(0, Number::New(w));
  ret->Set(1, Number::New(h));
  return scope.Close(ret);
}

Handle<Value> sdl::WindowWrapper::GetMinimumSize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  int w, h;
  SDL_GetWindowMinimumSize(obj->window_, &w, &h);
  Handle<Array> ret = Array::New(2);
  ret->Set(0, Number::New(w));
  ret->Set(1, Number::New(h));
  return scope.Close(ret);
}

Handle<Value> sdl::WindowWrapper::GetPixelFormat(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  uint32_t ret = SDL_GetWindowPixelFormat(obj->window_);
  if(SDL_PIXELFORMAT_UNKNOWN == ret) {
    return ThrowSDLException("Window->GetPixelFormat");
  }

  return scope.Close(Number::New(ret));
}

Handle<Value> sdl::WindowWrapper::GetPosition(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  int x, y;
  SDL_GetWindowPosition(obj->window_, &x, &y);

  Handle<Array> ret = Array::New(2);
  ret->Set(1, Number::New(x));
  ret->Set(2, Number::New(y));
  return scope.Close(ret);
}

Handle<Value> sdl::WindowWrapper::GetSize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  int w, h;
  SDL_GetWindowSize(obj->window_, &w, &h);
  
  Handle<Array> ret = Array::New(2);
  ret->Set(1, Number::New(w));
  ret->Set(2, Number::New(h));
  return scope.Close(ret);
}

Handle<Value> sdl::WindowWrapper::GetSurface(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_Surface* surf = SDL_GetWindowSurface(obj->window_);
  if(NULL == surf) {
    return ThrowSDLException("Window->GetSurface");
  }
  return scope.Close(WrapSurface(surf));
}

Handle<Value> sdl::WindowWrapper::GetTitle(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  return scope.Close(String::New(SDL_GetWindowTitle(obj->window_)));
}

Handle<Value> sdl::WindowWrapper::Hide(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_HideWindow(obj->window_);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::Show(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_ShowWindow(obj->window_);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::Maximize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_MaximizeWindow(obj->window_);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::Minimize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_MinimizeWindow(obj->window_);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::Raise(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_RaiseWindow(obj->window_);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::Restore(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  SDL_RestoreWindow(obj->window_);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetBordered(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  bool bordered = args[0]->IsUndefined() ? true : args[0]->BooleanValue();
  SDL_SetWindowBordered(obj->window_, bordered ? SDL_TRUE : SDL_FALSE);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetBrightness(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  double brightness = args[0]->IsUndefined() ? 1.0 : args[0]->NumberValue();
  if(brightness > 1.0) {
    brightness = 1.0;
  }
  else if(brightness < 0.0) {
    brightness = 0.0;
  }

  SDL_SetWindowBrightness(obj->window_, static_cast<float>(brightness));
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetDisplayMode(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args[0]->IsUndefined()) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetDisplayMode(DisplayMode)")));
  }
  SDL_DisplayMode* mode = UnwrapDisplayMode(args[0]);
  int err = SDL_SetWindowDisplayMode(obj->window_, mode);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetFullscreen(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args[0]->IsUndefined()) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetFullscreen(Number)")));
  }
  int fullscreen = args[0]->Int32Value();
  int err = SDL_SetWindowFullscreen(obj->window_, fullscreen);
  if(err < 0) {
    return ThrowSDLException("Window->SetFullscreen");
  }
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetGammaRamp(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 3 ) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetGammaRamp(Array, Array, Array)")));
  }
  if(!args[0]->IsArray() || !args[1]->IsArray() || !args[2]->IsArray()) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetGammaRamp(Array, Array, Array)")));
  }

  Handle<Array> red = Handle<Array>::Cast(args[0]);
  Handle<Array> green = Handle<Array>::Cast(args[1]);
  Handle<Array> blue = Handle<Array>::Cast(args[2]);
  uint16_t redArr[256];
  uint16_t greenArr[256];
  uint16_t blueArr[256];
  for(int i = 0; i < 256; i++) {
    redArr[i] = red->CloneElementAt(i)->Int32Value();
    greenArr[i] = green->CloneElementAt(i)->Int32Value();
    blueArr[i] = blue->CloneElementAt(i)->Int32Value();
  }
  int err = SDL_SetWindowGammaRamp(obj->window_, redArr, greenArr, blueArr);
  if(err < 0) {
    return ThrowSDLException("Window->SetGammaRamp");
  }

  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetGrab(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetGrab(Boolean)")));
  }
  bool grab = args[0]->ToBoolean()->BooleanValue();
  SDL_SetWindowGrab(obj->window_, grab ? SDL_TRUE : SDL_FALSE);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetIcon(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetIcon(Surface)")));
  }
  SDL_Surface* surface = UnwrapSurface(Handle<Object>::Cast(args[0]));
  SDL_SetWindowIcon(obj->window_, surface);

  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetMaximumSize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetMaximumSize(Number, Number)")));
  }
  int max_w = args[0]->Int32Value();
  int max_h = args[0]->Int32Value();
  SDL_SetWindowMaximumSize(obj->window_, max_w, max_h);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetMinimumSize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetMinimumSize(Number, Number)")));
  }
  int min_w = args[0]->Int32Value();
  int min_h = args[0]->Int32Value();
  SDL_SetWindowMinimumSize(obj->window_, min_w, min_h);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetPosition(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetPosition(Number, Number)")));
  }
  int x = args[0]->Int32Value();
  int y = args[0]->Int32Value();
  SDL_SetWindowPosition(obj->window_, x, y);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetSize(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 2) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetSize(Number, Number)")));
  }
  int w = args[0]->Int32Value();
  int h = args[0]->Int32Value();
  SDL_SetWindowSize(obj->window_, w, h);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::SetTitle(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetTitle(String)")));
  }
  String::Utf8Value title(args[0]);
  SDL_SetWindowTitle(obj->window_, *title);
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::UpdateWindowSurface(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  int err = SDL_UpdateWindowSurface(obj->window_);
  if(err < 0) {
    return ThrowSDLException("Window->SetTitle");
  }
  return Undefined();
}

Handle<Value> sdl::WindowWrapper::UpdateWindowSurfaceRects(const Arguments& args) {
  HandleScope scope;

  WindowWrapper* obj = ObjectWrap::Unwrap<WindowWrapper>(args.This());
  if(args.Length() < 1) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected UpdateWindowSurfaceRects(Array)")));
  }

  Handle<Array> arr = Handle<Array>::Cast(args[0]);
  int len = arr->Length();
  SDL_Rect* rects = new SDL_Rect[len];
  for(int i = 0; i < len; i++) {
    rects[i] = *UnwrapRect(arr->CloneElementAt(i));
  }

  int err = SDL_UpdateWindowSurfaceRects(obj->window_, rects, len);
  delete rects;
  if(err < 0) {
    return ThrowSDLException("Window->SetTitle");
  }
  return Undefined();
}

static Handle<Value> CreateWindow(const Arguments& args) {
  return sdl::WindowWrapper::New(args);
}
static Handle<Value> CreateWindowAndRenderer(const Arguments& args) {

}
static Handle<Value> CreateWindowFrom(const Arguments& args) {

}
static Handle<Value> DestroyWindow(const Arguments& args) {

}

static Handle<Value> DisableScreenSaver(const Arguments& args) {

}
static Handle<Value> EnableScreenSaver(const Arguments& args) {

}
static Handle<Value> IsScreenSaverEnabled(const Arguments& args) {

}

static Handle<Value> GetClosestDisplayMode(const Arguments& args) {

}
static Handle<Value> GetCurrentDisplayMode(const Arguments& args) {

}
static Handle<Value> GetCurrentVideoDriver(const Arguments& args) {

}
static Handle<Value> GetDesktopDisplayMode(const Arguments& args) {

}
static Handle<Value> GetDisplayBounds(const Arguments& args) {

}
static Handle<Value> GetDisplayMode(const Arguments& args) {

}
static Handle<Value> GetDisplayName(const Arguments& args) {

}
static Handle<Value> GetNumDisplayModes(const Arguments& args) {

}
static Handle<Value> GetNumVideoDisplays(const Arguments& args) {

}
static Handle<Value> GetNumVideoDrivers(const Arguments& args) {

}
static Handle<Value> GetVideoDrivers(const Arguments& args) {

}
static Handle<Value> GetWindowFromID(const Arguments& args) {

}

static Handle<Value> ShowMessageBox(const Arguments& args) {

}
static Handle<Value> ShowSimpleMessageBox(const Arguments& args) {

}

static Handle<Value> VideoInit(const Arguments& args) {

}
static Handle<Value> VideoQuit(const Arguments& args) {

}

////////////////////////////////////////////////////////////////////////////////

Handle<Value> sdl::ClearError(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected ClearError()")));
  }

  SDL_ClearError();

  return Undefined();
}

Handle<Value> sdl::GetError(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetError()")));
  }

  return String::New(SDL_GetError());
}

Handle<Value> sdl::SetError(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsString())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetError(String)")));
  }

  String::Utf8Value message(args[1]);

  SDL_SetError(*message);

  return Undefined();
}

static Handle<Value> sdl::WaitEvent(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsFunction())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WaitEvent(Function)")));
  }

  SDL_Event e;
  int err = SDL_WaitEvent(&e);
  if(0 == err) {
    std::string err = "WaitEvent failed: ";
    err += SDL_GetError();
    return ThrowException(MakeSDLException(err.c_str()));
  }
  Handle<Value> argv[1];
  argv[0] = sdl::SDLEventToJavascriptObject(e);
  Handle<Function>::Cast(args[0])->Call(Context::GetCurrent()->Global(), 1, argv);
  return Undefined();
}

static Handle<Value> sdl::WaitEventTimeout(const Arguments& args) {
  HandleScope scope;

  if(!(args.Length() == 2 && args[0]->IsFunction() && args[1]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WaitEventTimeout(Function, Number)")));
  }

  SDL_Event e;
  int timeout = args[1]->Int32Value();
  int err = SDL_WaitEventTimeout(&e, timeout);
  if(0 == err) {
    std::string err = "WaitEventTimeout failed: ";
    err += SDL_GetError();
    return ThrowException(MakeSDLException(err.c_str()));
  }
  Handle<Value> argv[1];
  argv[0] = sdl::SDLEventToJavascriptObject(e);
  Handle<Function>::Cast(args[0])->Call(Context::GetCurrent()->Global(), 1, argv);
  return Undefined();
}

Handle<Value> sdl::PollEvent(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected PollEvent()")));
  }

  SDL_Event event;
  if (!SDL_PollEvent(&event)) {
    return Undefined();
  }

  Local<Object> evt = SDLEventToJavascriptObject(event);
  return scope.Close(evt);
}

static Handle<Value> sdl::NumJoysticks(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected NumJoysticks()")));
  }

  return Number::New(SDL_NumJoysticks());
}

static Handle<Value> sdl::JoystickOpen(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickOpen(Number)")));
  }

  SDL_Joystick* joystick = SDL_JoystickOpen(args[0]->Int32Value());
  if (!joystick) return ThrowSDLException(__func__);
  return scope.Close(WrapJoystick(joystick));
}

static Handle<Value> sdl::JoystickName(const Arguments& args) {
  return Undefined();
  // HandleScope scope;

  // if (!(args.Length() == 1 && args[0]->IsNumber())) {
  //   return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickName(Number)")));
  // }

  // return String::New(SDL_JoystickName(UnwrapJoystick(args[0])));
}

static Handle<Value> sdl::JoystickNumAxes(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumAxes(Joystick)")));
  }

  return Number::New(SDL_JoystickNumAxes(UnwrapJoystick(args[0]->ToObject())));
}

static Handle<Value> sdl::JoystickNumButtons(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumButtons(Joystick)")));
  }

  return Number::New(SDL_JoystickNumButtons(UnwrapJoystick(args[0]->ToObject())));
}

static Handle<Value> sdl::JoystickNumBalls(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumBalls(Joystick)")));
  }

  return Number::New(SDL_JoystickNumBalls(UnwrapJoystick(args[0]->ToObject())));
}

static Handle<Value> sdl::JoystickNumHats(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickNumHats(Joystick)")));
  }

  return Number::New(SDL_JoystickNumHats(UnwrapJoystick(args[0]->ToObject())));
}

static Handle<Value> sdl::JoystickClose(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickClose(Joystick)")));
  }

  SDL_JoystickClose(UnwrapJoystick(args[0]->ToObject()));

  return Undefined();
}

static Handle<Value> sdl::JoystickUpdate(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickUpdate()")));
  }

  SDL_JoystickUpdate();
  return Undefined();
}

static Handle<Value> sdl::JoystickEventState(const Arguments& args) {
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

static Handle<Value> sdl::FillRect(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 3
      && args[0]->IsObject()
      && (args[1]->IsObject() || args[1]->IsNull())
      && args[2]->IsNumber()
  )) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected FillRect(Surface, Rect, Number)")));
  }

  SDL_Surface* surface = UnwrapSurface(args[0]->ToObject());
  SDL_Rect* rect;
  if (args[1]->IsNull()) {
    rect = NULL;
  } else if (args[1]->IsArray()) {
    SDL_Rect r;
    Handle<Object> arr = args[1]->ToObject();
    r.x = arr->Get(String::New("0"))->Int32Value();
    r.y = arr->Get(String::New("1"))->Int32Value();
    r.w = arr->Get(String::New("2"))->Int32Value();
    r.h = arr->Get(String::New("3"))->Int32Value();
    rect = &r;
  } else {
    rect = UnwrapRect(args[1]->ToObject());
  }
  int color = args[2]->Int32Value();

  if (SDL_FillRect (surface, rect, color) < 0) return ThrowSDLException(__func__);

  return Undefined();
}

static Handle<Value> sdl::CreateRGBSurface(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 3 && args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected CreateRGBSurface(Number, Number, Number)")));
  }

  int flags = args[0]->Int32Value();
  int width = args[1]->Int32Value();
  int height = args[2]->Int32Value();

  SDL_Surface *surface;
  int rmask, gmask, bmask, amask;

  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif

  surface = SDL_CreateRGBSurface(flags, width, height, 32, rmask, gmask, bmask, amask);
  if (surface == NULL) return ThrowSDLException(__func__);
  return scope.Close(WrapSurface(surface));
}

static Handle<Value> sdl::BlitSurface(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 4
        && args[0]->IsObject()
        && (args[1]->IsObject() || args[1]->IsNull())
        && args[2]->IsObject()
        && (args[3]->IsObject() || args[3]->IsNull())
  )) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected BlitSurface(Surface, Rect, Surface, Rect)")));
  }

  SDL_Surface* src = UnwrapSurface(args[0]->ToObject());
  SDL_Surface* dst = UnwrapSurface(args[2]->ToObject());

  SDL_Rect* srcrect;
  if (args[1]->IsNull()) {
    srcrect = NULL;
  } else if (args[1]->IsArray()) {
    Handle<Object> arr1 = args[1]->ToObject();
    srcrect = new SDL_Rect();
    srcrect->x = arr1->Get(String::New("0"))->Int32Value();
    srcrect->y = arr1->Get(String::New("1"))->Int32Value();
    srcrect->w = arr1->Get(String::New("2"))->Int32Value();
    srcrect->h = arr1->Get(String::New("3"))->Int32Value();
  } else {
    srcrect = UnwrapRect(args[1]->ToObject());
  }

  SDL_Rect* dstrect;
  if (args[3]->IsNull()) {
    dstrect = NULL;
  } else if (args[3]->IsArray()) {
    Handle<Object> arr2 = args[3]->ToObject();
    dstrect = new SDL_Rect();
    dstrect->x = arr2->Get(String::New("0"))->Int32Value();
    dstrect->y = arr2->Get(String::New("1"))->Int32Value();
    dstrect->w = arr2->Get(String::New("2"))->Int32Value();
    dstrect->h = arr2->Get(String::New("3"))->Int32Value();
  } else {
    dstrect = UnwrapRect(args[3]->ToObject());
  }

//  if (srcrect) printf("srcrect = {x: %d, y: %d, w: %d, h: %d}\n", srcrect->x, srcrect->y, srcrect->w, srcrect->h);
//  else printf("srcrect = null\n");
//  if (dstrect) printf("dstrect = {x: %d, y: %d, w: %d, h: %d}\n", dstrect->x, dstrect->y, dstrect->w, dstrect->h);
//  else printf("dstrect = null\n");


  if (SDL_BlitSurface(src, srcrect, dst, dstrect) < 0) return ThrowSDLException(__func__);
  return Undefined();
}

static Handle<Value> sdl::FreeSurface(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected FreeSurface(Surface)")));
  }

  // TODO: find a way to do this automatically by using GC hooks.  This is dangerous in JS land
  SDL_FreeSurface(UnwrapSurface(args[0]->ToObject()));
  args[0]->ToObject()->Set(String::New("DEAD"), Boolean::New(true));

  return Undefined();
}

static Handle<Value> sdl::SetColorKey(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 3 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetColorKey(Surface, Number, Number)")));
  }

  SDL_Surface* surface = UnwrapSurface(args[0]->ToObject());
  int flag = args[1]->Int32Value();
  int key = args[2]->Int32Value();

  if (SDL_SetColorKey(surface, flag, key) < 0) return ThrowSDLException(__func__);

  return Undefined();

}

static Handle<Value> sdl::MapRGB(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 4 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected MapRGB(PixelFormat, Number, Number, Number)")));
  }

  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[0]->ToObject());
  int r = args[1]->Int32Value();
  int g = args[2]->Int32Value();
  int b = args[3]->Int32Value();

  return Number::New(SDL_MapRGB(fmt, r, g, b));
}

static Handle<Value> sdl::MapRGBA(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 5 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber() && args[4]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected MapRGBA(PixelFormat, Number, Number, Number, Number)")));
  }

  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[0]->ToObject());
  int r = args[1]->Int32Value();
  int g = args[2]->Int32Value();
  int b = args[3]->Int32Value();
  int a = args[4]->Int32Value();

  return Number::New(SDL_MapRGBA(fmt, r, g, b, a));
}

static Handle<Value> sdl::GetRGB(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetRGB(Number, PixelFormat)")));
  }

  int pixel = args[0]->Int32Value();
  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[1]->ToObject());
  ::Uint8 r, g, b;

  SDL_GetRGB(pixel, fmt, &r, &g, &b);

  Local<Object> rgb = Object::New();
  rgb->Set(String::New("r"), Number::New(r));
  rgb->Set(String::New("g"), Number::New(g));
  rgb->Set(String::New("b"), Number::New(b));

  return scope.Close(rgb);
}

static Handle<Value> sdl::GetRGBA(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetRGBA(Number, PixelFormat)")));
  }

  int pixel = args[0]->Int32Value();
  SDL_PixelFormat* fmt = UnwrapPixelFormat(args[1]->ToObject());
  ::Uint8 r, g, b, a;

  SDL_GetRGBA(pixel, fmt, &r, &g, &b, &a);

  Local<Object> rgba = Object::New();
  rgba->Set(String::New("r"), Number::New(r));
  rgba->Set(String::New("g"), Number::New(g));
  rgba->Set(String::New("b"), Number::New(b));
  rgba->Set(String::New("a"), Number::New(a));

  return scope.Close(rgba);
}


static Handle<Value> sdl::SetClipRect(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetClipRect(SDL_Surface, SDL_Rect)")));
  }    
  
  SDL_Surface* surface = UnwrapSurface(args[0]->ToObject());
  SDL_Rect* rect;
  if (args[1]->IsNull()) {
    rect = NULL;
  } else if (args[1]->IsArray()) {
    SDL_Rect r;
    Handle<Object> arr = args[1]->ToObject();
    r.x = arr->Get(String::New("0"))->Int32Value();
    r.y = arr->Get(String::New("1"))->Int32Value();
    r.w = arr->Get(String::New("2"))->Int32Value();
    r.h = arr->Get(String::New("3"))->Int32Value();
    rect = &r;
  } else {
    rect = UnwrapRect(args[1]->ToObject());
  }
  if (SDL_SetClipRect (surface, rect ) < 0) return ThrowSDLException(__func__);

  return Undefined();
}

static Handle<Value> sdl::TTF::Init(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::Init()")));
  }

  if (TTF_Init() < 0) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("TTF::Init: "),
      String::New(TTF_GetError())
    )));
  }

  return Undefined();
}

static Handle<Value> sdl::TTF::OpenFont(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::OpenFont(String, Number)")));
  }

  String::Utf8Value file(args[0]);
  int ptsize = (args[1]->Int32Value());

  TTF_Font* font = TTF_OpenFont(*file, ptsize);
  if (font == NULL) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("TTF::OpenFont: "),
      String::New(TTF_GetError())
    )));
  }
  return scope.Close(WrapFont(font));
}

// TODO: Rewrite for SDL2.
// static Handle<Value> sdl::TTF::RenderTextBlended(const Arguments& args) {
//   HandleScope scope;

//   if (!(args.Length() == 3 && args[0]->IsObject() && args[1]->IsString() && args[2]->IsNumber())) {
//     return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::RenderTextBlended(Font, String, Number)")));
//   }

//   SDL_PixelFormat* vfmt = SDL_GetVideoInfo()->vfmt;
//   TTF_Font* font = UnwrapFont(args[0]->ToObject());
//   String::Utf8Value text(args[1]);
//   int colorCode = args[2]->Int32Value();

//   Uint8 r, g, b;
//   SDL_GetRGB(colorCode, vfmt, &r, &g, &b);

//   SDL_Color color;
//   color.r = r;
//   color.g = g;
//   color.b = b;

//   SDL_Surface *resulting_text;
//   resulting_text = TTF_RenderText_Blended(font, *text, color);
//   if (!resulting_text) {
//     return ThrowException(Exception::Error(String::Concat(
//       String::New("TTF::RenderTextBlended: "),
//       String::New(TTF_GetError())
//     )));
//   }
//   return scope.Close(WrapSurface(resulting_text));
// }

// TODO: make an async version so this can be used in loops or parallel load images
static Handle<Value> sdl::IMG::Load(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsString())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected IMG::Load(String)")));
  }

  String::Utf8Value file(args[0]);

  SDL_Surface *image;
  image=IMG_Load(*file);
  if(!image) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("IMG::Load: "),
      String::New(IMG_GetError())
    )));
  }

  return scope.Close(WrapSurface(image));
}

Handle<Value> sdl::GL::SetAttribute(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetAttribute(Number, Number)")));
  }

  int attr = args[0]->Int32Value();
  int value = args[1]->Int32Value();

  if (SDL_GL_SetAttribute((SDL_GLattr)attr, value)) return ThrowSDLException(__func__);
  return Undefined();
}

Handle<Value> sdl::GL::GetAttribute(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetAttribute(Number)")));
  }

  int attr = args[0]->Int32Value();
  int value;

  if (SDL_GL_GetAttribute((SDL_GLattr)attr, &value)) return ThrowSDLException(__func__);

  return Number::New(value);
}

NODE_MODULE(node_sdl, init)
