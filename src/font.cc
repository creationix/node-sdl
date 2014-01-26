#include "font.h"
#include "helpers.h"
#include "container.h"
#include "surface.h"

#include <iostream>

using namespace v8;
using namespace node;


Persistent<FunctionTemplate> sdl::TTF::FontWrapper::wrap_template_;

sdl::TTF::FontWrapper::FontWrapper() {
}
sdl::TTF::FontWrapper::~FontWrapper() {
	if(NULL != font_) {
		TTF_CloseFont(font_);
	}
}

void sdl::TTF::FontWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("FontWrapper"));

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderTextSolid", RenderTextSolid);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderUtf8Solid", RenderUTF8Solid);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderUnicodeSolid", RenderUnicodeSolid);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderGlyphSolid", RenderGlyphSolid);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderTextShaded", RenderTextShaded);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderUtf8Shaded", RenderUTF8Shaded);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderUnicodeShaded", RenderUnicodeShaded);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderGlyphShaded", RenderGlyphShaded);

	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderTextBlended", RenderTextBlended);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderUtf8Blended", RenderUTF8Blended);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderUnicodeBlended", RenderUnicodeBlended);
	NODE_SET_PROTOTYPE_METHOD(wrap_template_, "renderGlyphBlended", RenderGlyphBlended);

	exports->Set(String::New("Font"), wrap_template_->GetFunction());
}
Handle<Value> sdl::TTF::FontWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("A Font must be created with the new operator.")));
	}

	HandleScope scope;

	if(args[0]->IsExternal()) {
		FontWrapper* obj = new FontWrapper();
		obj->font_ = static_cast<TTF_Font*>(Handle<External>::Cast(args[0])->Value());
		obj->Wrap(args.This());
		return args.This();
	}
	else {
		if(!args[0]->IsString()) {
			return ThrowException(Exception::TypeError(
				String::New("Invalid arguments: First argument to new sdl.Font must be a String.")));
		}
		if(!args[1]->IsNumber()) {
			return ThrowException(Exception::TypeError(
				String::New("Invalid arguments: Second argument to new sdl.Font must be a Number.")));
		}

		String::Utf8Value file(args[0]);
		int ptsize = args[1]->Int32Value();
		TTF_Font* font = TTF_OpenFont(*file, ptsize);
		if(NULL == font) {
			return ThrowSDLException(__func__);
		}

		FontWrapper* obj = new FontWrapper();
		obj->font_ = font;
		obj->Wrap(args.This());
		return args.This();
	}
}

