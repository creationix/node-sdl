#include <v8.h>
#include <node.h>
#include <SDL.h>

using namespace v8;
using namespace node;

namespace node_sdl {


////////////////////////////////////////////////////////////////////////////////
//                                 INIT                                       //
////////////////////////////////////////////////////////////////////////////////

  static Handle<Value> Init(const Arguments& args) {
    HandleScope scope;
    int flags = (args[0]->Int32Value());
    if (SDL_Init(flags) < 0) {
      return ThrowException(Exception::Error(String::Concat(
        String::New("Couldn't initialize SDL: "),
        String::New(SDL_GetError())
      )));
    }
    return Undefined();
  }

  static Handle<Value> Quit(const Arguments& args) {
    HandleScope scope;
    SDL_Quit();
    return Undefined();
  }

  static Handle<Value> QuitSubSystem(const Arguments& args) {
    HandleScope scope;
    int flags = (args[0]->Int32Value());
    SDL_QuitSubSystem(flags);
    return Undefined();
  }

  static Handle<Value> GetError(const Arguments& args) {
    HandleScope scope;
    return String::New(SDL_GetError());
  }

////////////////////////////////////////////////////////////////////////////////
//                                 VIDEO                                      //
////////////////////////////////////////////////////////////////////////////////

  static SDL_Surface* screen;

  // We changed the API here for ease of binding and use
  // Normally it would return the screen.
  static Handle<Value> SetVideoMode(const Arguments& args) {
    HandleScope scope;
    int width = (args[0]->Int32Value());
    int height = (args[1]->Int32Value());
    int bpp = (args[2]->Int32Value());
    int flags = (args[3]->Int32Value());
    screen = SDL_SetVideoMode(width, height, bpp, flags);
    if (screen == NULL) {
      return Number::New(-1);
    }
    return Number::New(0);
  }

  static Handle<Value> VideoModeOK(const Arguments& args) {
    HandleScope scope;
    int width = (args[0]->Int32Value());
    int height = (args[1]->Int32Value());
    int bpp = (args[2]->Int32Value());
    int flags = (args[3]->Int32Value());
    return Number::New(SDL_VideoModeOK(width, height, bpp, flags));
  }

  static Handle<Value> GetScreenWidth(const Arguments& args) {
    HandleScope scope;
    return Number::New(screen->w);
  }

  static Handle<Value> GetScreenHeight(const Arguments& args) {
    HandleScope scope;
    return Number::New(screen->h);
  }


  static Handle<Value> MapRGB(const Arguments& args) {
    HandleScope scope;
    Uint8 r = (args[0]->Int32Value());
    Uint8 g = (args[1]->Int32Value());
    Uint8 b = (args[2]->Int32Value());
    return Number::New(SDL_MapRGB(screen->format, r, g, b));
  }

  static Handle<Value> MapRGBA(const Arguments& args) {
    HandleScope scope;
    Uint8 r = (args[0]->Int32Value());
    Uint8 g = (args[1]->Int32Value());
    Uint8 b = (args[2]->Int32Value());
    Uint8 a = (args[3]->Int32Value());
    return Number::New(SDL_MapRGBA(screen->format, r, g, b, a));
  }

  static Handle<Value> LockSurface(const Arguments& args) {
    HandleScope scope;
    if (SDL_MUSTLOCK(screen)) {
      if (SDL_LockSurface(screen) < 0) {
        return ThrowException(Exception::Error(String::Concat(
          String::New("Can't lock screen: "),
          String::New(SDL_GetError())
        )));
      }
    }
    return Undefined();
  }

