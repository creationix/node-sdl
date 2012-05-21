#ifdef __APPLE__
#include <objc/objc.h>
#include <objc/objc-runtime.h>
#endif

#include "SDL.h"
#include "sdl.h"
#include <v8.h>

using namespace v8;

extern "C" void
init(Handle<Object> target)
{
#ifdef __APPLE__
  // on the mac it is necessary to create to call [NSApplication sharedApplication]
  // before we can create a rendering window
  objc_msgSend(objc_lookUpClass("NSApplication"), sel_getUid("sharedApplication"));
#endif
    
  NODE_SET_METHOD(target, "init", sdl::Init);
  NODE_SET_METHOD(target, "initSubSystem", sdl::InitSubSystem);
  NODE_SET_METHOD(target, "quit", sdl::Quit);
  NODE_SET_METHOD(target, "quitSubSystem", sdl::QuitSubSystem);
  NODE_SET_METHOD(target, "wasInit", sdl::WasInit);
  NODE_SET_METHOD(target, "clearError", sdl::ClearError);
  NODE_SET_METHOD(target, "getError", sdl::GetError);
  NODE_SET_METHOD(target, "setError", sdl::SetError);
  NODE_SET_METHOD(target, "waitEvent", sdl::WaitEvent);
  NODE_SET_METHOD(target, "pollEvent", sdl::PollEvent);
  NODE_SET_METHOD(target, "setVideoMode", sdl::SetVideoMode);
  NODE_SET_METHOD(target, "videoModeOK", sdl::VideoModeOK);
  NODE_SET_METHOD(target, "numJoysticks", sdl::NumJoysticks);
  NODE_SET_METHOD(target, "joystickOpen", sdl::JoystickOpen);
  NODE_SET_METHOD(target, "joystickOpened", sdl::JoystickOpened);
  NODE_SET_METHOD(target, "joystickName", sdl::JoystickName);
  NODE_SET_METHOD(target, "joystickNumAxes", sdl::JoystickNumAxes);
  NODE_SET_METHOD(target, "joystickNumButtons", sdl::JoystickNumButtons);
  NODE_SET_METHOD(target, "joystickNumBalls", sdl::JoystickNumBalls);
  NODE_SET_METHOD(target, "joystickNumHats", sdl::JoystickNumHats);
  NODE_SET_METHOD(target, "joystickClose", sdl::JoystickClose);
  NODE_SET_METHOD(target, "joystickUpdate", sdl::JoystickUpdate);
  NODE_SET_METHOD(target, "joystickEventState", sdl::JoystickEventState);
  NODE_SET_METHOD(target, "flip", sdl::Flip);
  NODE_SET_METHOD(target, "fillRect", sdl::FillRect);
  NODE_SET_METHOD(target, "updateRect", sdl::UpdateRect);
  NODE_SET_METHOD(target, "createRGBSurface", sdl::CreateRGBSurface);
  NODE_SET_METHOD(target, "blitSurface", sdl::BlitSurface);
  NODE_SET_METHOD(target, "freeSurface", sdl::FreeSurface);
  NODE_SET_METHOD(target, "setColorKey", sdl::SetColorKey);
  NODE_SET_METHOD(target, "displayFormat", sdl::DisplayFormat);
  NODE_SET_METHOD(target, "displayFormatAlpha", sdl::DisplayFormatAlpha);
  NODE_SET_METHOD(target, "setAlpha", sdl::SetAlpha);
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
  INIT->Set(String::New("EVERYTHING"), Number::New(SDL_INIT_EVERYTHING));
  INIT->Set(String::New("NOPARACHUTE"), Number::New(SDL_INIT_NOPARACHUTE));

  Local<Object> SURFACE = Object::New();
  target->Set(String::New("SURFACE"), SURFACE);
  SURFACE->Set(String::New("ANYFORMAT"), Number::New(SDL_ANYFORMAT));
  SURFACE->Set(String::New("ASYNCBLIT"), Number::New(SDL_ASYNCBLIT));
  SURFACE->Set(String::New("DOUBLEBUF"), Number::New(SDL_DOUBLEBUF));
  SURFACE->Set(String::New("HWACCEL"), Number::New(SDL_HWACCEL));
  SURFACE->Set(String::New("HWPALETTE"), Number::New(SDL_HWPALETTE));
  SURFACE->Set(String::New("HWSURFACE"), Number::New(SDL_HWSURFACE));
  SURFACE->Set(String::New("FULLSCREEN"), Number::New(SDL_FULLSCREEN));
  SURFACE->Set(String::New("OPENGL"), Number::New(SDL_OPENGL));
  SURFACE->Set(String::New("RESIZABLE"), Number::New(SDL_RESIZABLE));
  SURFACE->Set(String::New("RLEACCEL"), Number::New(SDL_RLEACCEL));
  SURFACE->Set(String::New("SRCALPHA"), Number::New(SDL_SRCALPHA));
  SURFACE->Set(String::New("SRCCOLORKEY"), Number::New(SDL_SRCCOLORKEY));
  SURFACE->Set(String::New("SWSURFACE"), Number::New(SDL_SWSURFACE));
  SURFACE->Set(String::New("PREALLOC"), Number::New(SDL_PREALLOC));

  Local<Object> TTF = Object::New();
  target->Set(String::New("TTF"), TTF);
  NODE_SET_METHOD(TTF, "init", sdl::TTF::Init);
  NODE_SET_METHOD(TTF, "openFont", sdl::TTF::OpenFont);
  NODE_SET_METHOD(TTF, "renderTextBlended", sdl::TTF::RenderTextBlended);

  Local<Object> IMG = Object::New();
  target->Set(String::New("IMG"), IMG);

  NODE_SET_METHOD(IMG, "load", sdl::IMG::Load);

  Local<Object> WM = Object::New();
  target->Set(String::New("WM"), WM);

  NODE_SET_METHOD(WM, "setCaption", sdl::WM::SetCaption);
  NODE_SET_METHOD(WM, "setIcon", sdl::WM::SetIcon);

  Local<Object> GL = Object::New();
  target->Set(String::New("GL"), GL);


  NODE_SET_METHOD(GL, "setAttribute", sdl::GL::SetAttribute);
  NODE_SET_METHOD(GL, "getAttribute", sdl::GL::GetAttribute);
  NODE_SET_METHOD(GL, "swapBuffers", sdl::GL::SwapBuffers);

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
}

