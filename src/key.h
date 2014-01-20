#ifndef NODEJS_KEY_H
#define NODEJS_KEY_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace key {
		void Init(v8::Handle<v8::Object> exports);
	}

	v8::Handle<v8::Value> GetKeyFromName(const v8::Arguments& args);
	v8::Handle<v8::Value> GetKeyFromScancode(const v8::Arguments& args);
	v8::Handle<v8::Value> GetKeyName(const v8::Arguments& args);
	
	v8::Handle<v8::Value> GetScancodeFromKey(const v8::Arguments& args);
	v8::Handle<v8::Value> GetScancodeFromName(const v8::Arguments& args);
	v8::Handle<v8::Value> GetScancodeName(const v8::Arguments& args);

	v8::Handle<v8::Value> HasScreenKeyboardSupport(const v8::Arguments& args);
	v8::Handle<v8::Value> IsScreenKeyboardShown(const v8::Arguments& args);

	v8::Handle<v8::Value> GetKeyboardFocus(const v8::Arguments& args);
	v8::Handle<v8::Value> GetKeyboardState(const v8::Arguments& args);
	v8::Handle<v8::Value> GetModState(const v8::Arguments& args);
	v8::Handle<v8::Value> SetModState(const v8::Arguments& args);

	v8::Handle<v8::Value> IsTextInputActive(const v8::Arguments& args);
	v8::Handle<v8::Value> SetTextInputRect(const v8::Arguments& args);
	v8::Handle<v8::Value> StartTextInput(const v8::Arguments& args);
	v8::Handle<v8::Value> StopTextInput(const v8::Arguments& args);
}

#endif