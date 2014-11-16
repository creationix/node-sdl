#ifndef NODESDL_RENDER_H
#define NODESDL_RENDER_H

#include <node.h>
#include <v8.h>

#include "SDL.h"


namespace sdl {
	class RendererWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> render_wrap_template_;
		static v8::Persistent<v8::FunctionTemplate> software_render_wrap_template_;

		RendererWrapper();
		~RendererWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> NewSoftware(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetDrawBlendMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetDrawColor(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetOutputSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetTarget(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetInfo(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetClipRect(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetLogicalSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetScale(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetViewport(const v8::Arguments& args);
		static v8::Handle<v8::Value> ReadPixels(const v8::Arguments& args);
		static v8::Handle<v8::Value> TargetSupported(const v8::Arguments& args);

		static v8::Handle<v8::Value> SetClipRect(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetLogicalSize(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetScale(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetViewport(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetDrawBlendMode(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetDrawColor(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetTarget(const v8::Arguments& args);

		static v8::Handle<v8::Value> Clear(const v8::Arguments& args);
		static v8::Handle<v8::Value> Present(const v8::Arguments& args);
		static v8::Handle<v8::Value> Copy(const v8::Arguments& args);
		static v8::Handle<v8::Value> DrawLine(const v8::Arguments& args);
		static v8::Handle<v8::Value> DrawLines(const v8::Arguments& args);
		static v8::Handle<v8::Value> DrawPoint(const v8::Arguments& args);
		static v8::Handle<v8::Value> DrawPoints(const v8::Arguments& args);
		static v8::Handle<v8::Value> DrawRect(const v8::Arguments& args);
		static v8::Handle<v8::Value> DrawRects(const v8::Arguments& args);
		static v8::Handle<v8::Value> FillRect(const v8::Arguments& args);
		static v8::Handle<v8::Value> FillRects(const v8::Arguments& args);

		SDL_Renderer* renderer_;
	};
}

#endif
