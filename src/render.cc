#include "render.h"
#include "SDL.h"
#include "helpers.h"
#include "texture.h"
#include "surface.h"
#include "struct_wrappers.h"
#include "window.h"

using namespace v8;

////////////////////////////////////////////////////////////////////////////////
// RendererWrapper Class Definition.
Persistent<FunctionTemplate> sdl::RendererWrapper::render_wrap_template_;
Persistent<FunctionTemplate> sdl::RendererWrapper::software_render_wrap_template_;

sdl::RendererWrapper::RendererWrapper() {
}

sdl::RendererWrapper::~RendererWrapper() {
	if(NULL != renderer_) {
		SDL_DestroyRenderer(renderer_);
	}
}

void sdl::RendererWrapper::Init(Handle<Object> exports) {
  	// Setup hardware renderer construction.
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	render_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	render_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	render_wrap_template_->SetClassName(String::NewSymbol("RendererWrapper"));

	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getDrawBlendMode", GetDrawBlendMode);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getDrawColor", GetDrawColor);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getOutputSize", GetOutputSize);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getTarget", GetTarget);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getInfo", GetInfo);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getClipRect", GetClipRect);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getLogicalSize", GetLogicalSize);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getScale", GetScale);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "getViewport", GetViewport);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "targetSupported", TargetSupported);

	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setClipRect", SetClipRect);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setLogicalSize", SetLogicalSize);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setScale", SetScale);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setViewport", SetViewport);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setDrawBlendMode", SetDrawBlendMode);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setDrawColor", SetDrawColor);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "setTarget", SetTarget);

	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "clear", Clear);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "present", Present);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "copy", Copy);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "drawLine", DrawLine);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "drawLines", DrawLine);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "drawPoint", DrawPoint);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "drawRect", DrawRect);
	NODE_SET_PROTOTYPE_METHOD(render_wrap_template_, "fillRect", FillRect);

	exports->Set(String::NewSymbol("Renderer"), render_wrap_template_->GetFunction());

  // Setup software renderer construction.
	tpl = FunctionTemplate::New(NewSoftware);
	software_render_wrap_template_ = Persistent<FunctionTemplate>::New(tpl);

	software_render_wrap_template_->InstanceTemplate()->SetInternalFieldCount(1);
	software_render_wrap_template_->SetClassName(String::NewSymbol("SoftwareRendererWrapper"));

	exports->Set(String::NewSymbol("SoftwareRenderer"), software_render_wrap_template_->GetFunction());
}

Handle<Value> sdl::RendererWrapper::New(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of a Renderer.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid Arguments: Expected at least: new Renderer(sdl.Window)")));
	}
	else if(!args[0]->IsObject()) {
		return ThrowException(Exception::TypeError(String::New("Invalid Arguments: Expected at least: new Renderer(sdl.Window)")));
	}

	WindowWrapper* window = ObjectWrap::Unwrap<WindowWrapper>(Handle<Object>::Cast(args[0]));
	int index = args[1]->IsUndefined() ? -1 : args[1]->Int32Value();
	int flags = args[2]->IsUndefined() ? SDL_RENDERER_ACCELERATED : args[2]->Int32Value();
	SDL_Renderer* renderer = SDL_CreateRenderer(window->window_, index, flags);
	if(NULL == renderer) {
		return ThrowSDLException(__func__);
	}

	RendererWrapper* obj = new RendererWrapper();
	obj->renderer_ = renderer;
	obj->Wrap(args.This());
	return args.This();
}

Handle<Value> sdl::RendererWrapper::NewSoftware(const Arguments& args) {
	if(!args.IsConstructCall()) {
		return ThrowException(Exception::TypeError(
			String::New("Use the new operator to create instances of a Renderer.")));
	}

	HandleScope scope;

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid Arguments: Expected: new SoftwareRenderer(sdl.Surface)")));
	}
	else if(!args[0]->IsObject()) {
		return ThrowException(Exception::TypeError(String::New("Invalid Arguments: Expected: new SoftwareRenderer(sdl.Surface)")));
	}

	SurfaceWrapper* wrap = ObjectWrap::Unwrap<SurfaceWrapper>(Handle<Object>::Cast(args[0]));
	SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(wrap->surface_);
	if(NULL == renderer) {
		return ThrowSDLException(__func__);
	}

	RendererWrapper* obj = new RendererWrapper();
	obj->renderer_ = renderer;
	obj->Wrap(args.This());
	return args.This();
}

// Handle<Value> sdl::RendererWrapper::CreateTexture(const Arguments& args) {
//   HandleScope scope;
//   RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

//   return Undefined();
// }
// Handle<Value> sdl::RendererWrapper::CreateTextureFromSurface(const Arguments& args) {
//   HandleScope scope;
//   RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

//   return Undefined();
// }

Handle<Value> sdl::RendererWrapper::GetDrawBlendMode(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<sdl::RendererWrapper>(args.This());
	SDL_BlendMode mode;
	int err = SDL_GetRenderDrawBlendMode(obj->renderer_, &mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return scope.Close(Number::New(mode));
}

Handle<Value> sdl::RendererWrapper::GetDrawColor(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());
	uint8_t r, g, b, a;
	int err = SDL_GetRenderDrawColor(obj->renderer_, &r, &g, &b, &a);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	SDL_Color* color = new SDL_Color;
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = a;

	return scope.Close(WrapColor(color));
}

