#include "struct_wrappers.h"
#include "helpers.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <sstream>

using namespace v8;

namespace sdl {
	typedef Handle<ObjectTemplate> (*TemplateMaker)();

	static Persistent<ObjectTemplate> rect_template_;
	static Persistent<ObjectTemplate> point_template_;
	static Persistent<ObjectTemplate> color_template_;
	static Persistent<ObjectTemplate> palette_template_;
	static Persistent<ObjectTemplate> pixelformat_template_;
	static Persistent<ObjectTemplate> rendererinfo_template_;
	static Persistent<ObjectTemplate> displaymode_template_;
	static Persistent<ObjectTemplate> joystick_template_;
	static Persistent<ObjectTemplate> font_template_;

	///////////////////////////////////////////////////////////////////////////////
	// Initialize everything we need to use the wrappers.
	void InitWrappers(Handle<Object> exports) {
		// std::cout << "About to initialize SDL struct wrappers." << std::endl;
		HandleScope scope;
		// std::cout << "Created scope for struct wrapper initialization." << std::endl;

		// std::cout << "Putting together information for color wrapper..." << std::endl;
		const int numColor = 4;
		std::string colorSymbols[] = {"r", "g", "b", "a"};
		AccessorGetter colorGetters[] = {GetColorRed, GetColorGreen, GetColorBlue, GetColorAlpha};
		AccessorSetter colorSetters[] = {SetColorRed, SetColorGreen, SetColorBlue, SetColorAlpha};

		// std::cout << "Putting together information for palette wrapper..." << std::endl;
		const int numberPalette = 2;
		std::string paletteSymbols[] = {"ncolors", "colors"};
		AccessorGetter paletteGetters[] = {GetNcolors, GetColors};
		AccessorSetter paletteSetters[] = {0, 0};

		// std::cout << "Putting together information for display mode wrapper..." << std::endl;
		const int numberDisplayMode = 4;
		std::string displayModeSymbols[] = {"format", "w", "h", "refreshRate"};
		AccessorGetter displayModeGetters[] = {GetDisplayModeFormat, GetDisplayModeWidth,
											   GetDisplayModeHeight, GetDisplayModeRefreshRate};
		AccessorSetter displayModeSetters[] = {0, 0, 0, 0};

		// std::cout << "Putting together information for pixel format wrapper..." << std::endl;
		const int numberPixelFormat = 8;
		std::string pixelFormatSymbols[] = {"format", "palette", "bitsPerPixel", "bytesPerPixel",
											"rmask", "gmask", "bmask", "amask"};
		AccessorGetter pixelFormatGetters[] = {GetFormatFormat, GetFormatPalette, GetFormatBits,
											   GetFormatBytes, GetFormatRmask, GetFormatGmask,
											   GetFormatBmask, GetFormatAmask};
		AccessorSetter pixelFormatSetters[] = {0, 0, 0, 0, 0, 0, 0, 0};

		const int numberRendererInfo = 6;
		std::string rendererInfoSymbols[] = {"name", "flags", "numTextureFormats", "textureFormats",
											 "maxTextureWidth", "maxTextureHeight"};
		AccessorGetter rendererInfoGetters[] = {GetRendererInfoName, GetRendererInfoFlags,
												GetRendererInfoNumTextureFormats,
												GetRendererInfoTextureFormats,
												GetRendererInfoMaxTextureWidth,
												GetRendererInfoMaxTextureHeight};
		AccessorSetter rendererInfoSetters[] = {0, 0, 0, 0, 0, 0};

		// std::cout << "Putting together meta information for creating wrapping bindings..." << std::endl;
		const int numberTemplates = 5;
		Handle<ObjectTemplate> *templates[] = {&color_template_,
											   &palette_template_, &displaymode_template_,
											   &pixelformat_template_, &rendererinfo_template_};
		int numberSymbols[] = {numColor, numberPalette, numberDisplayMode,
							   numberPixelFormat, numberRendererInfo};
		std::string *allSymbols[] = {colorSymbols, paletteSymbols,
									 displayModeSymbols, pixelFormatSymbols, rendererInfoSymbols};
		AccessorGetter *allGetters[] = {colorGetters, paletteGetters,
										displayModeGetters, pixelFormatGetters, rendererInfoGetters};
		AccessorSetter *allSetters[] = {colorSetters, paletteSetters,
										displayModeSetters, pixelFormatSetters, rendererInfoSetters};
		InvocationCallback allConstructors[] = {ConstructColor,
											    ConstructPalette, 0, 0, 0};
		std::string constructorNames[] = {"Color", "Palette",
										  "DisplayMode", "PixelFormat", "RendererInfo"};

		// std::cout << std::endl << "About to begin loop to create wrapping bindings..." << std::endl;
		for(int i = 0; i < numberTemplates; i++) {
			// std::cout << "Creating new ObjectTemplate." << std::endl;
			Handle<ObjectTemplate> raw_template = ObjectTemplate::New();
			// std::cout << "Setting internal field count for ObjectTemplate to 1." << std::endl;
			raw_template->SetInternalFieldCount(1);

			const int symbols = numberSymbols[i];
			// std::cout << "There are " << symbols << " symbols to create." << std::endl;
			std::string *symbolNames = allSymbols[i];
			AccessorGetter *getters = allGetters[i];
			AccessorSetter *setters = allSetters[i];
			for(int j = 0; j < symbols; j++) {
				// std::cout << "Creating symbol: " << symbolNames[j] << std::endl;
				// std::cout << "This symbol " << (setters[j] != 0 ? "has" : "does not have") << " a setter." << std::endl;
				raw_template->SetAccessor(String::NewSymbol(symbolNames[j].c_str()), getters[j], setters[j] == 0 ? 0 : setters[j]);
			}

			// std::cout << "Directly setting the ObjectTemplate with the template that has the accessors." << std::endl;
			*(templates[i]) = Persistent<ObjectTemplate>::New(raw_template);

			if(allConstructors[i] != 0) {
				// std::cout << "Creating constructor with name: " << constructorNames[i] << "." << std::endl;
				NODE_SET_METHOD(exports, constructorNames[i].c_str(), allConstructors[i]);
			}
			// std::cout << std::endl;
		}

		PointWrapper::Init(exports);
		// std::cout << "Finished initializing wrappers." << std::endl;
		// TODO: Joystick and Font.
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Point Wrapper/Unwrapper.
    Persistent<FunctionTemplate> sdl::PointWrapper::point_wrap_template_;

	sdl::PointWrapper::PointWrapper() {
	}
	sdl::PointWrapper::~PointWrapper() {
		if(NULL != point_) {
			delete point_;
		}
	}

	void sdl::PointWrapper::Init(Handle<Object> exports) {
		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		point_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

		point_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
		point_wrap_template_->SetClassName(String::NewSymbol("PointWrapper"));

		Local<ObjectTemplate> proto = point_wrap_template_->PrototypeTemplate();
		proto->SetAccessor(String::NewSymbol("x"), GetX, SetX);
		proto->SetAccessor(String::NewSymbol("y"), GetY, SetY);
		NODE_SET_PROTOTYPE_METHOD(point_wrap_template_, "toString", ToString);

		exports->Set(String::NewSymbol("Point"), point_wrap_template_->GetFunction());
	}
	Handle<Value> sdl::PointWrapper::New(const Arguments& args) {
		if(!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of a Point.")));
		}

		HandleScope scope;

		int x = args[0]->IsUndefined() ? 0 : args[0]->Int32Value();
		int y = args[0]->IsUndefined() ? 0 : args[1]->Int32Value();
		PointWrapper* obj = new PointWrapper();
		obj->point_ = new SDL_Point;
		obj->point_->x = x;
		obj->point_->y = y;
		obj->Wrap(args.This());
		return args.This();
	}

	Handle<Value> sdl::PointWrapper::GetX(Local<String> name, const AccessorInfo& info) {
		HandleScope scope;

		PointWrapper* obj = ObjectWrap::Unwrap<PointWrapper>(info.This());
		return scope.Close(Number::New(obj->point_->x));
	}
	Handle<Value> sdl::PointWrapper::GetY(Local<String> name, const AccessorInfo& info) {
		HandleScope scope;

		PointWrapper* obj = ObjectWrap::Unwrap<PointWrapper>(info.This());
		return scope.Close(Number::New(obj->point_->y));
	}
	void sdl::PointWrapper::SetX(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		HandleScope scope;

		PointWrapper* obj = ObjectWrap::Unwrap<PointWrapper>(info.This());
		int x = value->Int32Value();
		obj->point_->x = x;
	}
	void sdl::PointWrapper::SetY(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		HandleScope scope;

		PointWrapper* obj = ObjectWrap::Unwrap<PointWrapper>(info.This());
		int y = value->Int32Value();
		obj->point_->y = y;
	}

	Handle<Value> sdl::PointWrapper::ToString(const Arguments& args) {
		HandleScope scope;

		// PointWrapper* obj = ObjectWrap::Unwrap<PointWrapper>(args.This());
		int x = args.This()->Get(String::New("x"))->Int32Value();
		int y = args.This()->Get(String::New("y"))->Int32Value();
		std::stringstream ss;
		ss << "{x: " << x << ", y:" << y << "}";
		return scope.Close(String::New(ss.str().c_str()));
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Color Wrapper/Unwrapper.
	Handle<Value> ConstructColor(const Arguments& args) {
		HandleScope scope;

		if(args.Length() < 1) {
			return ThrowException(Exception::TypeError(String::New("Invalid call. Excpected: ConstructRect(x, y, width, height)")));
		}

		SDL_Color *color = new SDL_Color;
		color->r = args[0]->Int32Value();
		color->g = args[1]->Int32Value();
		color->b = args[2]->Int32Value();
		color->a = args[3]->Int32Value();

		return scope.Close(WrapColor(color));
	}

	Handle<Object> WrapColor(SDL_Color* color) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

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
	void SetColorRed(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->r = value->Int32Value();
		}
	}
	void SetColorGreen(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->g = value->Int32Value();
		}
	}
	void SetColorBlue(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->b = value->Int32Value();
		}
	}
	void SetColorAlpha(Local<String> name, Local<Value> value, const AccessorInfo& info) {
		if(!value->IsUndefined()) {
			SDL_Color* color = UnwrapColor(info.Holder());
			color->a = value->Int32Value();
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Palette Wrapper/Unwrapper.
	Handle<Value> ConstructPalette(const Arguments& args) {
		HandleScope scope;

		if(args.Length() < 1) {
			return ThrowException(Exception::TypeError(String::New("Invalid call. Excpected: ConstructPalette(Array)")));
		}

		Handle<Array> colors = Handle<Array>::Cast(args[0]);
		int length = colors->Length();
		SDL_Palette* palette = new SDL_Palette;
		palette->ncolors = length;
		palette->colors = new SDL_Color[length];
		for(int i = 0; i < length; i++) {
			Handle<Object> element = colors->CloneElementAt(i);
			SDL_Color* color = UnwrapColor(element);
			palette->colors[i] = *color;
		}

		return scope.Close(WrapPalette(palette));
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

		return scope.Close(result);
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

	///////////////////////////////////////////////////////////////////////////////
	// SDL_RendererInfo Wrapper/Unwrapper.
	Handle<Object> WrapRendererInfo(SDL_RendererInfo* info) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

		Handle<ObjectTemplate> templ = rendererinfo_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(info);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_RendererInfo* UnwrapRendererInfo(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_RendererInfo*>(ptr);
	}

	// Property getters.
	Handle<Value> GetRendererInfoName(Local<String> name, const AccessorInfo& info) {
		SDL_RendererInfo* rinfo = UnwrapRendererInfo(info.Holder());
		return String::New(rinfo->name);
	}
	Handle<Value> GetRendererInfoFlags(Local<String> name, const AccessorInfo& info) {
		SDL_RendererInfo* rinfo = UnwrapRendererInfo(info.Holder());
		return Number::New(rinfo->flags);
	}
	Handle<Value> GetRendererInfoNumTextureFormats(Local<String> name, const AccessorInfo& info) {
		SDL_RendererInfo* rinfo = UnwrapRendererInfo(info.Holder());
		return Number::New(rinfo->num_texture_formats);
	}
	Handle<Value> GetRendererInfoTextureFormats(Local<String> name, const AccessorInfo& info) {
		SDL_RendererInfo* rinfo = UnwrapRendererInfo(info.Holder());
		Handle<Array> ret = Array::New(rinfo->num_texture_formats);
		for(unsigned int i = 0; i < rinfo->num_texture_formats; i++) {
			ret->Set(i, Number::New(rinfo->texture_formats[i]));
		}
		return ret;
	}
	Handle<Value> GetRendererInfoMaxTextureWidth(Local<String> name, const AccessorInfo& info) {
		SDL_RendererInfo* rinfo = UnwrapRendererInfo(info.Holder());
		return Number::New(rinfo->max_texture_width);
	}
	Handle<Value> GetRendererInfoMaxTextureHeight(Local<String> name, const AccessorInfo& info) {
		SDL_RendererInfo* rinfo = UnwrapRendererInfo(info.Holder());
		return Number::New(rinfo->max_texture_height);
	}

	///////////////////////////////////////////////////////////////////////////////
	// SDL_Joystick Wrapper/Unwrapper.
	Handle<Object> WrapJoystick(SDL_Joystick* joystick) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

  		// Fetch the template for creating JavaScript http request wrappers.
  		// It only has to be created once, which we do on demand.
		Handle<ObjectTemplate> templ = joystick_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(joystick);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	SDL_Joystick* UnwrapJoystick(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<SDL_Joystick*>(ptr);
	}

	///////////////////////////////////////////////////////////////////////////////
	// TTF_Font Wrapper/Unwrapper.
	Handle<Object> WrapFont(TTF_Font* font) {
  		// Handle scope for temporary handles.
		HandleScope handle_scope;

  		// Fetch the template for creating JavaScript http request wrappers.
  		// It only has to be created once, which we do on demand.
		Handle<ObjectTemplate> templ = font_template_;

  		// Create an empty http request wrapper.
		Handle<Object> result = templ->NewInstance();

  		// Wrap the raw C++ pointer in an External so it can be referenced
  		// from within JavaScript.
		Handle<External> request_ptr = External::New(font);

  		// Store the request pointer in the JavaScript wrapper.
		result->SetInternalField(0, request_ptr);

  		// Return the result through the current handle scope.  Since each
  		// of these handles will go away when the handle scope is deleted
  		// we need to call Close to let one, the result, escape into the
  		// outer handle scope.
		return handle_scope.Close(result);
	}

	TTF_Font* UnwrapFont(Handle<Object> obj) {
		Handle<External> field = Handle<External>::Cast(obj->GetInternalField(0));
		void* ptr = field->Value();
		return static_cast<TTF_Font*>(ptr);
	}
}