#include "texture.h"
#include "helpers.h"
#include "struct_wrappers.h"
#include "render.h"

#include "SDL.h"

using namespace v8;

////////////////////////////////////////////////////////////////////////////////
// TextureWrapper Class Definition.
Persistent<FunctionTemplate> sdl::TextureWrapper::texture_wrap_template_;

sdl::TextureWrapper::TextureWrapper() {
}

sdl::TextureWrapper::TextureWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}

sdl::TextureWrapper::~TextureWrapper() {
	if(NULL != texture_) {
		SDL_DestroyTexture(texture_);
	}
}

void sdl::TextureWrapper::Init(Handle<Object> exports) {
  // Setup hardware renderer construction.
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	texture_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	texture_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	texture_wrap_template_->SetClassName(String::NewSymbol("TextureWrapper"));

	exports->Set(String::NewSymbol("Texture"), texture_wrap_template_->GetFunction());
}

Handle<Value> sdl::TextureWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of a Texture.")));
	}

	HandleScope scope;

	SDL_Texture* tex;
	RendererWrapper* r = ObjectWrap::Unwrap<RendererWrapper>(Handle<Object>::Cast(args[0]));
	if(args.Length() > 2) {
		int format = args[1]->IsUndefined() ? SDL_PIXELFORMAT_UNKNOWN : args[1]->Int32Value();
		int access = args[2]->IsUndefined() ? SDL_TEXTUREACCESS_STREAMING : args[2]->Int32Value();
		int w = args[3]->IsUndefined() ? 1 : args[3]->Int32Value();
		int h = args[4]->IsUndefined() ? 1 : args[4]->Int32Value();
		tex = SDL_CreateTexture(r->renderer_, format, access, w, h);
	}
	else {
		SDL_Surface* surface = UnwrapSurface(Handle<Object>::Cast(args[1]));
		tex = SDL_CreateTextureFromSurface(r->renderer_, surface);
	}
	if(NULL == tex) {
		return ThrowSDLException(__func__);
	}
	TextureWrapper* obj = new TextureWrapper();
	obj->texture_ = tex;
	obj->Wrap(args.This());

	return args.This();
}
