#include "surface.h"
#include "helpers.h"
#include "struct_wrappers.h"
#include "container.h"
#include <iostream>

using namespace v8;
using namespace node;

Persistent<FunctionTemplate> sdl::SurfaceWrapper::wrap_template_;

sdl::SurfaceWrapper::SurfaceWrapper() {
}

sdl::SurfaceWrapper::SurfaceWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}

sdl::SurfaceWrapper::~SurfaceWrapper() {
	if(NULL != surface_) {
		SDL_FreeSurface(surface_);
	}
}

void sdl::SurfaceWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("SurfaceWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "loadBMP", LoadBMP);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "loadBMPRW", LoadBMPRW);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "saveBMP", SaveBMP);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "saveBMPRW", SaveBMPRW);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "blitScaled", BlitScaled);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "blitSurface", BlitSurface);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "lowerBlit", LowerBlit);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "lowerBlitScaled", LowerBlitScaled);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "convertSurface", ConvertSurface);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "convertSurfaceFormat", ConvertSurfaceFormat);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getWidth", GetWidth);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getHeight", GetHeight);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getPitch", GetPitch);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getPixelFormat", GetPixelFormat);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "fillRect", FillRect);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "fillRects", FillRects);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getClipRect", GetClipRect);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getColorKey", GetColorKey);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getAlphaMod", GetAlphaMod);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getBlendMode", GetBlendMode);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "getColorMod", GetColorMod);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setClipRect", SetClipRect);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setColorKey", SetColorKey);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setAlphaMod", SetAlphaMod);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setBlendMode", SetBlendMode);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setColorMod", SetColorMod);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setPalette", SetPalette);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "setRLE", SetRLE);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "lock", Lock);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "unlock", Unlock);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "mustLock", MustLock);

	exports->Set(String::NewSymbol("Surface"), wrap_template_->GetFunction());
}
Handle<Value> sdl::SurfaceWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of a Surface.")));
	}

	HandleScope scope;
	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected new sdl.Surface(Number, Number)")));
	}

	int flags = 0;
	int width = args[0]->Int32Value();
	int height = args[1]->Int32Value();
	int depth = args[2]->IsUndefined() ? 32 : args[2]->Int32Value();
	int rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface* surface = SDL_CreateRGBSurface(flags, width, height, depth, rmask, gmask, bmask, amask);
    if(NULL == surface) {
    	return ThrowSDLException(__func__);
    }

    SurfaceWrapper* obj = new SurfaceWrapper();
    obj->surface_ = surface;
    obj->Wrap(args.This());
    return args.This();
}

Handle<Value> sdl::SurfaceWrapper::LoadBMP(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected loadBMP(String)")));
	}

	String::Utf8Value file(args[0]);
	SDL_Surface* surface = SDL_LoadBMP(*file);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	SurfaceWrapper* obj = new SurfaceWrapper();
	obj->surface_ = surface;
	Handle<Object> ret = Object::New();
	obj->Wrap(ret);

	return scope.Close(ret);
}
Handle<Value> sdl::SurfaceWrapper::LoadBMPRW(const Arguments& args) {
	// TODO: Implement LoadBMPRW.
	return ThrowException(Exception::TypeError(
		String::New("LoadBMPRW is not implemented yet.")));
}
Handle<Value> sdl::SurfaceWrapper::SaveBMP(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected saveBMP(Surface, String)")));
	}

	SurfaceWrapper* wrap = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	String::Utf8Value file(args[1]);
	int err = SDL_SaveBMP(wrap->surface_, *file);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::SaveBMPRW(const Arguments& args) {
	// TODO Implement SaveBMPRW.
	return ThrowException(Exception::TypeError(
		String::New("SaveBMPRW is not implemented yet.")));
}

