#ifndef STRUCT_WRAPPERS_H
#define STRUCT_WRAPPERS_H

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

namespace sdl {
	void InitWrappers();

	///////////////////////////////////////////////////////////////////////////////
  	// Wrappers and Unwrappers.
  	// These include ObjectTemplates which have properties for the same fields
  	// as defined in the SDL documentation. (excluding anything SDL uses internally
  	// or is just plain unused)

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Surface Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeSurfaceTemplate();
	Handle<Object> WrapSurface(SDL_Surface* surface);
	SDL_Surface* UnwrapSurface(Handle<Object> obj);
	// Property getters;
	Handle<Value> GetSurfaceFlags(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetSurfaceFormat(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetSurfaceWidth(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetSurfaceHeight(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetSurfacePitch(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetSurfaceRect(Local<String> name, const AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Rect Wrapper/Unwrapper.
	Handle<Object> WrapRect(SDL_Rect* rect);
	SDL_Rect* UnwrapRect(Handle<Object> obj);
	// Property getters.
	Handle<Value> GetRectX(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetRectY(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetRectW(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetRectH(Local<String> name, const AccessorInfo& info);
	// Property setters.
	Handle<Value> SetRectX(Local<String> name, Local<Value> value, const AccessorInfo& info);
	Handle<Value> SetRectY(Local<String> name, Local<Value> value, const AccessorInfo& info);
	Handle<Value> SetRectW(Local<String> name, Local<Value> value, const AccessorInfo& info);
	Handle<Value> SetRectH(Local<String> name, Local<Value> value, const AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Color Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeColorTemplate();
	Handle<Object> WrapColor(SDL_Color* color);
	SDL_Color* UnwrapColor(Handle<Object> obj);
	// Property getters.
	Handle<Value> GetColorRed(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetColorGreen(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetColorBlue(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetColorAlpha(Local<String> name, const AccessorInfo& info);
	// Property setters.
	Handle<Value> SetColorRed(Local<String> name, Local<Value> value, const AccessorInfo& info);
	Handle<Value> SetColorGreen(Local<String> name, Local<Value> value, const AccessorInfo& info);
	Handle<Value> SetColorBlue(Local<String> name, Local<Value> value, const AccessorInfo& info);
	Handle<Value> SetColorAlpha(Local<String> name, Local<Value> value, const AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Palette Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakePaletteTemplate();
	Handle<Object> WrapPalette(SDL_Palette* palette);
	SDL_Palette* UnwrapPalette(Handle<Object> obj);
	// Property functions.
	Handle<Value> GetColorRed(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetNcolors(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetColors(Local<String> name, const AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_DisplayMode Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeDisplayModeTemplate();
	Handle<Object> WrapDisplayMode(SDL_DisplayMode* mode);
	SDL_DisplayMode* UnwrapDisplayMode(Handle<Value> val);
	Handle<Value> GetDisplayModeFormat(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetDisplayModeWidth(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetDisplayModeHeight(Local<String> name, const AccessorInfo& info);
	Handle<Value> GetDisplayModeRefreshRate(Local<String> name, const AccessorInfo& info);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_PixelFormat Wrapper/Unwrapper.
	Handle<Object> WrapPixelFormat(SDL_PixelFormat* pixelformat);
	SDL_PixelFormat* UnwrapPixelFormat(Handle<Object> obj);

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Joystick Wrapper/Unwrapper.
	Handle<Object> WrapJoystick(SDL_Joystick* joystick);
	SDL_Joystick* UnwrapJoystick(Handle<Object> obj);

	///////////////////////////////////////////////////////////////////////////////
	// TTF_Font Wrapper/Unwrapper.
	Handle<Object> WrapFont(TTF_Font* font);
	TTF_Font* UnwrapFont(Handle<Object> obj);
}

#endif