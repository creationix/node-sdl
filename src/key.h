#ifndef NODEJS_KEY_H
#define NODEJS_KEY_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace key {
		void Init(v8::Handle<v8::Object> exports);
	}
}

#endif