  static Handle<Value> UnlockSurface(const Arguments& args) {
    HandleScope scope;
    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }
    return Undefined();
  }

  static Handle<Value> MUSTLOCK(const Arguments& args) {
    HandleScope scope;
    return Boolean::New(SDL_MUSTLOCK(screen));
  }

  /*
   * Set the pixel at (x, y) to the given value
   * NOTE: The surface must be locked before calling this!
   */
  static Handle<Value> PutPixel(const Arguments& args) {
    HandleScope scope;

    int x = (args[0]->Int32Value());
    int y = (args[1]->Int32Value());
    ::Uint32 pixel = (args[2]->Int32Value());

    int bpp = screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(::Uint32 *)p = pixel;
        break;
    }

    return Undefined();
  }

  /*
   * Return the pixel value at (x, y)
   * NOTE: The surface must be locked before calling this!
   */
  static Handle<Value> GetPixel(const Arguments& args) {
    HandleScope scope;
    int x = (args[0]->Int32Value());
    int y = (args[1]->Int32Value());

      int bpp = screen->format->BytesPerPixel;
      /* Here p is the address to the pixel we want to retrieve */
      Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

      switch(bpp) {
      case 1:
          return Number::New(*p);

      case 2:
          return Number::New(*(Uint16 *)p);

      case 3:
          if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
              return Number::New(p[0] << 16 | p[1] << 8 | p[2]);
          else
              return Number::New(p[0] | p[1] << 8 | p[2] << 16);

      case 4:
          return Number::New(*(::Uint32 *)p);

      default:
          return Number::New(0);       /* shouldn't happen, but avoids warnings */
      }
  }

  static Handle<Value> UpdateRect(const Arguments& args) {
    HandleScope scope;
    int x = (args[0]->Int32Value());
    int y = (args[1]->Int32Value());
    int w = (args[2]->Int32Value());
    int h = (args[3]->Int32Value());
    SDL_UpdateRect(screen, x, y, w, h);
    return Undefined();
  }

  static Handle<Value> Flip(const Arguments& args) {
    HandleScope scope;
    SDL_Flip (screen);
    return Undefined();
  }

  static Handle<Value> FillRect(const Arguments& args) {
    HandleScope scope;
    SDL_Rect rect;
    rect.x = (args[0]->Int32Value());
    rect.y = (args[1]->Int32Value());
    rect.w = (args[2]->Int32Value());
    rect.h = (args[3]->Int32Value());
    ::Uint32 color = (args[4]->Int32Value());
    if (SDL_FillRect (screen, &rect, color) < 0) {
      return ThrowException(Exception::Error(String::Concat(
        String::New("Couldn't draw rectangle: "),
        String::New(SDL_GetError())
      )));
    }
    return Undefined();
  }

  static Handle<Value> Fill(const Arguments& args) {
    HandleScope scope;
    ::Uint32 color = (args[4]->Int32Value());
    if (SDL_FillRect (screen, NULL, color) < 0) {
      return ThrowException(Exception::Error(String::Concat(
        String::New("Couldn't fill screen: "),
        String::New(SDL_GetError())
      )));
    }
    return Undefined();
  }


//  static Handle<Value> Clear(const Arguments& args) {
//    HandleScope scope;
//    SDL_FillRect (screen, NULL, 0);
//    return Undefined();
//  }



