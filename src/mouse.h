#ifndef NODEJS_MOUSE_H
#define NODEJS_MOUSE_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace mouse {
		void Init(v8::Handle<v8::Object> exports);
	}

	class CursorWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;
		static v8::Persistent<v8::FunctionTemplate> wrap_template_system_;

		CursorWrapper();
		CursorWrapper(v8::Handle<v8::Object> toWrap);
		~CursorWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> NewSystem(const v8::Arguments& args);

		static v8::Handle<v8::Value> FreeCursor(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetCursor(const v8::Arguments& args);

		SDL_Cursor* cursor_;
	};

	// v8::Handle<v8::Value> CreateCursor(const v8::Arguments& args);
	// v8::Handle<v8::Value> CreateSystemCursor(const v8::Arguments& args);

	v8::Handle<v8::Value> ShowCursor(const v8::Arguments& args);
	v8::Handle<v8::Value> GetCursor(const v8::Arguments& args);
	v8::Handle<v8::Value> GetDefaultCursor(const v8::Arguments& args);
	v8::Handle<v8::Value> GetMouseFocus(const v8::Arguments& args);
	v8::Handle<v8::Value> GetMouseState(const v8::Arguments& args);
	v8::Handle<v8::Value> GetRelativeMouseMode(const v8::Arguments& args);
	v8::Handle<v8::Value> GetRelativeMouseState(const v8::Arguments& args);

	v8::Handle<v8::Value> SetRelativeMouseMode(const v8::Arguments& args);

	v8::Handle<v8::Value> WarpMouseInWindow(const v8::Arguments& args);

	v8::Handle<v8::Value> ButtonMacroWrapper(const v8::Arguments& args);
}

#endif