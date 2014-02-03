#include "container.h"
#include <iostream>
#include <sstream>
#include "SDL.h"
#include "struct_wrappers.h"

using namespace v8;

Persistent<FunctionTemplate> sdl::RectWrapper::wrap_template_;

sdl::RectWrapper::RectWrapper() {
}
sdl::RectWrapper::RectWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}
sdl::RectWrapper::~RectWrapper() {
	if(NULL != rect_) {
		delete rect_;
	}
}

void sdl::RectWrapper::Init(Handle<Object> exports) {
	Handle<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::New("RectWrapper"));

	Local<ObjectTemplate> templ = wrap_template_->PrototypeTemplate();
	templ->SetAccessor(String::NewSymbol("x"), GetX, SetX);
	templ->SetAccessor(String::NewSymbol("y"), GetY, SetY);
	templ->SetAccessor(String::NewSymbol("w"), GetW, SetW);
	templ->SetAccessor(String::NewSymbol("h"), GetH, SetH);

	exports->Set(String::NewSymbol("Rect"), wrap_template_->GetFunction());
}
Handle<Value> sdl::RectWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Cannot construct a Rect without using the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 4) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected new sdl.Rect(Number, Number, Number, Number)")));
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();
	int w = args[2]->Int32Value();
	int h = args[3]->Int32Value();
	SDL_Rect* rect = new SDL_Rect;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;

	RectWrapper* wrap = new RectWrapper();
	wrap->rect_ = rect;
	wrap->Wrap(args.This());

	return args.This();
}

Handle<Value> sdl::RectWrapper::GetX(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(info.This());

	return scope.Close(Number::New(wrap->rect_->x));
}
Handle<Value> sdl::RectWrapper::GetY(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(info.This());

	return scope.Close(Number::New(wrap->rect_->y));
}
Handle<Value> sdl::RectWrapper::GetW(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(info.This());

	return scope.Close(Number::New(wrap->rect_->w));
}
Handle<Value> sdl::RectWrapper::GetH(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(info.This());

	return scope.Close(Number::New(wrap->rect_->h));
}

void sdl::RectWrapper::SetX(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	int x = value->Int32Value();
	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info.This()));
	wrap->rect_->x = x;
}
void sdl::RectWrapper::SetY(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	int y = value->Int32Value();
	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info.This()));
	wrap->rect_->y = y;
}
void sdl::RectWrapper::SetW(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	int w = value->Int32Value();
	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info.This()));
	wrap->rect_->w = w;
}
void sdl::RectWrapper::SetH(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	int h = value->Int32Value();
	RectWrapper* wrap = ObjectWrap::Unwrap<RectWrapper>(Handle<Object>::Cast(info.This()));
	wrap->rect_->h = h;
}

Persistent<FunctionTemplate> sdl::ColorWrapper::wrap_template_;

sdl::ColorWrapper::ColorWrapper() {
}
sdl::ColorWrapper::~ColorWrapper() {
	// std::cout << "ColorWrapper destructor running." << std::endl;
	if(NULL != color_) {
		delete color_;
	}
}

void sdl::ColorWrapper::Init(Handle<Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("ColorWrapper"));

	Local<ObjectTemplate> templ = wrap_template_->PrototypeTemplate();
	templ->SetAccessor(String::NewSymbol("r"), GetRed, SetRed);
	templ->SetAccessor(String::NewSymbol("g"), GetGreen, SetGreen);
	templ->SetAccessor(String::NewSymbol("b"), GetBlue, SetBlue);
	templ->SetAccessor(String::NewSymbol("a"), GetAlpha, SetAlpha);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getColor", GetColor);
	NODE_SET_PROTOTYPE_METHOD(tpl, "toString", ToString);

	exports->Set(String::NewSymbol("Color"), wrap_template_->GetFunction());
}
Handle<Value> sdl::ColorWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instance of a Color.")));
	}

	HandleScope scope;

	if(args.Length() < 3) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected new sdl.Color(Number, Number, Number[, Number])")));
	}

	uint8_t r = static_cast<uint8_t>(args[0]->Int32Value());
	uint8_t g = static_cast<uint8_t>(args[1]->Int32Value());
	uint8_t b = static_cast<uint8_t>(args[2]->Int32Value());
	uint8_t a = args[3]->IsUndefined() ? 255 : static_cast<uint8_t>(args[3]->Int32Value());
	SDL_Color* color = new SDL_Color;
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;

	ColorWrapper* obj = new ColorWrapper();
	obj->color_ = color;
	obj->Wrap(args.This());
	return args.This();
}

