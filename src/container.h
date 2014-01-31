#ifndef NODESDL_CONTAINER_H
#define NODESDL_CONTAINER_H

#include <node.h>
#include <v8.h>

#include "SDL.h"


namespace sdl {
	class RectWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		RectWrapper();
		RectWrapper(v8::Handle<v8::Object> toWrap);
		~RectWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetX(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetY(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetW(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetH(v8::Local<v8::String> name, const v8::AccessorInfo& info);

		static void SetX(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetY(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetW(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetH(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		SDL_Rect* rect_;
	};

	class ColorWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		ColorWrapper();
		~ColorWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetRed(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetGreen(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetBlue(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetAlpha(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetColor(const v8::Arguments& args);

		static void SetRed(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetGreen(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetBlue(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetAlpha(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

		static v8::Handle<v8::Value> ToString(const v8::Arguments& args);

		SDL_Color* color_;
	};

	class FingerWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		FingerWrapper();
		FingerWrapper(v8::Handle<v8::Object> toWrap);
		~FingerWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetFingerID(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetX(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetY(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetPressure(v8::Local<v8::String> name, const v8::AccessorInfo& info);

		SDL_Finger* finger_;
	};
}

#endif