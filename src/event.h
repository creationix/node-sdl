#ifndef NODEJS_EVENT_H
#define NODEJS_EVENT_H

#include <v8.h>
#include <node.h>
#include "SDL.h"


namespace sdl {
	v8::Local<v8::Object> SDLEventToJavascriptObject(const SDL_Event& event);
}

#endif