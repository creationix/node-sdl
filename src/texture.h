#ifndef NODESDL_TEXTURE_H
#define NODESDL_TEXTURE_H

#include <node.h>
#include <v8.h>

#include "SDL.h"


namespace sdl {
	class TextureWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> texture_wrap_template_;
		static v8::Persistent<v8::FunctionTemplate> texture_wrap_template_from_surface_;

		TextureWrapper();
		TextureWrapper(v8::Handle<v8::Object> toWrap);
		~TextureWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> NewFromSurface(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetAlphaMod(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetBlendMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetColorMod(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetFormat(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetWidth(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetHeight(const v8::Arguments& args);

		static v8::Handle<v8::Value> SetAlphaMod(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetBlendMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetColorMod(const v8::Arguments& args);

		// TODO: Implement for streaming to a texture.
		// static v8::Handle<v8::Value> Lock(const v8::Arguments& args);
		// static v8::Handle<v8::Value> Unlock(const v8::Arguments& args);

		static v8::Handle<v8::Value> Update(const v8::Arguments& args);

		SDL_Texture* texture_;
	};
}

#endif