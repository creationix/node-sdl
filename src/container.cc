#include "container.h"
#include <iostream>
#include <sstream>
#include "SDL.h"
#include "struct_wrappers.h"

using namespace v8;

Persistent<FunctionTemplate> sdl::ColorWrapper::wrap_template_;

sdl::ColorWrapper::ColorWrapper() {
}
sdl::ColorWrapper::~ColorWrapper() {
	std::cout << "ColorWrapper destructor running." << std::endl;
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