Handle<Value> sdl::TTF::FontWrapper::RenderTextSolid(const Arguments& args) {
	HandleScope scope;
	Context::Scope context_scope(Context::GetCurrent());

	std::cout << "Unwrapping this into FontWrapper." << std::endl;
	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	std::cout << "Checking arguments." << std::endl;
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderTextSolid must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderTextSolid must be an sdl.Color.")));
	}

	std::cout << "Creating Utf8Value from arg 0." << std::endl;
	String::Utf8Value text(args[0]);
	std::cout << "Unwrapping arg 1 to ColorWrapper." << std::endl;
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderTextSolid for some reason. (is it not an sdl.Color?)")));
	}

	std::cout << "Rendering text." << std::endl;
	SDL_Surface* surface = TTF_RenderText_Solid(font->font_, *text, *color->color_);
	std::cout << "Done rendering text." << std::endl;
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	std::cout << "Creating new Javascript object to carry the surface wrapper." << std::endl;
	Handle<Value> arg = External::New(surface);
	Handle<Value> argv[] = {arg};
	Handle<Object> ret = SurfaceWrapper::wrap_template_->GetFunction()->NewInstance(1, argv);
	// std::cout << "Internal field count: " << ret->InternalFieldCount() << std::endl;
	// std::cout << "Creating a surface wrapper, which will wrap the previous object." << std::endl;
	// SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	// std::cout << "Setting the wrapper internal surface to the rendered text." << std::endl;
	// wrap->surface_ = surface;
	// std::cout << "Returning the Javascript surface." << std::endl;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderUTF8Solid(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderUtf8Solid must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUtf8Solid must be an sdl.Color.")));
	}

	String::Utf8Value text(args[0]);
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUtf8Solid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUTF8_Solid(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderUnicodeSolid(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUnicodeSolid must be an sdl.Color.")));
	}

	String::Value text(args[0]);
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUnicodeSolid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUNICODE_Solid(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderGlyphSolid(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderGlyphSolid must be an sdl.Color.")));
	}

	String::Value text(args[0]);
	if(0 == text.length()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderGlyphSolid must be able to convert to a string of at least length 1.")));
	}
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderGlyphSolid for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderGlyph_Solid(font->font_, (*text)[0], *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}

Handle<Value> sdl::TTF::FontWrapper::RenderTextShaded(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderTextShaded must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderTextShaded must be an sdl.Color.")));
	}
	if(!args[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Third argument to renderTextShaded must be an sdl.Color.")));
	}

	String::Utf8Value text(args[0]);
	ColorWrapper* fg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderTextShaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap third argument to renderTextShaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderText_Shaded(font->font_, *text, *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderUTF8Shaded(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderUtf8Shaded must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUtf8Shaded must be an sdl.Color.")));
	}
	if(!args[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Third argument to renderUtf8Shaded must be an sdl.Color.")));
	}

	String::Utf8Value text(args[0]);
	ColorWrapper* fg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUtf8Shaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap third argument to renderUtf8Shaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUTF8_Shaded(font->font_, *text, *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderUnicodeShaded(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderUnicodeShaded must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUnicodeShaded must be an sdl.Color.")));
	}
	if(!args[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Third argument to renderUnicodeShaded must be an sdl.Color.")));
	}

	String::Value text(args[0]);
	ColorWrapper* fg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap third argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUNICODE_Shaded(font->font_, *text, *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderGlyphShaded(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderUnicodeShaded must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUnicodeShaded must be an sdl.Color.")));
	}
	if(!args[2]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Third argument to renderUnicodeShaded must be an sdl.Color.")));
	}

	String::Value text(args[0]);
	if(0 == text.length()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderGlyphShaded must be able to convert to a string of at least length 1.")));
	}
	ColorWrapper* fg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == fg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}
	ColorWrapper* bg = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[2]));
	if(NULL == bg) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap third argument to renderUnicodeShaded for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderGlyph_Shaded(font->font_, (*text)[0], *fg->color_, *bg->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}

Handle<Value> sdl::TTF::FontWrapper::RenderTextBlended(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderTextBlended must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderTextBlended must be an sdl.Color.")));
	}

	String::Utf8Value text(args[0]);
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderTextBlended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderText_Blended(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderUTF8Blended(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderUtf8Blended must be a String.")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUtf8Blended must be an sdl.Color.")));
	}

	String::Utf8Value text(args[0]);
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUtf8Blended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderUnicodeBlended(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderUnicodeBlended must be an sdl.Color.")));
	}

	String::Value text(args[0]);
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderUnicodeBlended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderUNICODE_Blended(font->font_, *text, *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}
Handle<Value> sdl::TTF::FontWrapper::RenderGlyphBlended(const Arguments& args) {
	HandleScope scope;

	FontWrapper* font = ObjectWrap::Unwrap<FontWrapper>(args.This());
	if(NULL == font) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap 'this' for some reason. (did you not use a Font object?)")));
	}
	if(!args[1]->IsObject()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Second argument to renderGlyphBlended must be an sdl.Color.")));
	}

	String::Value text(args[0]);
	if(0 == text.length()) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: First argument to renderGlyphBlended must be able to convert to a string of at least length 1.")));
	}
	ColorWrapper* color = ObjectWrap::Unwrap<ColorWrapper>(Handle<Object>::Cast(args[1]));
	if(NULL == color) {
		return ThrowException(Exception::TypeError(
			String::New("Could not unwrap second argument to renderGlyphBlended for some reason. (is it not an sdl.Color?)")));
	}

	SDL_Surface* surface = TTF_RenderGlyph_Blended(font->font_, (*text)[0], *color->color_);
	if(NULL == surface) {
		return ThrowSDLException(__func__);
	}

	Handle<Object> ret = Object::New();
	SurfaceWrapper* wrap = new SurfaceWrapper(ret);
	wrap->surface_ = surface;
	return scope.Close(ret);
}

void sdl::TTF::Initialize(Handle<Object> exports) {
	Handle<Object> TTF = Object::New();
	exports->Set(String::New("TTF"), TTF);
	NODE_SET_METHOD(TTF, "init", Init);
	NODE_SET_METHOD(TTF, "wasInit", WasInit);
	NODE_SET_METHOD(TTF, "quit", Quit);
	NODE_SET_METHOD(TTF, "getError", GetError);

	FontWrapper::Init(TTF);
}

Handle<Value> sdl::TTF::Init(const Arguments& args) {
	HandleScope scope;

	if (!(args.Length() == 0)) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::Init()")));
	}

	if (TTF_Init() < 0) {
		return ThrowException(Exception::Error(String::Concat(
			String::New("TTF::Init: "),
			String::New(TTF_GetError())
			)));
	}

	return Undefined();
}

