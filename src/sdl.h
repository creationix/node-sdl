#ifndef NODE_SDL_H_
#define NODE_SDL_H_

#include <node.h>
#include <v8.h>
#include <map>
#include <string>

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "helpers.h"

using namespace v8;

namespace sdl {

  class WindowWrapper : public node::ObjectWrap {
  public:
    WindowWrapper(std::string title, int x, int y, int w, int h, uint32_t flags);
    ~WindowWrapper();

    static Persistent<FunctionTemplate> window_wrap_template_;

    static void Init(Handle<Object> exports);
    static Handle<Value> New(const Arguments& args);

    static Handle<Value> GetBrightness(const Arguments& args);
    static Handle<Value> GetDisplayIndex(const Arguments& args);
    static Handle<Value> GetDisplayMode(const Arguments& args);
    static Handle<Value> GetFlags(const Arguments& args);
    static Handle<Value> GetGammaRamp(const Arguments& args);
    static Handle<Value> GetGrab(const Arguments& args);
    static Handle<Value> GetWindowID(const Arguments& args);
    static Handle<Value> GetMaximumSize(const Arguments& args);
    static Handle<Value> GetMinimumSize(const Arguments& args);
    static Handle<Value> GetPixelFormat(const Arguments& args);
    static Handle<Value> GetPosition(const Arguments& args);
    static Handle<Value> GetSize(const Arguments& args);
    static Handle<Value> GetSurface(const Arguments& args);
    static Handle<Value> GetTitle(const Arguments& args);
    // TODO: Implement this. Probably won't get to this anytime soon.
    //       Too specific to be useful?
    // static Handle<Value> GetWMInfo(const Arguments& args);

    static Handle<Value> Hide(const Arguments& args);
    static Handle<Value> Show(const Arguments& args);

    static Handle<Value> Maximize(const Arguments& args);
    static Handle<Value> Minimize(const Arguments& args);

    static Handle<Value> Raise(const Arguments& args);
    static Handle<Value> Restore(const Arguments& args);

    static Handle<Value> SetBordered(const Arguments& args);
    static Handle<Value> SetBrightness(const Arguments& args);
    // TODO: Implement this? Because Javascript can just set arbritary data, not needed?
    // static Handle<Value> SetData(const Arguments& args);
    static Handle<Value> SetDisplayMode(const Arguments& args);
    static Handle<Value> SetFullscreen(const Arguments& args);
    static Handle<Value> SetGammaRamp(const Arguments& args);
    static Handle<Value> SetGrab(const Arguments& args);
    static Handle<Value> SetIcon(const Arguments& args);
    static Handle<Value> SetMaximumSize(const Arguments& args);
    static Handle<Value> SetMinimumSize(const Arguments& args);
    static Handle<Value> SetPosition(const Arguments& args);
    static Handle<Value> SetSize(const Arguments& args);
    static Handle<Value> SetTitle(const Arguments& args);

    static Handle<Value> UpdateWindowSurface(const Arguments& args);
    static Handle<Value> UpdateWindowSurfaceRects(const Arguments& args);

  private:
    SDL_Window* window_;
  };

  // Initialization and Shutdown.
  static Handle<Value> Init(const Arguments& args);
  static Handle<Value> InitSubSystem(const Arguments& args);
  static Handle<Value> WasInit(const Arguments& args);
  static Handle<Value> Quit(const Arguments& args);
  static Handle<Value> QuitSubSystem(const Arguments& args);

  // Display and Window Management.
  static Handle<Value> CreateWindow(const Arguments& args);
  static Handle<Value> CreateWindowAndRenderer(const Arguments& args);
  static Handle<Value> CreateWindowFrom(const Arguments& args);
  static Handle<Value> DestroyWindow(const Arguments& args);

  static Handle<Value> DisableScreenSaver(const Arguments& args);
  static Handle<Value> EnableScreenSaver(const Arguments& args);
  static Handle<Value> IsScreenSaverEnabled(const Arguments& args);

  static Handle<Value> GetClosestDisplayMode(const Arguments& args);
  static Handle<Value> GetCurrentDisplayMode(const Arguments& args);
  static Handle<Value> GetCurrentVideoDriver(const Arguments& args);
  static Handle<Value> GetDesktopDisplayMode(const Arguments& args);
  static Handle<Value> GetDisplayBounds(const Arguments& args);
  static Handle<Value> GetDisplayMode(const Arguments& args);
  static Handle<Value> GetDisplayName(const Arguments& args);
  static Handle<Value> GetNumDisplayModes(const Arguments& args);
  static Handle<Value> GetNumVideoDisplays(const Arguments& args);
  static Handle<Value> GetNumVideoDrivers(const Arguments& args);
  static Handle<Value> GetVideoDrivers(const Arguments& args);
  static Handle<Value> GetWindowFromID(const Arguments& args);

  static Handle<Value> ShowMessageBox(const Arguments& args);
  static Handle<Value> ShowSimpleMessageBox(const Arguments& args);

  static Handle<Value> VideoInit(const Arguments& args);
  static Handle<Value> VideoQuit(const Arguments& args);

  // Error handling?
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