////////////////////////////////////////////////////////////////////////////////
//                                JOYSTICK                                    //
////////////////////////////////////////////////////////////////////////////////

  static SDL_Joystick *joy;

  static Handle<Value> NumJoysticks(const Arguments& args) {
    HandleScope scope;
    return Number::New(SDL_NumJoysticks());
  }

  static Handle<Value> JoystickOpen(const Arguments& args) {
    HandleScope scope;
    int i = (args[0]->Int32Value());
    joy = SDL_JoystickOpen(i);
    if (!joy) {
      return ThrowException(Exception::Error(String::Concat(
        String::New("Couldn't open joystick "),
        Number::New(i)->ToString()
      )));
    }
    return Undefined();
  }

  static Handle<Value> JoystickOpened(const Arguments& args) {
    HandleScope scope;
    int i = (args[0]->Int32Value());
    return Number::New(SDL_JoystickOpened(i));
  }

  static Handle<Value> JoystickName(const Arguments& args) {
    HandleScope scope;
    int i = (args[0]->Int32Value());
    return String::New(SDL_JoystickName(i));
  }

  static Handle<Value> JoystickNumAxes(const Arguments& args) {
    HandleScope scope;
    return Number::New(SDL_JoystickNumAxes(joy));
  }

  static Handle<Value> JoystickNumButtons(const Arguments& args) {
    HandleScope scope;
    return Number::New(SDL_JoystickNumButtons(joy));
  }

  static Handle<Value> JoystickNumBalls(const Arguments& args) {
    HandleScope scope;
    return Number::New(SDL_JoystickNumBalls(joy));
  }

  static Handle<Value> JoystickNumHats(const Arguments& args) {
    HandleScope scope;
    return Number::New(SDL_JoystickNumHats(joy));
  }

  static Handle<Value> JoystickGetAxis(const Arguments& args) {
    HandleScope scope;
    int axis = (args[0]->Int32Value());
    return Number::New(SDL_JoystickGetAxis(joy, axis));
  }

  static Handle<Value> JoystickGetButton(const Arguments& args) {
    HandleScope scope;
    int button = (args[0]->Int32Value());
    return Number::New(SDL_JoystickGetButton(joy, button));
  }

  static Handle<Value> JoystickGetHat(const Arguments& args) {
    HandleScope scope;
    int hat = (args[0]->Int32Value());
    return Number::New(SDL_JoystickGetHat(joy, hat));
  }

  static Handle<Value> JoystickUpdate(const Arguments& args) {
    HandleScope scope;
    SDL_JoystickUpdate();
    return Undefined();
  }

  static Handle<Value> JoystickEventState(const Arguments& args) {
    HandleScope scope;

    int state = (args[0]->Int32Value());
    return Number::New(SDL_JoystickEventState(state));
  }

////////////////////////////////////////////////////////////////////////////////
//                                APP STATE                                   //
////////////////////////////////////////////////////////////////////////////////

  static Handle<Value> GetAppState(const Arguments& args) {
    HandleScope scope;
    int appState = SDL_GetAppState();
    Local<Object> result = Object::New();
    result->Set(String::New("mouseFocus"), Boolean::New(appState & SDL_APPMOUSEFOCUS));
    result->Set(String::New("inputFocus"), Boolean::New(appState & SDL_APPINPUTFOCUS));
    result->Set(String::New("active"), Boolean::New(appState & SDL_APPACTIVE));
    return scope.Close(result);
  }

// MOUSE

  static Handle<Value> GetMouseState(const Arguments& args) {
    HandleScope scope;
    int x;
    int y;
    Uint8 buttonState = SDL_GetMouseState(&x, &y);
    Local<Object> result = Object::New();
    result->Set(String::New("x"), Number::New(x));
    result->Set(String::New("y"), Number::New(y));
    result->Set(String::New("buttonState"), Number::New(buttonState));
    return scope.Close(result);
  }


////////////////////////////////////////////////////////////////////////////////
//                                 EVENTS                                     //
////////////////////////////////////////////////////////////////////////////////

  static int EIO_WaitEvent(eio_req *req) {
    SDL_WaitEvent(NULL);
    return 0;
  }

  typedef struct { Persistent<Function> fn; } callback_t;

  static int EIO_OnEvent(eio_req *req) {
    HandleScope scope;

    callback_t *callback = (callback_t *) req->data;
    ev_unref(EV_DEFAULT_UC);

    SDL_Event event;
    SDL_PollEvent(&event);

    Handle<Value> argv[1];

    Local<Object> data = Object::New();


    switch (event.type) {
      case SDL_ACTIVEEVENT:
        data->Set(String::New("type"), String::New("ACTIVE"));
        if (event.active.state & SDL_APPMOUSEFOCUS)
          data->Set(String::New("MOUSEFOCUS"), Boolean::New(event.active.gain));
        if (event.active.state & SDL_APPINPUTFOCUS)
          data->Set(String::New("INPUTFOCUS"), Boolean::New(event.active.gain));
        if (event.active.state & SDL_APPACTIVE)
          data->Set(String::New("ACTIVE"), Boolean::New(event.active.gain));
        break;

      case SDL_KEYDOWN:
      case SDL_KEYUP:
        data->Set(String::New("type"), String::New(event.type == SDL_KEYDOWN ? "KEYDOWN" : "KEYUP"));
        data->Set(String::New("scancode"), Number::New(event.key.keysym.scancode));
        data->Set(String::New("sym"), Number::New(event.key.keysym.sym));
        data->Set(String::New("mod"), Number::New(event.key.keysym.mod));
        break;
      default:
        data->Set(String::New("typeCode"), Number::New(event.type));
        break;
    }

    argv[0] = scope.Close(data);

    callback->fn->Call(Context::GetCurrent()->Global(), 1, argv);

    callback->fn.Dispose();
    free(callback);
    return 0;
  }

  static Handle<Value> WaitEvent(const Arguments& args) {
    HandleScope scope;

    callback_t *callback = (callback_t*) malloc(sizeof(callback_t));
    callback->fn = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
    eio_custom(EIO_WaitEvent, EIO_PRI_DEFAULT, EIO_OnEvent, callback);
    ev_ref(EV_DEFAULT_UC);
    return Undefined();
  }