////////////////////////////////////////////////////////////////////////////////

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

Handle<Value> sdl::GL::SwapBuffers(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SwapBuffers()")));
  }

  SDL_GL_SwapBuffers();
  return Undefined();
}

Handle<Value> sdl::Init(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Init(Number)")));
  }

  if (SDL_Init(args[0]->Int32Value()) < 0) return ThrowSDLException(__func__);

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

static void sdl::EIO_WaitEvent(eio_req *req) {
  sdl::closure_t *closure = (sdl::closure_t *) req->data;
  closure->status = SDL_WaitEvent(NULL);
}

static int sdl::EIO_OnEvent(eio_req *req) {
  HandleScope scope;

  sdl::closure_t *closure = (sdl::closure_t *) req->data;
  ev_unref(EV_DEFAULT_UC);

  Handle<Value> argv[1];
  if (closure->status == 0) {
    argv[0] = MakeSDLException("WaitEvent");
  } else {
    argv[0] = Undefined();
  }

  closure->fn->Call(Context::GetCurrent()->Global(), 1, argv);

  closure->fn.Dispose();
  free(closure);
  return 0;
}

static Handle<Value> sdl::WaitEvent(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsFunction())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WaitEvent(Function)")));
  }

  closure_t *closure = (closure_t*) malloc(sizeof(closure_t));
  closure->fn = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  eio_custom(EIO_WaitEvent, EIO_PRI_DEFAULT, EIO_OnEvent, closure);
  ev_ref(EV_DEFAULT_UC);
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

  Local<Object> evt = Object::New();

  switch (event.type) {
    case SDL_ACTIVEEVENT:
      evt->Set(String::New("type"), String::New("ACTIVEEVENT"));
      evt->Set(String::New("gain"), Boolean::New(event.active.gain));
      evt->Set(String::New("state"), Number::New(event.active.state));
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      evt->Set(String::New("type"), String::New(event.type == SDL_KEYDOWN ? "KEYDOWN" : "KEYUP"));
      evt->Set(String::New("scancode"), Number::New(event.key.keysym.scancode));
      evt->Set(String::New("sym"), Number::New(event.key.keysym.sym));
      evt->Set(String::New("mod"), Number::New(event.key.keysym.mod));
      break;
    case SDL_MOUSEMOTION:
      evt->Set(String::New("type"), String::New("MOUSEMOTION"));
      evt->Set(String::New("state"), Number::New(event.motion.state));
      evt->Set(String::New("which"), Number::New(event.motion.which));
      evt->Set(String::New("x"), Number::New(event.motion.x));
      evt->Set(String::New("y"), Number::New(event.motion.y));
      evt->Set(String::New("xrel"), Number::New(event.motion.xrel));
      evt->Set(String::New("yrel"), Number::New(event.motion.yrel));
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      evt->Set(String::New("type"), String::New(event.type == SDL_MOUSEBUTTONDOWN ? "MOUSEBUTTONDOWN" : "MOUSEBUTTONUP"));
      evt->Set(String::New("button"), Number::New(event.button.button));
      evt->Set(String::New("which"), Number::New(event.button.which));
      evt->Set(String::New("x"), Number::New(event.button.x));
      evt->Set(String::New("y"), Number::New(event.button.y));
      break;
    case SDL_JOYAXISMOTION:
      evt->Set(String::New("type"), String::New("JOYAXISMOTION"));
      evt->Set(String::New("which"), Number::New(event.jaxis.which));
      evt->Set(String::New("axis"), Number::New(event.jaxis.axis));
      evt->Set(String::New("value"), Number::New(event.jaxis.value));
      break;
    case SDL_JOYBALLMOTION:
      evt->Set(String::New("type"), String::New("JOYBALLMOTION"));
      evt->Set(String::New("which"), Number::New(event.jball.which));
      evt->Set(String::New("ball"), Number::New(event.jball.ball));
      evt->Set(String::New("xrel"), Number::New(event.jball.xrel));
      evt->Set(String::New("yrel"), Number::New(event.jball.yrel));
      break;
    case SDL_JOYHATMOTION:
      evt->Set(String::New("type"), String::New("JOYHATMOTION"));
      evt->Set(String::New("which"), Number::New(event.jhat.which));
      evt->Set(String::New("hat"), Number::New(event.jhat.hat));
      evt->Set(String::New("value"), Number::New(event.jhat.value));
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      evt->Set(String::New("type"), String::New(event.type == SDL_JOYBUTTONDOWN ? "JOYBUTTONDOWN" : "JOYBUTTONUP"));
      evt->Set(String::New("which"), Number::New(event.jbutton.which));
      evt->Set(String::New("button"), Number::New(event.jbutton.button));
      break;
    case SDL_QUIT:
      evt->Set(String::New("type"), String::New("QUIT"));
      break;
    default:
      evt->Set(String::New("type"), String::New("UNKNOWN"));
      evt->Set(String::New("typeCode"), Number::New(event.type));
      break;
  }

  return scope.Close(evt);
}