Handle<Value> sdl::TTF::WasInit(const Arguments& args) {
	HandleScope scope;

	return scope.Close(Boolean::New(TTF_WasInit() ? true : false));
}

Handle<Value> sdl::TTF::Quit(const Arguments& args) {
	HandleScope scope;

	TTF_Quit();

	return Undefined();
}

// Handle<Value> sdl::TTF::SetError(const Arguments& args) {
// 	HandleScope scope;

// 	return Undefined();
// }
Handle<Value> sdl::TTF::GetError(const Arguments& args) {
	HandleScope scope;

	const char* error = TTF_GetError();
	// Can this happen?!
	if(NULL == error) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(String::New(error));
}

// Handle<Value> sdl::TTF::OpenFont(const Arguments& args) {
// 	HandleScope scope;

// 	if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsNumber())) {
// 		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::OpenFont(String, Number)")));
// 	}

// 	String::Utf8Value file(args[0]);
// 	int ptsize = (args[1]->Int32Value());

// 	TTF_Font* font = TTF_OpenFont(*file, ptsize);
// 	if (font == NULL) {
// 		return ThrowException(Exception::Error(String::Concat(
// 			String::New("TTF::OpenFont: "),
// 			String::New(TTF_GetError())
// 			)));
// 	}
// 	return Undefined();
// 	// return scope.Close(WrapFont(font));
// }

// TODO: Rewrite for SDL2.
// static Handle<Value> sdl::TTF::RenderTextBlended(const Arguments& args) {
//   HandleScope scope;

//   if (!(args.Length() == 3 && args[0]->IsObject() && args[1]->IsString() && args[2]->IsNumber())) {
//     return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected TTF::RenderTextBlended(Font, String, Number)")));
//   }

//   SDL_PixelFormat* vfmt = SDL_GetVideoInfo()->vfmt;
//   TTF_Font* font = UnwrapFont(args[0]->ToObject());
//   String::Utf8Value text(args[1]);
//   int colorCode = args[2]->Int32Value();

//   Uint8 r, g, b;
//   SDL_GetRGB(colorCode, vfmt, &r, &g, &b);

//   SDL_Color color;
//   color.r = r;
//   color.g = g;
//   color.b = b;

//   SDL_Surface *resulting_text;
//   resulting_text = TTF_RenderText_Blended(font, *text, color);
//   if (!resulting_text) {
//     return ThrowException(Exception::Error(String::Concat(
//       String::New("TTF::RenderTextBlended: "),
//       String::New(TTF_GetError())
//     )));
//   }
//   return scope.Close(WrapSurface(resulting_text));
// }
