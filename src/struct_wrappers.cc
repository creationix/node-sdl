#include "struct_wrappers.h"

namespace sdl {
	static Persistent<ObjectTemplate> rect_template_;
	static Persistent<ObjectTemplate> color_template_;
	static Persistent<ObjectTemplate> surface_template_;
	static Persistent<ObjectTemplate> palette_template_;
	static Persistent<ObjectTemplate> pixelformat_template_;
	static Persistent<ObjectTemplate> displaymode_template_;

	///////////////////////////////////////////////////////////////////////////////
	// Initialize everything we need to use the wrappers.
	void InitWrappers() {
		Handle<ObjectTemplate> raw_template = MakeColorTemplate();
		color_template_ = Persistent<ObjectTemplate>::New(raw_template);

		raw_template = MakeRectTemplate();
		rect_template_ = Persistent<ObjectTemplate>::New(raw_template);

		raw_template = MakeSurfaceTemplate();
		surface_template_ = Persistent<ObjectTemplate>::New(raw_template);

		raw_template = MakePaletteTemplate();
		palette_template_ = Persistent<ObjectTemplate>::New(raw_template);

		raw_template = MakePixelFormatTemplate();
		pixelformat_template_ = Persistent<ObjectTemplate>::New(raw_template);

		raw_template = MakeDisplayModeTemplate();
		displaymode_template_ = Persistent<ObjectTemplate>::New(raw_template);
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Surface Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeSurfaceTemplate() {
		HandleScope handle_scope;

		Handle<ObjectTemplate> result = ObjectTemplate::New();
		result->SetInternalFieldCount(1);

  		// Add accessors for some of the fields of the surface.
		result->SetAccessor(String::NewSymbol("flags"), GetSurfaceFlags);
		result->SetAccessor(String::NewSymbol("format"), GetSurfaceFormat);
		result->SetAccessor(String::NewSymbol("w"), GetSurfaceWidth);
		result->SetAccessor(String::NewSymbol("h"), GetSurfaceHeight);
		result->SetAccessor(String::NewSymbol("pitch"), GetSurfacePitch);
		result->SetAccessor(String::NewSymbol("clip_rect"), GetSurfaceRect);

  		// Again, return the result through the current handle scope.
		return handle_scope.Close(result);
	}

	Handle<Object> WrapSurface(SDL_Surface* surface) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

		Handle<ObjectTemplate> templ = surface_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(surface);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_Surface* UnwrapSurface(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_Surface*>(ptr);
	}

	// Property getters.
	Handle<Value> GetSurfaceFlags(Local<String> name, const AccessorInfo& info) {
		SDL_Surface* surface = UnwrapSurface(info.Holder());
		return Number::New(surface->flags);
	}
	Handle<Value> GetSurfaceFormat(Local<String> name, const AccessorInfo& info) {
		HandleScope scope;
		SDL_Surface* surface = UnwrapSurface(info.Holder());
		return scope.Close(WrapPixelFormat(surface->format));
	}
	Handle<Value> GetSurfaceWidth(Local<String> name, const AccessorInfo& info) {
		SDL_Surface* surface = UnwrapSurface(info.Holder());
		return Number::New(surface->w);
	}
	Handle<Value> GetSurfaceHeight(Local<String> name, const AccessorInfo& info) {
		SDL_Surface* surface = UnwrapSurface(info.Holder());
		return Number::New(surface->h);
	}
	Handle<Value> GetSurfacePitch(Local<String> name, const AccessorInfo& info) {
		SDL_Surface* surface = UnwrapSurface(info.Holder());
		return Number::New(surface->pitch);
	}
	Handle<Value> GetSurfaceRect(Local<String> name, const AccessorInfo& info) {
		HandleScope scope;
		SDL_Surface* surface = UnwrapSurface(info.Holder());
		return scope.Close(WrapRect(&surface->clip_rect));
	}
	// Property setters.

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Rect Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeRectTemplate() {
		HandleScope handle_scope;

		Handle<ObjectTemplate> result = ObjectTemplate::New();
		result->SetInternalFieldCount(1);

	  	// Add accessors for some of the fields of the rect.
		result->SetAccessor(String::NewSymbol("x"), GetRectX);
		result->SetAccessor(String::NewSymbol("y"), GetRectY);
		result->SetAccessor(String::NewSymbol("w"), GetRectW);
		result->SetAccessor(String::NewSymbol("h"), GetRectH);

	  	// Again, return the result through the current handle scope.
		return handle_scope.Close(result);
	}

	Handle<Object> WrapRect(SDL_Rect* rect) {
	  	// Handle scope for temporary handles.
		HandleScope handle_scope;

		Handle<ObjectTemplate> templ = rect_template_;

	  	// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

	  	// Wrap the raw C++ pointer in an External so it can be referenced
	  	// from within JavaScript.
		Handle<External> request_ptr = External::New(rect);

	  	// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

	  	// Return the result through the current handle scope.  Since each
	  	// of these handles will go away when the handle scope is deleted
	  	// we need to call Close to let one, the result, escape into the
	  	// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_Rect* UnwrapRect(Handle<Object> obj) {
	  Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
	  void* ptr = field->Value();
	  return static_cast<SDL_Rect*>(ptr);
	}

	// Property getters.
	Handle<Value> GetRectX(Local<String> name, const AccessorInfo& info) {
		SDL_Rect* rect = UnwrapRect(info.Holder());
		return Number::New(rect->x);
	}
	Handle<Value> GetRectY(Local<String> name, const AccessorInfo& info) {
		SDL_Rect* rect = UnwrapRect(info.Holder());
		return Number::New(rect->y);
	}
	Handle<Value> GetRectW(Local<String> name, const AccessorInfo& info) {
		SDL_Rect* rect = UnwrapRect(info.Holder());
		return Number::New(rect->w);
	}
	Handle<Value> GetRectH(Local<String> name, const AccessorInfo& info) {
		SDL_Rect* rect = UnwrapRect(info.Holder());
		return Number::New(rect->h);
	}
	// Property setters.
	Handle<Value> SetRectX(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Rect* rect = UnwrapRect(info.Holder());
			rect->x = value->Int32Value();
		}
		return Undefined();
	}
	Handle<Value> SetRectY(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Rect* rect = UnwrapRect(info.Holder());
			rect->y = value->Int32Value();
		}
		return Undefined();
	}
	Handle<Value> SetRectW(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Rect* rect = UnwrapRect(info.Holder());
			rect->w = value->Int32Value();
		}
		return Undefined();
	}
	Handle<Value> SetRectH(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Rect* rect = UnwrapRect(info.Holder());
			rect->h = value->Int32Value();
		}
		return Undefined();
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Color Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeColorTemplate() {
		HandleScope handle_scope;

		Handle<ObjectTemplate> result = ObjectTemplate::New();
		result->SetInternalFieldCount(1);

		result->SetAccessor(String::NewSymbol("r"), GetColorRed);
		result->SetAccessor(String::NewSymbol("g"), GetColorGreen);
		result->SetAccessor(String::NewSymbol("b"), GetColorBlue);
		result->SetAccessor(String::NewSymbol("a"), GetColorAlpha);

		return handle_scope.Close(result);
	}

	Handle<Object> WrapColor(SDL_Color* color) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

		// Fetch the template for creating JavaScript http request wrappers.
		// It only has to be created once, which we do on demand.
		if (color_template_.IsEmpty()) {
			Handle<ObjectTemplate> raw_template = MakeColorTemplate();
			color_template_ = Persistent<ObjectTemplate>::New(raw_template);
		}
		Handle<ObjectTemplate> templ = color_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(color);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_Color* UnwrapColor(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_Color*>(ptr);
	}

	// Property getters.
	Handle<Value> GetColorRed(Local<String> name, const AccessorInfo& info) {
		SDL_Color* color = UnwrapColor(info.Holder());
		return Number::New(color->r);
	}
	Handle<Value> GetColorGreen(Local<String> name, const AccessorInfo& info) {
		SDL_Color* color = UnwrapColor(info.Holder());
		return Number::New(color->g);
	}
	Handle<Value> GetColorBlue(Local<String> name, const AccessorInfo& info) {
		SDL_Color* color = UnwrapColor(info.Holder());
		return Number::New(color->b);
	}
	Handle<Value> GetColorAlpha(Local<String> name, const AccessorInfo& info) {
		SDL_Color* color = UnwrapColor(info.Holder());
		return Number::New(color->a);
	}
	// Property setters.
	Handle<Value> SetColorRed(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->r = value->Int32Value();
		}
		return Undefined();
	}
	Handle<Value> SetColorGreen(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->g = value->Int32Value();
		}
		return Undefined();
	}
	Handle<Value> SetColorBlue(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->b = value->Int32Value();
		}
		return Undefined();
	}
	Handle<Value> SetColorAlpha(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->a = value->Int32Value();
		}
		return Undefined();
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Palette Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakePaletteTemplate() {
		HandleScope handle_scope;

		Handle<ObjectTemplate> result = ObjectTemplate::New();
		result->SetInternalFieldCount(1);

		result->SetAccessor(String::NewSymbol("ncolors"), GetNcolors);
		result->SetAccessor(String::NewSymbol("colors"), GetColors);

		return handle_scope.Close(result);
	}

	Handle<Object> WrapPalette(SDL_Palette* palette) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

		Handle<ObjectTemplate> templ = palette_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(palette);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_Palette* UnwrapPalette(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_Palette*>(ptr);
	}

	// Property getters.
	Handle<Value> GetNcolors(Local<String> name, const AccessorInfo& info) {
		SDL_Palette* palette = UnwrapPalette(info.Holder());
		return Number::New(palette->ncolors);
	}
	Handle<Value> GetColors(Local<String> name, const AccessorInfo& info) {
		SDL_Palette* palette = UnwrapPalette(info.Holder());
		Handle<Array> ret = Array::New(palette->ncolors);
		for(int i = 0; i < palette->ncolors; i++) {
			ret->Set(i, WrapColor(palette->colors + i));
		}
		return ret;
	}
	// Property setters.

	///////////////////////////////////////////////////////////////////////////////
	// SDL_DisplayMode Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakeDisplayModeTemplate() {
		HandleScope scope;

		Handle<ObjectTemplate> result = ObjectTemplate::New();
		result->SetInternalFieldCount(1);

		result->SetAccessor(String::NewSymbol("format"), GetDisplayModeFormat);
		result->SetAccessor(String::NewSymbol("w"), GetDisplayModeWidth);
		result->SetAccessor(String::NewSymbol("h"), GetDisplayModeHeight);
		result->SetAccessor(String::NewSymbol("refreshRate"), GetDisplayModeRefreshRate);

		return handle_scope.Close(result);
	}

	Handle<Object> WrapDisplayMode(SDL_DisplayMode* mode) {
		HandleScope scope;

		Handle<ObjectTemplate> templ = displaymode_template_;
		Handle<Object> result = templ->NewInstance();
		Handle<External> request_ptr = External::New(mode);
		result->SetInternalField(0, request_ptr);
		return scope.Close(result);
	}

	SDL_DisplayMode* UnwrapDisplayMode(Handle<Value> val) {
		Handle<Object> obj = Handle<Object>::Cast(val);
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_DisplayMode*>(ptr);
	}

	Handle<Value> GetDisplayModeFormat(Local<String> name, const AccessorInfo& info) {
		SDL_DisplayMode* mode = UnwrapDisplayMode(info.Holder());
		return Number::New(mode->format);
	}
	Handle<Value> GetDisplayModeWidth(Local<String> name, const AccessorInfo& info) {
		SDL_DisplayMode* mode = UnwrapDisplayMode(info.Holder());
		return Number::New(mode->w);
	}
	Handle<Value> GetDisplayModeHeight(Local<String> name, const AccessorInfo& info) {
		SDL_DisplayMode* mode = UnwrapDisplayMode(info.Holder());
		return Number::New(mode->h);
	}
	Handle<Value> GetDisplayModeRefreshRate(Local<String> name, const AccessorInfo& info) {
		SDL_DisplayMode* mode = UnwrapDisplayMode(info.Holder());
		return Number::New(mode->refresh_rate);
	}


	///////////////////////////////////////////////////////////////////////////////
	// SDL_PixelFormat Wrapper/Unwrapper.
	Handle<ObjectTemplate> MakePixelFormatTemplate() {
		HandleScope handle_scope;

		Handle<ObjectTemplate> result = ObjectTemplate::New();
		result->SetInternalFieldCount(1);

  		// Add accessors for some of the fields of the pixelformat.
		result->SetAccessor(String::NewSymbol("bitsPerPixel"), GetFormatBits);
		result->SetAccessor(String::NewSymbol("bytesPerPixel"), GetFormatBytes);
		result->SetAccessor(String::NewSymbol("rmask"), GetFormatRmask);
		result->SetAccessor(String::NewSymbol("gmask"), GetFormatGmask);
		result->SetAccessor(String::NewSymbol("bmask"), GetFormatBmask);
		result->SetAccessor(String::NewSymbol("amask"), GetFormatAmask);

  		// Again, return the result through the current handle scope.
		return handle_scope.Close(result);
	}

	Handle<Object> WrapPixelFormat(SDL_PixelFormat* pixelformat) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

		Handle<ObjectTemplate> templ = pixelformat_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(pixelformat);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_PixelFormat* UnwrapPixelFormat(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_PixelFormat*>(ptr);
	}

	// Property getters.
	Handle<Value> GetFormatFormat(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->format);
	}
	Handle<Value> GetFormatPalette(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return WrapPalette(format->palette);
	}
	Handle<Value> GetFormatBits(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->BitsPerPixel);
	}
	Handle<Value> GetFormatBytes(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->BytesPerPixel);
	}
	Handle<Value> GetFormatRmask(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->Rmask);
	}
	Handle<Value> GetFormatGmask(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->Gmask);
	}
	Handle<Value> GetFormatBmask(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->Bmask);
	}
	Handle<Value> GetFormatAmask(Local<String> name, const AccessorInfo& info) {
		SDL_PixelFormat* format = UnwrapPixelFormat(info.Holder());
		return Number::New(format->Amask);
	}

}