static Handle<Value> sdl::SetVideoMode(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 4 && args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetVideoMode(Number, Number, Number, Number)")));
  }

  int width = (args[0]->Int32Value());
  int height = (args[1]->Int32Value());
  int bpp = (args[2]->Int32Value());
  int flags = (args[3]->Int32Value());

  SDL_Surface* screen = SDL_SetVideoMode(width, height, bpp, flags);
  if (screen == NULL) return ThrowSDLException(__func__);
  return scope.Close(WrapSurface(screen));
}

static Handle<Value> sdl::VideoModeOK(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 4 && args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected VideoModeOK(Number, Number, Number, Number)")));
  }

  int width = (args[0]->Int32Value());
  int height = (args[1]->Int32Value());
  int bpp = (args[2]->Int32Value());
  int flags = (args[3]->Int32Value());

  return Number::New(SDL_VideoModeOK(width, height, bpp, flags));
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

static Handle<Value> sdl::JoystickOpened(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickOpened(Number)")));
  }

  return Number::New(SDL_JoystickOpened(args[0]->Int32Value()));
}


static Handle<Value> sdl::JoystickName(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JoystickName(Number)")));
  }

  return String::New(SDL_JoystickName(args[0]->Int32Value()));
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


static Handle<Value> sdl::Flip(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Flip(Surface)")));
  }

  SDL_Flip(UnwrapSurface(args[0]->ToObject()));

  return Undefined();
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