Handle<Value> sdl::RendererWrapper::GetTarget(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	SDL_Texture* texture = SDL_GetRenderTarget(obj->renderer_);
	if(NULL == texture) {
		return Null();
	}

	Handle<Object> toWrap = Object::New();
	TextureWrapper* texWrap = new TextureWrapper(toWrap);
	texWrap->texture_ = texture;
	return scope.Close(toWrap);
}

Handle<Value> sdl::RendererWrapper::GetInfo(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	SDL_RendererInfo* info = new SDL_RendererInfo;
	int err = SDL_GetRendererInfo(obj->renderer_, info);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	return scope.Close(WrapRendererInfo(info));
}

Handle<Value> sdl::RendererWrapper::GetOutputSize(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<sdl::RendererWrapper>(args.This());

	int w, h;
	int err = SDL_GetRendererOutputSize(obj->renderer_, &w, &h);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}
	Local<Array> ret = Array::New(2);
	ret->Set(0, Number::New(w));
	ret->Set(1, Number::New(h));
	return scope.Close(ret);
}

Handle<Value> sdl::RendererWrapper::GetClipRect(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	SDL_Rect* rect = new SDL_Rect;
	SDL_RenderGetClipRect(obj->renderer_, rect);

	return scope.Close(WrapRect(rect));
}

Handle<Value> sdl::RendererWrapper::GetLogicalSize(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	int w, h;
	SDL_RenderGetLogicalSize(obj->renderer_, &w, &h);
	Handle<Array> ret = Array::New(2);
	ret->Set(0, Number::New(w));
	ret->Set(1, Number::New(h));

	return scope.Close(ret);
}
Handle<Value> sdl::RendererWrapper::GetScale(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	float scaleX, scaleY;
	SDL_RenderGetScale(obj->renderer_, &scaleX, &scaleY);
	Handle<Array> ret = Array::New(2);
	ret->Set(0, Number::New(scaleX));
	ret->Set(1, Number::New(scaleY));

	return scope.Close(ret);
}
Handle<Value> sdl::RendererWrapper::GetViewport(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	SDL_Rect* rect = new SDL_Rect;
	SDL_RenderGetViewport(obj->renderer_, rect);

	return scope.Close(WrapRect(rect));
}
// TODO: Implement.
Handle<Value> sdl::RendererWrapper::ReadPixels(const Arguments& args) {
	HandleScope scope;
	// RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	return ThrowException(Exception::Error(String::New("Not implemented.")));
}
Handle<Value> sdl::RendererWrapper::TargetSupported(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	return scope.Close(Boolean::New(SDL_RenderTargetSupported(obj->renderer_)));
}