////////////////////////////////////////////////////////////////////////////////

}

extern "C" void
init(Handle<Object> target)
{
  HandleScope scope;

  // INIT
  target->Set(String::New("INIT_TIMER"), Number::New(SDL_INIT_TIMER));
  target->Set(String::New("INIT_AUDIO"), Number::New(SDL_INIT_AUDIO));
  target->Set(String::New("INIT_VIDEO"), Number::New(SDL_INIT_VIDEO));
  target->Set(String::New("INIT_CDROM"), Number::New(SDL_INIT_CDROM));
  target->Set(String::New("INIT_JOYSTICK"), Number::New(SDL_INIT_JOYSTICK));
  target->Set(String::New("INIT_EVERYTHING"), Number::New(SDL_INIT_EVERYTHING));
  target->Set(String::New("INIT_NOPARACHUTE"), Number::New(SDL_INIT_NOPARACHUTE));
  target->Set(String::New("INIT_EVENTTHREAD"), Number::New(SDL_INIT_EVENTTHREAD));
  NODE_SET_METHOD(target, "init", node_sdl::Init);
  NODE_SET_METHOD(target, "quit", node_sdl::Quit);
  NODE_SET_METHOD(target, "quitSubSystem", node_sdl::QuitSubSystem);
  NODE_SET_METHOD(target, "getError", node_sdl::GetError);

  // VIDEO
  target->Set(String::New("ASYNCBLIT"), Number::New(SDL_ASYNCBLIT));
  target->Set(String::New("SWSURFACE"), Number::New(SDL_SWSURFACE));
  target->Set(String::New("HWSURFACE"), Number::New(SDL_HWSURFACE));
  target->Set(String::New("ANYFORMAT"), Number::New(SDL_ANYFORMAT));
  target->Set(String::New("FULLSCREEN"), Number::New(SDL_FULLSCREEN));
  target->Set(String::New("DOUBLEBUF"), Number::New(SDL_DOUBLEBUF));
  NODE_SET_METHOD(target, "setVideoMode", node_sdl::SetVideoMode);
  NODE_SET_METHOD(target, "videoModeOK", node_sdl::VideoModeOK);
  NODE_SET_METHOD(target, "getScreenWidth", node_sdl::GetScreenWidth);
  NODE_SET_METHOD(target, "getScreenHeight", node_sdl::GetScreenHeight);
  NODE_SET_METHOD(target, "mapRGB", node_sdl::MapRGB);
  NODE_SET_METHOD(target, "mapRGBA", node_sdl::MapRGBA);
  NODE_SET_METHOD(target, "lockSurface", node_sdl::LockSurface);
  NODE_SET_METHOD(target, "unlockSurface", node_sdl::UnlockSurface);
  NODE_SET_METHOD(target, "MUSTLOCK", node_sdl::MUSTLOCK);
  NODE_SET_METHOD(target, "putPixel", node_sdl::PutPixel);
  NODE_SET_METHOD(target, "getPixel", node_sdl::GetPixel);
  NODE_SET_METHOD(target, "updateRect", node_sdl::UpdateRect);
  NODE_SET_METHOD(target, "flip", node_sdl::Flip);
  NODE_SET_METHOD(target, "fillRect", node_sdl::FillRect);
  NODE_SET_METHOD(target, "fill", node_sdl::Fill);

  // JOYSTICK
  target->Set(String::New("QUERY"), Number::New(SDL_QUERY));
  target->Set(String::New("ENABLE"), Number::New(SDL_ENABLE));
  target->Set(String::New("IGNORE"), Number::New(SDL_IGNORE));
  NODE_SET_METHOD(target, "numJoysticks", node_sdl::NumJoysticks);
  NODE_SET_METHOD(target, "joystickOpen", node_sdl::JoystickOpen);
  NODE_SET_METHOD(target, "joystickOpened", node_sdl::JoystickOpened);
  NODE_SET_METHOD(target, "joystickName", node_sdl::JoystickName);
  NODE_SET_METHOD(target, "joystickNumAxes", node_sdl::JoystickNumAxes);
  NODE_SET_METHOD(target, "joystickNumButtons", node_sdl::JoystickNumButtons);
  NODE_SET_METHOD(target, "joystickNumBalls", node_sdl::JoystickNumBalls);
  NODE_SET_METHOD(target, "joystickNumHats", node_sdl::JoystickNumHats);
  NODE_SET_METHOD(target, "joystickGetAxis", node_sdl::JoystickGetAxis);
  NODE_SET_METHOD(target, "joystickGetButton", node_sdl::JoystickGetButton);
  NODE_SET_METHOD(target, "joystickGetHat", node_sdl::JoystickGetHat);
  NODE_SET_METHOD(target, "joystickUpdate", node_sdl::JoystickUpdate);
  NODE_SET_METHOD(target, "joystickEventState", node_sdl::JoystickEventState);

  // EVENTS
  target->Set(String::New("ACTIVEEVENT"), Number::New(SDL_ACTIVEEVENT));
  target->Set(String::New("KEYDOWN"), Number::New(SDL_KEYDOWN));
  target->Set(String::New("KEYUP"), Number::New(SDL_KEYUP));
  target->Set(String::New("MOUSEMOTION"), Number::New(SDL_MOUSEMOTION));
  target->Set(String::New("MOUSEBUTTONDOWN"), Number::New(SDL_MOUSEBUTTONDOWN));
  target->Set(String::New("MOUSEBUTTONUP"), Number::New(SDL_MOUSEBUTTONUP));
  target->Set(String::New("JOYAXISMOTION"), Number::New(SDL_JOYAXISMOTION));
  target->Set(String::New("JOYBALLMOTION"), Number::New(SDL_JOYBALLMOTION));
  target->Set(String::New("SDL_JOYHATMOTION"), Number::New(SDL_JOYHATMOTION));
  target->Set(String::New("JOYBUTTONDOWN"), Number::New(SDL_JOYBUTTONDOWN));
  target->Set(String::New("JOYBUTTONUP"), Number::New(SDL_JOYBUTTONUP));
  target->Set(String::New("VIDEORESIZE"), Number::New(SDL_VIDEORESIZE));
  target->Set(String::New("VIDEOEXPOSE"), Number::New(SDL_VIDEOEXPOSE));
  target->Set(String::New("QUIT"), Number::New(SDL_QUIT));
  target->Set(String::New("USEREVENT"), Number::New(SDL_USEREVENT));
  target->Set(String::New("NUMEVENTS"), Number::New(SDL_NUMEVENTS));
  target->Set(String::New("SYSWMEVENT"), Number::New(SDL_SYSWMEVENT));

  target->Set(String::New("PRESSED"), Number::New(SDL_PRESSED));
  target->Set(String::New("RELEASED"), Number::New(SDL_RELEASED));

  target->Set(String::New("QUERY"), Number::New(SDL_QUERY));
  target->Set(String::New("ENABLE"), Number::New(SDL_ENABLE));
  target->Set(String::New("IGNORE"), Number::New(SDL_IGNORE));
  NODE_SET_METHOD(target, "waitEvent", node_sdl::WaitEvent);
  NODE_SET_METHOD(target, "getAppState", node_sdl::GetAppState);
  NODE_SET_METHOD(target, "getMouseState", node_sdl::GetMouseState);

}