static Handle<Value> sdl::UpdateRect(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2
      && args[0]->IsObject()
      && (args[1]->IsObject() || args[1]->IsNull())
  )) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected UpdateRect(Surface, Rect)")));
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

  SDL_UpdateRect(surface, rect->x, rect->y, rect->w, rect->h);

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

static Handle<Value> sdl::DisplayFormat(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected DisplayFormat(Surface)")));
  }

  SDL_Surface* surface = UnwrapSurface(args[0]->ToObject());

  return scope.Close(WrapSurface(SDL_DisplayFormat(surface)));
}

static Handle<Value> sdl::DisplayFormatAlpha(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected DisplayFormatAlpha(Surface)")));
  }

  SDL_Surface* surface = UnwrapSurface(args[0]->ToObject());

  return scope.Close(WrapSurface(SDL_DisplayFormatAlpha(surface)));
}

static Handle<Value> sdl::SetAlpha(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 3 && args[0]->IsObject() && args[1]->IsNumber() && args[2]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetAlpha(Surface, Number, Number)")));
  }

  SDL_Surface* surface = UnwrapSurface(args[0]->ToObject());
  int flags = args[1]->Int32Value();
  int alpha = args[2]->Int32Value();

  if (SDL_SetAlpha(surface, flags, alpha) < 0) return ThrowSDLException(__func__);

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

static Handle<Value> sdl::TTF::RenderTextBlended(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 3 && args[0]->IsObject() && args[1]->IsString() && args[2]->IsNumber())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::RenderTextBlended(Font, String, Number)")));
  }

  SDL_PixelFormat* vfmt = SDL_GetVideoInfo()->vfmt;
  TTF_Font* font = UnwrapFont(args[0]->ToObject());
  String::Utf8Value text(args[1]);
  int colorCode = args[2]->Int32Value();

  Uint8 r, g, b;
  SDL_GetRGB(colorCode, vfmt, &r, &g, &b);

  SDL_Color color;
  color.r = r;
  color.g = g;
  color.b = b;

  SDL_Surface *resulting_text;
  resulting_text = TTF_RenderText_Blended(font, *text, color);
  if (!resulting_text) {
    return ThrowException(Exception::Error(String::Concat(
      String::New("TTF::RenderTextBlended: "),
      String::New(TTF_GetError())
    )));
  }
  return scope.Close(WrapSurface(resulting_text));
}

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

static Handle<Value> sdl::WM::SetCaption(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsString())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WM::SetCaption(String, String)")));
  }

  String::Utf8Value title(args[0]);
  String::Utf8Value icon(args[0]);

  SDL_WM_SetCaption(*title, *icon);

  return Undefined();
}

static Handle<Value> sdl::WM::SetIcon(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 1 && args[0]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected WM::SetIcon(Surface)")));
  }

  SDL_Surface* icon = UnwrapSurface(args[0]->ToObject());
  int colorkey = SDL_MapRGB(icon->format, 255, 0, 255);
  SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
  SDL_WM_SetIcon(icon, NULL);

  return Undefined();
}

