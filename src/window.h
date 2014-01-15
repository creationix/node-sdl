#ifndef NODESDL_WINDOW_H
#define NODESDL_WINDOW_H

#include <node.h>
#include <v8.h>

#include "SDL.h"

namespace sdl {
	class WindowWrapper : public node::ObjectWrap {
	public:
		WindowWrapper();
		~WindowWrapper();

		static v8::Persistent<v8::FunctionTemplate> window_wrap_template_;

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetBrightness(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetDisplayIndex(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetDisplayMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetFlags(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetGammaRamp(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetGrab(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetWindowID(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetMaximumSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetMinimumSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetPixelFormat(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetPosition(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetSurface(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetTitle(const v8::Arguments& args);
	    // TODO: Implement this. Probably won't get to this anytime soon.
	    //       Too specific to be useful?
	    // static v8::Handle<v8::Value> GetWMInfo(const v8::Arguments& args);

		static v8::Handle<v8::Value> Hide(const v8::Arguments& args);
		static v8::Handle<v8::Value> Show(const v8::Arguments& args);

		static v8::Handle<v8::Value> Maximize(const v8::Arguments& args);
		static v8::Handle<v8::Value> Minimize(const v8::Arguments& args);

		static v8::Handle<v8::Value> Raise(const v8::Arguments& args);
		static v8::Handle<v8::Value> Restore(const v8::Arguments& args);

		static v8::Handle<v8::Value> SetBordered(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetBrightness(const v8::Arguments& args);
	    // TODO: Implement this? Because Javascript can just set arbritary data, not needed?
	    // static v8::Handle<v8::Value> SetData(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetDisplayMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetFullscreen(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetGammaRamp(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetGrab(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetIcon(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetMaximumSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetMinimumSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetPosition(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetTitle(const v8::Arguments& args);

		static v8::Handle<v8::Value> UpdateWindowSurface(const v8::Arguments& args);
		static v8::Handle<v8::Value> UpdateWindowSurfaceRects(const v8::Arguments& args);

		SDL_Window* window_;
	};
}

#endif