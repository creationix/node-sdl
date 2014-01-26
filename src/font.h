#ifndef NODEJS_FONT_H
#define NODEJS_FONT_H

#include <v8.h>
#include <node.h>

#include "SDL.h"
#include "SDL_ttf.h"


namespace sdl {
	namespace TTF {
		class FontWrapper : public node::ObjectWrap {
		public:
			static v8::Persistent<v8::FunctionTemplate> wrap_template_;

			FontWrapper();
			~FontWrapper();

			static void Init(v8::Handle<v8::Object> exports);
			static v8::Handle<v8::Value> New(const v8::Arguments& args);

			static v8::Handle<v8::Value> RenderTextSolid(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderUTF8Solid(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderUnicodeSolid(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderGlyphSolid(const v8::Arguments& args);

			static v8::Handle<v8::Value> RenderTextShaded(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderUTF8Shaded(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderUnicodeShaded(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderGlyphShaded(const v8::Arguments& args);

			static v8::Handle<v8::Value> RenderTextBlended(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderUTF8Blended(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderUnicodeBlended(const v8::Arguments& args);
			static v8::Handle<v8::Value> RenderGlyphBlended(const v8::Arguments& args);

			TTF_Font* font_;
		};

		void Initialize(v8::Handle<v8::Object> exports);

		v8::Handle<v8::Value> Init(const v8::Arguments& args);
		v8::Handle<v8::Value> WasInit(const v8::Arguments& args);
		v8::Handle<v8::Value> Quit(const v8::Arguments& args);

		// v8::Handle<v8::Value> SetError(const v8::Arguments& args);
		v8::Handle<v8::Value> GetError(const v8::Arguments& args);

		// v8::Handle<v8::Value> OpenFont(const v8::Arguments& args);
		// v8::Handle<v8::Value> OpenFontIndex(const v8::Arguments& args);
		// v8::Handle<v8::Value> CloseFont(const v8::Arguments& args);
	}
}

#endif