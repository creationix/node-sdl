#ifndef NODESDL_SDLGL_H
#define NODESDL_SDLGL_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace gl {
		void Init(v8::Handle<v8::Object> exports);

		class ContextWrapper : node::ObjectWrap {
		public:
			ContextWrapper();
			ContextWrapper(v8::Handle<v8::Object> obj);
			~ContextWrapper();

			static v8::Persistent<v8::FunctionTemplate> wrap_template_;

			static void Init(v8::Handle<v8::Object> exports);
			static v8::Handle<v8::Value> New(const v8::Arguments& args);

			SDL_GLContext context_;
		};

		v8::Handle<v8::Value> BindTexture(const v8::Arguments& args);
		v8::Handle<v8::Value> UnbindTexture(const v8::Arguments& args);

		v8::Handle<v8::Value> ExtensionSupported(const v8::Arguments& args);

		v8::Handle<v8::Value> LoadLibrary(const v8::Arguments& args);
		v8::Handle<v8::Value> UnloadLibrary(const v8::Arguments& args);

		v8::Handle<v8::Value> SetAttribute(const v8::Arguments& args);
		v8::Handle<v8::Value> MakeCurrent(const v8::Arguments& args);
		v8::Handle<v8::Value> SetSwapInterval(const v8::Arguments& args);

		v8::Handle<v8::Value> GetAttribute(const v8::Arguments& args);
		v8::Handle<v8::Value> GetCurrentContext(const v8::Arguments& args);
		v8::Handle<v8::Value> GetCurrentWindow(const v8::Arguments& args);
		v8::Handle<v8::Value> GetDrawableSize(const v8::Arguments& args);
		v8::Handle<v8::Value> GetSwapInterval(const v8::Arguments& args);
	}
}

#endif