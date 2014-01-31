#ifndef NODESDL_SURFACE_H
#define NODESDL_SURFACE_H

#include <node.h>
#include <v8.h>

#include "SDL.h"


namespace sdl {
	class SurfaceWrapper : public node::ObjectWrap {
	public:
	    static v8::Persistent<v8::FunctionTemplate> wrap_template_;

	    SurfaceWrapper();
	    SurfaceWrapper(v8::Handle<v8::Object> toWrap);
	    ~SurfaceWrapper();

	    static void Init(v8::Handle<v8::Object> exports);
	    static v8::Handle<v8::Value> New(const v8::Arguments& args);

	    static v8::Handle<v8::Value> LoadBMP(const v8::Arguments& args);
	    static v8::Handle<v8::Value> LoadBMPRW(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SaveBMP(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SaveBMPRW(const v8::Arguments& args);

	    static v8::Handle<v8::Value> BlitScaled(const v8::Arguments& args);
	    static v8::Handle<v8::Value> BlitSurface(const v8::Arguments& args);
	    static v8::Handle<v8::Value> LowerBlit(const v8::Arguments& args);
	    static v8::Handle<v8::Value> LowerBlitScaled(const v8::Arguments& args);
	    static v8::Handle<v8::Value> ConvertSurface(const v8::Arguments& args);
	    static v8::Handle<v8::Value> ConvertSurfaceFormat(const v8::Arguments& args);

	    static v8::Handle<v8::Value> FillRect(const v8::Arguments& args);
	    static v8::Handle<v8::Value> FillRects(const v8::Arguments& args);

	    static v8::Handle<v8::Value> GetClipRect(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetColorKey(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetAlphaMod(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetBlendMode(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetColorMod(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetWidth(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetHeight(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetPitch(const v8::Arguments& args);
	    static v8::Handle<v8::Value> GetPixelFormat(const v8::Arguments& args);

	    static v8::Handle<v8::Value> SetClipRect(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SetColorKey(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SetAlphaMod(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SetBlendMode(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SetColorMod(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SetPalette(const v8::Arguments& args);
	    static v8::Handle<v8::Value> SetRLE(const v8::Arguments& args);

	    static v8::Handle<v8::Value> Lock(const v8::Arguments& args);
	    static v8::Handle<v8::Value> Unlock(const v8::Arguments& args);
	    static v8::Handle<v8::Value> MustLock(const v8::Arguments& args);

		SDL_Surface* surface_;
	};
}

#endif