Handle<Value> sdl::ColorWrapper::GetRed(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	return scope.Close(Number::New(obj->color_->r));
}
Handle<Value> sdl::ColorWrapper::GetGreen(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	return scope.Close(Number::New(obj->color_->g));
}
Handle<Value> sdl::ColorWrapper::GetBlue(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	return scope.Close(Number::New(obj->color_->b));
}
Handle<Value> sdl::ColorWrapper::GetAlpha(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	return scope.Close(Number::New(obj->color_->a));
}
Handle<Value> sdl::ColorWrapper::GetColor(const Arguments& args) {
	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid argument: Expected GetColor(PixelFormat)")));
	}

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(args.This());
	SDL_PixelFormat* format = UnwrapPixelFormat(Handle<Object>::Cast(args[0]));
	SDL_Color* c = obj->color_;
	uint32_t color = SDL_MapRGBA(format, c->r, c->g, c->b, c->a);
	return scope.Close(Number::New(color));
}

void sdl::ColorWrapper::SetRed(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	uint8_t r = static_cast<uint8_t>(value->Int32Value());
	obj->color_->r = r;
}
void sdl::ColorWrapper::SetGreen(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	uint8_t g = static_cast<uint8_t>(value->Int32Value());
	obj->color_->g = g;
}
void sdl::ColorWrapper::SetBlue(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	uint8_t b = static_cast<uint8_t>(value->Int32Value());
	obj->color_->b = b;
}
void sdl::ColorWrapper::SetAlpha(Local<String> name, Local<Value> value, const AccessorInfo& info) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(info.This());
	uint8_t a = static_cast<uint8_t>(value->Int32Value());
	obj->color_->a = a;
}

Handle<Value> sdl::ColorWrapper::ToString(const Arguments& args) {
	HandleScope scope;

	ColorWrapper* obj = ObjectWrap::Unwrap<ColorWrapper>(args.This());
	SDL_Color* c = obj->color_;
	std::stringstream ss;
	ss << "{r:" << (int)c->r << ", g:" << (int)c->g << ", b:" << (int)c->b << ", a:" << (int)c->a << "}";
	return scope.Close(String::New(ss.str().c_str()));
}

////////////////////////////////////////////////////////////////////////////////
// FingerWrapper Class Definition.
v8::Persistent<v8::FunctionTemplate> sdl::FingerWrapper::wrap_template_;

sdl::FingerWrapper::FingerWrapper() {
}
sdl::FingerWrapper::FingerWrapper(Handle<Object> toWrap) {
	Wrap(toWrap);
}
sdl::FingerWrapper::~FingerWrapper() {
	std::cout << "FingerWrapper destructor running." << std::endl;
	if(NULL != finger_) {
		delete finger_;
	}
}

void sdl::FingerWrapper::Init(v8::Handle<v8::Object> exports) {
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	wrap_template_->SetClassName(String::NewSymbol("FingerWrapper"));

	Local<ObjectTemplate> templ = wrap_template_->PrototypeTemplate();
	templ->SetAccessor(String::NewSymbol("fingerID"), GetFingerID);
	templ->SetAccessor(String::NewSymbol("x"), GetX);
	templ->SetAccessor(String::NewSymbol("y"), GetY);
	templ->SetAccessor(String::NewSymbol("pressure"), GetPressure);

	exports->Set(String::NewSymbol("Finger"), wrap_template_->GetFunction());
}
Handle<Value> sdl::FingerWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Can only construct a FingerWrapper with the new operator.")));
	}

	HandleScope scope;

	if(args.Length() < 4) {
		return ThrowException(Exception::TypeError(
			String::New("Invalid arguments: Expected new sdl.Finger(Number, Number, Number, Number)")));
	}

	SDL_FingerID id = static_cast<SDL_FingerID>(args[0]->IntegerValue());
	float x = static_cast<float>(args[1]->NumberValue());
	float y = static_cast<float>(args[2]->NumberValue());
	float pressure = static_cast<float>(args[3]->NumberValue());

	SDL_Finger* finger = new SDL_Finger;
	finger->id = id;
	finger->x = x;
	finger->y = y;
	finger->pressure = pressure;

	FingerWrapper* obj = new FingerWrapper();
	obj->finger_ = finger;
	obj->Wrap(args.This());

	return args.This();
}

Handle<Value> sdl::FingerWrapper::GetFingerID(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	FingerWrapper* obj = ObjectWrap::Unwrap<FingerWrapper>(info.This());

	return scope.Close(Number::New(obj->finger_->id));
}
Handle<Value> sdl::FingerWrapper::GetX(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	FingerWrapper* obj = ObjectWrap::Unwrap<FingerWrapper>(info.This());

	return scope.Close(Number::New(obj->finger_->x));
}
Handle<Value> sdl::FingerWrapper::GetY(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	FingerWrapper* obj = ObjectWrap::Unwrap<FingerWrapper>(info.This());

	return scope.Close(Number::New(obj->finger_->y));
}
Handle<Value> sdl::FingerWrapper::GetPressure(Local<String> name, const AccessorInfo& info) {
	HandleScope scope;

	FingerWrapper* obj = ObjectWrap::Unwrap<FingerWrapper>(info.This());

	return scope.Close(Number::New(obj->finger_->pressure));
}
