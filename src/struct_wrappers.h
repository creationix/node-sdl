#ifndef STRUCT_WRAPPERS_H
#define STRUCT_WRAPPERS_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

namespace sdl {
	void InitWrappers(v8::Handle<v8::Object> exports);

	///////////////////////////////////////////////////////////////////////////////
  	// Wrappers and Unwrappers.
  	// These include v8::ObjectTemplates which have properties for the same fields
  	// as defined in the SDL documentation. (excluding anything SDL uses internally
  	// or is just plain unused)

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Surface Wrapper/Unwrapper.
	v8::Handle<v8::Value> ConstructSurface(const v8::Arguments& args);
	v8::Handle<v8::Object> WrapSurface(SDL_Surface* surface);
	SDL_Surface* UnwrapSurface(v8::Handle<v8::Object> obj);
	// Property getters;
	v8::Handle<v8::Value> GetSurfaceFlags(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetSurfaceFormat(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetSurfaceWidth(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetSurfaceHeight(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetSurfacePitch(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetSurfaceRect(v8::Local<v8::String> name, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Rect Wrapper/Unwrapper.
	v8::Handle<v8::Value> ConstructRect(const v8::Arguments& args);
	v8::Handle<v8::Object> WrapRect(SDL_Rect* rect);
	SDL_Rect* UnwrapRect(v8::Handle<v8::Object> obj);
	// Property getters.
	v8::Handle<v8::Value> GetRectX(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRectY(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRectW(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRectH(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	// Property setters.
	void SetRectX(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	void SetRectY(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	void SetRectW(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	void SetRectH(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Point Wrapper/Unwrapper.
	class PointWrapper : public node::ObjectWrap {
	public:
	    static v8::Persistent<v8::FunctionTemplate> point_wrap_template_;

		PointWrapper();
		~PointWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetX(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> GetY(v8::Local<v8::String> name, const v8::AccessorInfo& info);
		static void SetX(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static void SetY(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
		static v8::Handle<v8::Value> ToString(const v8::Arguments& args);

		SDL_Point* point_;
	};

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Color Wrapper/Unwrapper.
	v8::Handle<v8::Value> ConstructColor(const v8::Arguments& args);
	v8::Handle<v8::Object> WrapColor(SDL_Color* color);
	SDL_Color* UnwrapColor(v8::Handle<v8::Object> obj);
	// Property getters.
	v8::Handle<v8::Value> GetColorRed(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetColorGreen(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetColorBlue(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetColorAlpha(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	// Property setters.
	void SetColorRed(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	void SetColorGreen(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	void SetColorBlue(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	void SetColorAlpha(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Palette Wrapper/Unwrapper.
	v8::Handle<v8::Value> ConstructPalette(const v8::Arguments& args);
	v8::Handle<v8::Object> WrapPalette(SDL_Palette* palette);
	SDL_Palette* UnwrapPalette(v8::Handle<v8::Object> obj);
	// Property functions.
	v8::Handle<v8::Value> GetNcolors(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetColors(v8::Local<v8::String> name, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_DisplayMode Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapDisplayMode(SDL_DisplayMode* mode);
	SDL_DisplayMode* UnwrapDisplayMode(v8::Handle<v8::Value> val);
	v8::Handle<v8::Value> GetDisplayModeFormat(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetDisplayModeWidth(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetDisplayModeHeight(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetDisplayModeRefreshRate(v8::Local<v8::String> name, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_PixelFormat Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapPixelFormat(SDL_PixelFormat* pixelformat);
	SDL_PixelFormat* UnwrapPixelFormat(v8::Handle<v8::Object> obj);
	// Property getters.
	v8::Handle<v8::Value> GetFormatFormat(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatPalette(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatBits(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatBytes(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatRmask(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatGmask(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatBmask(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetFormatAmask(v8::Local<v8::String> name, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_RendererInfo Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapRendererInfo(SDL_RendererInfo* info);
	SDL_RendererInfo* UnwrapRendererInfo(v8::Handle<v8::Object> obj);
	// Property getters.
	v8::Handle<v8::Value> GetRendererInfoName(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRendererInfoFlags(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRendererInfoNumTextureFormats(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRendererInfoTextureFormats(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRendererInfoMaxTextureWidth(v8::Local<v8::String> name, const v8::AccessorInfo& info);
	v8::Handle<v8::Value> GetRendererInfoMaxTextureHeight(v8::Local<v8::String> name, const v8::AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Joystick Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapJoystick(SDL_Joystick* joystick);
	SDL_Joystick* UnwrapJoystick(v8::Handle<v8::Object> obj);

	///////////////////////////////////////////////////////////////////////////////
	// TTF_Font Wrapper/Unwrapper.
	v8::Handle<v8::Object> WrapFont(TTF_Font* font);
	TTF_Font* UnwrapFont(v8::Handle<v8::Object> obj);
}

#endif