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
#include "render.h"

namespace sdl {
  // Initialization and Shutdown.
  v8::Handle<v8::Value> Init(const v8::Arguments& args);
  v8::Handle<v8::Value> InitSubSystem(const v8::Arguments& args);
  v8::Handle<v8::Value> WasInit(const v8::Arguments& args);
  v8::Handle<v8::Value> Quit(const v8::Arguments& args);
  v8::Handle<v8::Value> QuitSubSystem(const v8::Arguments& args);

  // Display and Window Management.
  v8::Handle<v8::Value> DisableScreenSaver(const v8::Arguments& args);
  v8::Handle<v8::Value> EnableScreenSaver(const v8::Arguments& args);
  v8::Handle<v8::Value> IsScreenSaverEnabled(const v8::Arguments& args);

  v8::Handle<v8::Value> GetClosestDisplayMode(const v8::Arguments& args);
  v8::Handle<v8::Value> GetCurrentDisplayMode(const v8::Arguments& args);
  v8::Handle<v8::Value> GetCurrentVideoDriver(const v8::Arguments& args);
  v8::Handle<v8::Value> GetDesktopDisplayMode(const v8::Arguments& args);
  v8::Handle<v8::Value> GetDisplayBounds(const v8::Arguments& args);
  v8::Handle<v8::Value> GetDisplayMode(const v8::Arguments& args);
  v8::Handle<v8::Value> GetDisplayName(const v8::Arguments& args);
  v8::Handle<v8::Value> GetNumDisplayModes(const v8::Arguments& args);
  v8::Handle<v8::Value> GetNumVideoDisplays(const v8::Arguments& args);
  v8::Handle<v8::Value> GetNumVideoDrivers(const v8::Arguments& args);
  v8::Handle<v8::Value> GetVideoDriver(const v8::Arguments& args);
  // TODO: Put this functionality into Window?
  // v8::Handle<v8::Value> GetWindowFromID(const v8::Arguments& args);

  // TODO: Implement showing a normal message box.
  // v8::Handle<v8::Value> ShowMessageBox(const v8::Arguments& args);
  v8::Handle<v8::Value> ShowSimpleMessageBox(const v8::Arguments& args);

  v8::Handle<v8::Value> VideoInit(const v8::Arguments& args);
  v8::Handle<v8::Value> VideoQuit(const v8::Arguments& args);

  // Error handling?
  v8::Handle<v8::Value> ClearError(const v8::Arguments& args);
  v8::Handle<v8::Value> GetError(const v8::Arguments& args);
  v8::Handle<v8::Value> SetError(const v8::Arguments& args);

  v8::Handle<v8::Value> WaitEvent(const v8::Arguments& args);
  v8::Handle<v8::Value> WaitEventTimeout(const v8::Arguments& args);
  v8::Handle<v8::Value> PollEvent(const v8::Arguments& args);

  v8::Handle<v8::Value> NumJoysticks(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickOpen(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickName(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickNumAxes(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickNumButtons(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickNumBalls(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickNumHats(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickClose(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickUpdate(const v8::Arguments& args);
  v8::Handle<v8::Value> JoystickEventState(const v8::Arguments& args);

  v8::Handle<v8::Value> FillRect(const v8::Arguments& args);
  v8::Handle<v8::Value> CreateRGBSurface(const v8::Arguments& args);
  v8::Handle<v8::Value> BlitSurface(const v8::Arguments& args);
  v8::Handle<v8::Value> FreeSurface(const v8::Arguments& args);
  v8::Handle<v8::Value> SetColorKey(const v8::Arguments& args);

  v8::Handle<v8::Value> MapRGB(const v8::Arguments& args);
  v8::Handle<v8::Value> MapRGBA(const v8::Arguments& args);
  v8::Handle<v8::Value> GetRGB(const v8::Arguments& args);
  v8::Handle<v8::Value> GetRGBA(const v8::Arguments& args);
  v8::Handle<v8::Value> SetClipRect(const v8::Arguments& args);

  namespace TTF {
    v8::Handle<v8::Value> Init(const v8::Arguments& args);
    v8::Handle<v8::Value> OpenFont(const v8::Arguments& args);
    // static v8::Handle<v8::Value> RenderTextBlended(const v8::Arguments& args);
  }

  namespace IMG {
    v8::Handle<v8::Value> Load(const v8::Arguments& args);
  }

  namespace WM {
  }

  namespace GL {
    v8::Handle<v8::Value> SetAttribute (const v8::Arguments& args);
    v8::Handle<v8::Value> GetAttribute (const v8::Arguments& args);
  }

}

#endif