Handle<Value> sdl::RendererWrapper::SetClipRect(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	const SDL_Rect* clip = args[0]->IsUndefined() ? NULL : UnwrapRect(Handle<Object>::Cast(args[0]));
	int err = SDL_RenderSetClipRect(obj->renderer_, clip);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::SetLogicalSize(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected setLogicalSize(Number, Number)")));
	}
	int w = args[0]->IsUndefined() ? 1 : args[0]->Int32Value();
	int h = args[1]->IsUndefined() ? 1 : args[1]->Int32Value();
	int err = SDL_RenderSetLogicalSize(obj->renderer_, w, h);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::SetScale(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected setLogicalSize(Number, Number)")));
	}
	double scaleX = args[0]->IsUndefined() ? 1 : args[0]->NumberValue();
	double scaleY = args[1]->IsUndefined() ? 1 : args[1]->NumberValue();
	int err = SDL_RenderSetLogicalSize(obj->renderer_, scaleX, scaleY);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::SetViewport(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	const SDL_Rect* viewport = args[0]->IsUndefined() ? NULL : UnwrapRect(Handle<Object>::Cast(args[0]));
	int err = SDL_RenderSetViewport(obj->renderer_, viewport);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::SetDrawBlendMode(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	SDL_BlendMode mode = args[0]->IsUndefined() ? SDL_BLENDMODE_NONE : static_cast<SDL_BlendMode>(args[0]->Int32Value());
	int err = SDL_SetRenderDrawBlendMode(obj->renderer_, mode);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::SetDrawColor(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 4) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected setDrawColor(Number, Number, Number, Number)")));
	}
	int r = args[0]->IsUndefined() ? 0 : args[0]->Int32Value();
	int g = args[1]->IsUndefined() ? 0 : args[1]->Int32Value();
	int b = args[2]->IsUndefined() ? 0 : args[2]->Int32Value();
	int a = args[3]->IsUndefined() ? 0 : args[4]->Int32Value();
	int err = SDL_SetRenderDrawColor(obj->renderer_, r, g, b, a);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::SetTarget(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected setTarget(Texture)")));
	}
	TextureWrapper* tex = ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(args[0]));
	int err = SDL_SetRenderTarget(obj->renderer_, tex->texture_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}

Handle<Value> sdl::RendererWrapper::Clear(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	int err = SDL_RenderClear(obj->renderer_);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::Present(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());
	SDL_RenderPresent(obj->renderer_);

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::Copy(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() > 3) {
		TextureWrapper* texture = ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(args[0]));
		SDL_Rect* src = args[1]->IsUndefined() ? NULL : UnwrapRect(Handle<Object>::Cast(args[1]));
		SDL_Rect* dst = args[2]->IsUndefined() ? NULL : UnwrapRect(Handle<Object>::Cast(args[2]));
		double angle = args[3]->IsUndefined() ? 0 : args[3]->NumberValue();
		PointWrapper* point = args[4]->IsUndefined() ? NULL : ObjectWrap::Unwrap<PointWrapper>(Handle<Object>::Cast(args[4]));
		SDL_RendererFlip flip = args[5]->IsUndefined() ? SDL_FLIP_NONE : static_cast<SDL_RendererFlip>(args[5]->Int32Value());
		int err = SDL_RenderCopyEx(obj->renderer_, texture->texture_, src, dst, angle, point->point_, flip);
		if(err < 0) {
			return ThrowSDLException(__func__);
		}
	}
	else {
		TextureWrapper* texture = ObjectWrap::Unwrap<TextureWrapper>(Handle<Object>::Cast(args[0]));
		SDL_Rect* src = args[1]->IsUndefined() ? NULL : UnwrapRect(Handle<Object>::Cast(args[1]));
		SDL_Rect* dst = args[2]->IsUndefined() ? NULL : UnwrapRect(Handle<Object>::Cast(args[2]));
		int err = SDL_RenderCopy(obj->renderer_, texture->texture_, src, dst);
		if(err < 0) {
			return ThrowSDLException(__func__);
		}
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::DrawLine(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 4) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawLine(Number, Number, Number, Number)")));
	}

	int x1 = args[0]->Int32Value();
	int y1 = args[1]->Int32Value();
	int x2 = args[2]->Int32Value();
	int y2 = args[3]->Int32Value();
	int err = SDL_RenderDrawLine(obj->renderer_, x1, y1, x2, y2);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::DrawLines(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawLines(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(args[0]);
	const int numPoints = arr->Length();
	SDL_Point* points = new SDL_Point[numPoints];
	for(int i = 0; i < numPoints; i++) {
		PointWrapper* wrap = ObjectWrap::Unwrap<PointWrapper>(Handle<Object>::Cast(arr->Get(0)));
		points[i] = *wrap->point_;
	}
	int err = SDL_RenderDrawLines(obj->renderer_, points, numPoints);
	delete points;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::DrawPoint(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 2) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawPoint(Point)")));
	}

	int x = args[0]->Int32Value();
	int y = args[1]->Int32Value();
	int err = SDL_RenderDrawPoint(obj->renderer_, x, y);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::DrawPoints(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawPoints(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(args[0]);
	int numPoints = arr->Length();
	SDL_Point* points = new SDL_Point[numPoints];
	for(int i = 0; i < numPoints; i++) {
		PointWrapper* point = ObjectWrap::Unwrap<PointWrapper>(Handle<Object>::Cast(arr->Get(i)));
		points[i] = *point->point_;
	}
	int err = SDL_RenderDrawPoints(obj->renderer_, points, numPoints);
	delete points;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::DrawRect(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawRect(Rect)")));
	}

	SDL_Rect* rect = UnwrapRect(Handle<Object>::Cast(args[0]));
	int err = SDL_RenderDrawRect(obj->renderer_, rect);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::DrawRects(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawRects(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(args[0]);
	int numRects = arr->Length();
	SDL_Rect* rects = new SDL_Rect[numRects];
	for(int i = 0; i < numRects; i++) {
		SDL_Rect* rect = UnwrapRect(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *rect;
	}
	int err = SDL_RenderDrawRects(obj->renderer_, rects, numRects);
	delete rects;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::FillRect(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawRect(Rect)")));
	}

	SDL_Rect* rect = UnwrapRect(Handle<Object>::Cast(args[0]));
	int err = SDL_RenderFillRect(obj->renderer_, rect);
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
Handle<Value> sdl::RendererWrapper::FillRects(const Arguments& args) {
	HandleScope scope;
	RendererWrapper* obj = ObjectWrap::Unwrap<RendererWrapper>(args.This());

	if(args.Length() < 1) {
		return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected drawRects(Array)")));
	}

	Handle<Array> arr = Handle<Array>::Cast(args[0]);
	int numRects = arr->Length();
	SDL_Rect* rects = new SDL_Rect[numRects];
	for(int i = 0; i < numRects; i++) {
		SDL_Rect* rect = UnwrapRect(Handle<Object>::Cast(arr->Get(i)));
		rects[i] = *rect;
	}
	int err = SDL_RenderFillRects(obj->renderer_, rects, numRects);
	delete rects;
	if(err < 0) {
		return ThrowSDLException(__func__);
	}

	return Undefined();
}