Handle<Value> sdl::SurfaceWrapper::BlitScaled(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected blitScaled(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = args[1]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_BlitScaled(obj->surface_,
		src == NULL ? NULL : src->rect_,
		other->surface_,
		dst == NULL ? NULL : dst->rect_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::BlitSurface(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected blitSurface(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_BlitSurface(obj->surface_, src->rect_, other->surface_, dst == NULL ? NULL : dst->rect_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::LowerBlit(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected lowerBlit(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_LowerBlit(obj->surface_, src->rect_, other->surface_, dst == NULL ? NULL : dst->rect_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::LowerBlitScaled(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected lowerBlitScaled(Surface, Rect[, Rect])")));
	}

	SurfaceWrapper* obj = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SurfaceWrapper* other = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	RectWrapper* dst = args[1]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	RectWrapper* src = args[2]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[2]));
	int err = SDL_LowerBlitScaled(obj->surface_,
		src == NULL ? NULL : src->rect_,
		other->surface_,
		dst == NULL ? NULL : dst->rect_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::ConvertSurface(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected convertSurface(PixelFormat)")));
	}

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_PixelFormat* fmt = UnwrapPixelFormat(Handle<Object>::Cast(args[0]));
	SDL_Surface* ret = SDL_ConvertSurface(self->surface_, fmt, 0);
	if(NULL == ret) {
		return ThrowSDLException(__func__);
	}

	SurfaceWrapper* obj = new SurfaceWrapper();
	obj->surface_ = ret;
	Handle<Object> objRet = Object::New();
	obj->Wrap(objRet);
	return scope.Close(objRet);
}
Handle<Value> sdl::SurfaceWrapper::ConvertSurfaceFormat(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected convertSurfaceFormat(Number)")));
	}

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int fmt = args[0]->Int32Value();
	SDL_Surface* ret = SDL_ConvertSurfaceFormat(self->surface_, fmt, 0);
	if(NULL == ret) {
		return ThrowSDLException(__func__);
	}

	SurfaceWrapper* obj = new SurfaceWrapper();
	obj->surface_ = ret;
	Handle<Object> objRet = Object::New();
	obj->Wrap(objRet);
	return scope.Close(objRet);
}

Handle<Value> sdl::SurfaceWrapper::FillRect(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected fillRect(Number[, Rect])")));
	}

	Handle<Object> handleObj = Handle<Object>::Cast(args.This());
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(handleObj);
	int color = args[0]->Int32Value();
	RectWrapper* rect = args[1]->IsUndefined() ? NULL : ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[1]));
	int err = SDL_FillRect(self->surface_, rect == NULL ? NULL : rect->rect_, color);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::FillRects(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected fillRect(Number, Array)")));
	}

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int color = args[0]->Int32Value();
	Handle<Array> arr = Handle<Array>::Cast(args[1]);
	int numRects = arr->Length();
	SDL_Rect* rects = new SDL_Rect[numRects];
	for(int i = 0; i < numRects; i++) {
		RectWrapper* rect = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *rect->rect_;
	}
	int err = SDL_FillRects(self->surface_, rects, numRects, color);
	delete rects;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

Handle<Value> sdl::SurfaceWrapper::GetClipRect(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_Rect* rect = new SDL_Rect;
	SDL_GetClipRect(self->surface_, rect);

	Handle<Object> ret = Object::New();
	RectWrapper* wrap = new RectWrapper(ret);
	wrap->rect_ = rect;

	return scope.Close(ret);
}
Handle<Value> sdl::SurfaceWrapper::GetColorKey(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint32_t colorKey;
	int err = SDL_GetColorKey(self->surface_, &colorKey);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(colorKey));
}
Handle<Value> sdl::SurfaceWrapper::GetAlphaMod(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t alphaMod;
	int err = SDL_GetSurfaceAlphaMod(self->surface_, &alphaMod);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(alphaMod));
}
Handle<Value> sdl::SurfaceWrapper::GetBlendMode(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_BlendMode mode;
	int err = SDL_GetSurfaceBlendMode(self->surface_, &mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(mode));
}
Handle<Value> sdl::SurfaceWrapper::GetColorMod(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t r, g, b;
	int err = SDL_GetSurfaceColorMod(self->surface_, &r, &g, &b);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	Handle<Array> ret = Array::New(3);
	ret->Set(0, Number::New(r));
	ret->Set(1, Number::New(g));
	ret->Set(2, Number::New(b));
	return scope.Close(ret);
}
Handle<Value> sdl::SurfaceWrapper::GetWidth(const Arguments& args) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Number::New(self->surface_->w));
}
Handle<Value> sdl::SurfaceWrapper::GetHeight(const Arguments& args) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Number::New(self->surface_->h));
}
Handle<Value> sdl::SurfaceWrapper::GetPitch(const Arguments& args) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Number::New(self->surface_->pitch));
}
Handle<Value> sdl::SurfaceWrapper::GetPixelFormat(const Arguments& args) {
	HandleScope scope;
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(WrapPixelFormat(self->surface_->format));
}

Handle<Value> sdl::SurfaceWrapper::SetClipRect(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setClipRect(Rect)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	RectWrapper* clip = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(args[0]));
	SDL_bool ret = SDL_SetClipRect(self->surface_, clip->rect_);

	return scope.Close(Boolean::New(ret));
}
Handle<Value> sdl::SurfaceWrapper::SetColorKey(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setColorKey(Boolean, Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int flag = args[0]->BooleanValue() ? 1 : 0;
	uint32_t key = args[1]->Int32Value();
	int err = SDL_SetColorKey(self->surface_, flag, key);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::SetAlphaMod(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setAlphaMod(Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t alpha = static_cast<uint8_t>(args[0]->Int32Value());
	int err = SDL_SetSurfaceAlphaMod(self->surface_, alpha);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::SetBlendMode(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setBlendMode(Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_BlendMode mode = static_cast<SDL_BlendMode>(args[0]->Int32Value());
	int err = SDL_SetSurfaceBlendMode(self->surface_, mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::SetColorMod(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setColorMod(Number, Number, Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	uint8_t r = static_cast<uint8_t>(args[0]->Int32Value());
	uint8_t g = static_cast<uint8_t>(args[1]->Int32Value());
	uint8_t b = static_cast<uint8_t>(args[2]->Int32Value());
	int err = SDL_SetSurfaceColorMod(self->surface_, r, g, b);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::SetPalette(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setPalette(Palette)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_Palette* palette = UnwrapPalette(Handle<Object>::Cast(args[0]));
	int err = SDL_SetSurfacePalette(self->surface_, palette);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::SetRLE(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expected setRLE(Number)")));
	}
	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int flags = args[0]->Int32Value();
	int err = SDL_SetSurfaceRLE(self->surface_, flags);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

Handle<Value> sdl::SurfaceWrapper::Lock(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	int err = SDL_LockSurface(self->surface_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::Unlock(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	SDL_UnlockSurface(self->surface_);

	return Undefined();
}
Handle<Value> sdl::SurfaceWrapper::MustLock(const Arguments& args) {
	HandleScope scope;

	SurfaceWrapper* self = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args.This()));
	return scope.Close(Boolean::New(SDL_MUSTLOCK(self->surface_)));
}
