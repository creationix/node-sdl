#ifndef NODE_SDL_H_
#define NODE_SDL_H_

// Various defines to modify how the bindings operate somewhat when compiled. This can include
// things such as disabling runtime argument checks, including various logging, etc.
//
// NOTE: None of these flags are actually used yet. I am only putting them in here as a note to
// myself as to what features I will want to be optionally disabled in the future. Otherwise I
// will forget :)

// WARNING: The only time you should disable arg checking is after thoroughly testing your application
// and you don't see any exceptions being thrown. You WILL get segfaults if you aren't careful about
// what types you pass to some functions without having them checked. (such as passing an SDL_Color
// when something want an SDL_Texture, or vise versa) And there will be no stacktrace or anything
// pointing to a location where the problem happened.
#define ENABLE_ARG_CHECKING
// If this is not defined, the entire SDL_image wrapper will not be included in the build.
#define ENABLE_IMAGE
// If this is not defined, the entire SDL_ttf wrapper will not be included in the build.
#define ENABLE_TTF

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

  v8::Handle<v8::Value> MapRGB(const v8::Arguments& args);
  v8::Handle<v8::Value> MapRGBA(const v8::Arguments& args);
  v8::Handle<v8::Value> GetRGB(const v8::Arguments& args);
  v8::Handle<v8::Value> GetRGBA(const v8::Arguments& args);

  v8::Handle<v8::Value> AddHintCallback(const v8::Arguments& args);
  // v8::Handle<v8::Value> ClearHints(const v8::Arguments& args);
  v8::Handle<v8::Value> GetHint(const v8::Arguments& args);
  v8::Handle<v8::Value> SetHint(const v8::Arguments& args);
  v8::Handle<v8::Value> SetHintWithPriority(const v8::Arguments& args);

  v8::Handle<v8::Value> CompiledVersion(const v8::Arguments& args);
  v8::Handle<v8::Value> CompiledRevision(const v8::Arguments& args);
  v8::Handle<v8::Value> GetRevision(const v8::Arguments& args);
  v8::Handle<v8::Value> GetRevisionNumber(const v8::Arguments& args);
  v8::Handle<v8::Value> GetVersion(const v8::Arguments& args);

  v8::Handle<v8::Value> GetClipboardText(const v8::Arguments& args);
  v8::Handle<v8::Value> HasClipboardText(const v8::Arguments& args);
  v8::Handle<v8::Value> SetClipboardText(const v8::Arguments& args);

  namespace IMG {
    v8::Handle<v8::Value> Load(const v8::Arguments& args);
  }

  namespace WM {
  }
}

#endif
