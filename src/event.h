#ifndef NODEJS_EVENT_H
#define NODEJS_EVENT_H

#include <v8.h>
#include <node.h>
#include "SDL.h"


namespace sdl {
	namespace event {
		void Init(v8::Handle<v8::Object> exports);
	}

	// v8::Handle<v8::Value> AddEventWatch(const v8::Arguments& args);
	// v8::Handle<v8::Value> DelEventWatch(const v8::Arguments& args);
	v8::Handle<v8::Value> EventState(const v8::Arguments& args);

	// v8::Handle<v8::Value> FilterEvents(const v8::Arguments& args);
	// v8::Handle<v8::Value> GetEventFilter(const v8::Arguments& args);
	// v8::Handle<v8::Value> SetEventFilter(const v8::Arguments& args);

	v8::Handle<v8::Value> FlushEvent(const v8::Arguments& args);
	v8::Handle<v8::Value> FlushEvents(const v8::Arguments& args);

	v8::Handle<v8::Value> GetNumTouchDevices(const v8::Arguments& args);
	v8::Handle<v8::Value> GetNumTouchFingers(const v8::Arguments& args);
	v8::Handle<v8::Value> GetTouchDevice(const v8::Arguments& args);
	v8::Handle<v8::Value> GetTouchFinger(const v8::Arguments& args);
	v8::Handle<v8::Value> RecordGesture(const v8::Arguments& args);

	v8::Handle<v8::Value> HasEvent(const v8::Arguments& args);
	v8::Handle<v8::Value> HasEvents(const v8::Arguments& args);

	// v8::Handle<v8::Value> LoadDollarTemplates(const v8::Arguments& args);
	// v8::Handle<v8::Value> SaveAllDollarTemplates(const v8::Arguments& args);
	// v8::Handle<v8::Value> SaveDollarTemplate(const v8::Arguments& args);

	v8::Handle<v8::Value> WaitEvent(const v8::Arguments& args);
	v8::Handle<v8::Value> WaitEventTimeout(const v8::Arguments& args);
	v8::Handle<v8::Value> PollEvent(const v8::Arguments& args);
	// v8::Handle<v8::Value> PeepEvents(const v8::Arguments& args);
	// v8::Handle<v8::Value> PumpEvents(const v8::Arguments& args);
	// v8::Handle<v8::Value> PushEvent(const v8::Arguments& args);
	// v8::Handle<v8::Value> RegisterEvents(const v8::Arguments& args);

	v8::Handle<v8::Value> QuitRequested(const v8::Arguments& args);

	v8::Local<v8::Object> SDLEventToJavascriptObject(const SDL_Event& event);
}

#endif