#include <iostream>

#include "texture.h"
#include "surface.h"
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
	// std::cout << "Texture::New - Checking for constructor call." << std::endl;
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of a Texture.")));
	}

	HandleScope scope;

	SDL_Texture* tex;
	// std::cout << "Texture::New - Unwrapping RendererWrapper from first argument." << std::endl;
	RendererWrapper* r = ObjectWrap::Unwrap<RendererWrapper>(Handle<Object>::Cast(args[0]));
	// std::cout << "Texture::New - Done. Checking arguments length." << std::endl;
	if(args.Length() > 2) {
		// std::cout << "Texture::New - Found more than 2 arguments. Pulling format, access, w, h." << std::endl;
		int format = args[1]->IsUndefined() ? SDL_PIXELFORMAT_UNKNOWN : args[1]->Int32Value();
		int access = args[2]->IsUndefined() ? SDL_TEXTUREACCESS_STREAMING : args[2]->Int32Value();
		int w = args[3]->IsUndefined() ? 1 : args[3]->Int32Value();
		int h = args[4]->IsUndefined() ? 1 : args[4]->Int32Value();
		// std::cout << "Texture::New - Done. Calling SDL_CreateTexture." << std::endl;
		tex = SDL_CreateTexture(r->renderer_, format, access, w, h);
		// std::cout << "Texture::New - Done." << std::endl;
	}
	else if(args.Length() == 2) {
		// std::cout << "Texture::New - Found 2 arguments. Pulling SurfaceWrapper from second arg." << std::endl;
		SurfaceWrapper* wrap = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[1]));
		// std::cout << "Texture::New - Done. Calling SDL_CreateTextureFromSurface." << std::endl;
		tex = SDL_CreateTextureFromSurface(r->renderer_, wrap->surface_);
		// std::cout << "Texture::New - Done." << std::endl;
	}
	else {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: expecting new sdl.Texture(Renderer, Surface) or new sdl.Texture(Renderer, Number, Number, Number, Number)")));
	}
	if(NULL == tex) {
		return ThrowSDLException(__func__);
	}

	// std::cout << "Texture::New - Creating new TextureWrapper." << std::endl;
	TextureWrapper* obj = new TextureWrapper();
	// std::cout << "Texture::New - Setting texture_ to created tex." << std::endl;
	obj->texture_ = tex;
	// std::cout << "Texture::New - Wrapping args.This()." << std::endl;
	obj->Wrap(args.This());

	// std::cout << "Texture::New - Returning args.This()." << std::endl;
	return args.This();
}
