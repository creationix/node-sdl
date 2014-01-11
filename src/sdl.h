#ifndef NODE_SDL_H_
#define NODE_SDL_H_

#include <node.h>
#include <v8.h>
#include <map>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "helpers.h"

using namespace v8;

namespace sdl {

  class ColorWrapper : public node::ObjectWrap {
  public:
    ColorWrapper();
    ~ColorWrapper();

    static Persistent<FunctionTemplate> color_wrap_template_;

    static void Init(Handle<Object> exports);
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> GetRed(const Arguments& args);
    static Handle<Value> GetGreen(const Arguments& args);
    static Handle<Value> GetBlue(const Arguments& args);
    static Handle<Value> GetAlpha(const Arguments& args);

  private:
    SDL_Color color_;
  };

  class EventWrapper : public node::ObjectWrap {
  public:
    EventWrapper();
    ~EventWrapper();

    static Persistent<FunctionTemplate> event_wrap_template_;
    
    static std::map<uint32_t, std::string> event_type_to_string_;
    static std::map<SDL_WindowEventID, std::string> window_event_to_string_;

    static void Init(Handle<Object> exports);
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> GetType(const Arguments& args);
    static Handle<Value> GetSpecificType(const Arguments& args);

  private:
    SDL_Event event_;
  };

  static Handle<Value> Init(const Arguments& args);
  static Handle<Value> InitSubSystem(const Arguments& args);
  static Handle<Value> WasInit(const Arguments& args);

  static Handle<Value> Quit(const Arguments& args);
  static Handle<Value> QuitSubSystem(const Arguments& args);

  static Handle<Value> ClearError(const Arguments& args);
  static Handle<Value> GetError(const Arguments& args);
  static Handle<Value> SetError(const Arguments& args);

  static Handle<Value> WaitEvent(const Arguments& args);
  static Handle<Value> WaitEventTimeout(const Arguments& args);
  static Handle<Value> PollEvent(const Arguments& args);

  static Handle<Value> NumJoysticks(const Arguments& args);
  static Handle<Value> JoystickOpen(const Arguments& args);
  static Handle<Value> JoystickName(const Arguments& args);
  static Handle<Value> JoystickNumAxes(const Arguments& args);
  static Handle<Value> JoystickNumButtons(const Arguments& args);
  static Handle<Value> JoystickNumBalls(const Arguments& args);
  static Handle<Value> JoystickNumHats(const Arguments& args);
  static Handle<Value> JoystickClose(const Arguments& args);
  static Handle<Value> JoystickUpdate(const Arguments& args);
  static Handle<Value> JoystickEventState(const Arguments& args);

  static Handle<Value> FillRect(const Arguments& args);
  static Handle<Value> CreateRGBSurface(const Arguments& args);
  static Handle<Value> BlitSurface(const Arguments& args);
  static Handle<Value> FreeSurface(const Arguments& args);
  static Handle<Value> SetColorKey(const Arguments& args);

  static Handle<Value> SetAlpha(const Arguments& args);
  static Handle<Value> MapRGB(const Arguments& args);
  static Handle<Value> MapRGBA(const Arguments& args);
  static Handle<Value> GetRGB(const Arguments& args);
  static Handle<Value> GetRGBA(const Arguments& args);
  static Handle<Value> SetClipRect(const Arguments& args);

  namespace TTF {
    static Handle<Value> Init(const Arguments& args);
    static Handle<Value> OpenFont(const Arguments& args);
    // static Handle<Value> RenderTextBlended(const Arguments& args);
  }

  namespace IMG {
    static Handle<Value> Load(const Arguments& args);
  }

  namespace WM {
  }

  namespace GL {
    static Handle<Value> SetAttribute (const Arguments& args);
    static Handle<Value> GetAttribute (const Arguments& args);
  }

}

#endif
