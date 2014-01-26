#ifndef NODESDL_TEXTURE_H
#define NODESDL_TEXTURE_H

#include <node.h>
#include <v8.h>

#include "SDL.h"


namespace sdl {
	class TextureWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> texture_wrap_template_;

		TextureWrapper();
		TextureWrapper(v8::Handle<v8::Object> toWrap);
		~TextureWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> Update(const v8::Arguments& args);

		SDL_Texture* texture_;
	};
}

#endif