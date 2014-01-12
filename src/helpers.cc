#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "helpers.h"
#include "struct_wrappers.h"

namespace sdl {

// Helper for formatting error exceptions
Handle<Value> ThrowSDLException(const char* name) {
  return ThrowException(MakeSDLException(name));
}

Local<Value> MakeSDLException(const char* name) {
  return Exception::Error(String::Concat(
    String::Concat(String::New(name), String::New(": ")),
    String::New(SDL_GetError())
  ));
}



// Wrap/Unwrap SDL_Palette

static Persistent<ObjectTemplate> palette_template_;

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

  // Fetch the template for creating JavaScript http request wrappers.
  // It only has to be created once, which we do on demand.
  if (palette_template_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = MakePaletteTemplate();
    palette_template_ = Persistent<ObjectTemplate>::New(raw_template);
  }
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

// Wrap/Unwrap PixelFormat

static Persistent<ObjectTemplate> pixelformat_template_;

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

  // Fetch the template for creating JavaScript http request wrappers.
  // It only has to be created once, which we do on demand.
  if (pixelformat_template_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = MakePixelFormatTemplate();
    pixelformat_template_ = Persistent<ObjectTemplate>::New(raw_template);
  }
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

// Wrap/Unwrap Joystick

static Persistent<ObjectTemplate> joystick_template_;

//Handle<Value> GetJoystickFlags(Local<String> name, const AccessorInfo& info) {
//  SDL_Joystick* joystick = UnwrapJoystick(info.Holder());
//  return Number::New(joystick->flags);
//}
//Handle<Value> GetJoystickFormat(Local<String> name, const AccessorInfo& info) {
//  HandleScope scope;
//  SDL_Joystick* joystick = UnwrapJoystick(info.Holder());
//  return scope.Close(WrapPixelFormat(joystick->format));
//}
//Handle<Value> GetJoystickWidth(Local<String> name, const AccessorInfo& info) {
//  SDL_Joystick* joystick = UnwrapJoystick(info.Holder());
//  return Number::New(joystick->w);
//}
//Handle<Value> GetJoystickHeight(Local<String> name, const AccessorInfo& info) {
//  SDL_Joystick* joystick = UnwrapJoystick(info.Holder());
//  return Number::New(joystick->h);
//}
//Handle<Value> GetJoystickPitch(Local<String> name, const AccessorInfo& info) {
//  SDL_Joystick* joystick = UnwrapJoystick(info.Holder());
//  return Number::New(joystick->pitch);
//}
//Handle<Value> GetJoystickRect(Local<String> name, const AccessorInfo& info) {
//  HandleScope scope;
//  SDL_Joystick* joystick = UnwrapJoystick(info.Holder());
//  return scope.Close(WrapRect(&joystick->clip_rect));
//}

Handle<ObjectTemplate> MakeJoystickTemplate() {
  HandleScope handle_scope;

  Handle<ObjectTemplate> result = ObjectTemplate::New();
  result->SetInternalFieldCount(1);

  // Add accessors for some of the fields of the joystick.
//  result->SetAccessor(String::NewSymbol("flags"), GetJoystickFlags);
//  result->SetAccessor(String::NewSymbol("format"), GetJoystickFormat);
//  result->SetAccessor(String::NewSymbol("w"), GetJoystickWidth);
//  result->SetAccessor(String::NewSymbol("h"), GetJoystickHeight);
//  result->SetAccessor(String::NewSymbol("pitch"), GetJoystickPitch);
//  result->SetAccessor(String::NewSymbol("clip_rect"), GetJoystickRect);

  // Again, return the result through the current handle scope.
  return handle_scope.Close(result);
}

Handle<Object> WrapJoystick(SDL_Joystick* joystick) {
  // Handle scope for temporary handles.
  HandleScope handle_scope;

  // Fetch the template for creating JavaScript http request wrappers.
  // It only has to be created once, which we do on demand.
  if (joystick_template_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = MakeJoystickTemplate();
    joystick_template_ = Persistent<ObjectTemplate>::New(raw_template);
  }
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

// Wrap/Unwrap Font

static Persistent<ObjectTemplate> font_template_;

//Handle<Value> GetFontFlags(Local<String> name, const AccessorInfo& info) {
//  TTF_Font* font = UnwrapFont(info.Holder());
//  return Number::New(font->flags);
//}
//Handle<Value> GetFontFormat(Local<String> name, const AccessorInfo& info) {
//  HandleScope scope;
//  TTF_Font* font = UnwrapFont(info.Holder());
//  return scope.Close(WrapPixelFormat(font->format));
//}
//Handle<Value> GetFontWidth(Local<String> name, const AccessorInfo& info) {
//  TTF_Font* font = UnwrapFont(info.Holder());
//  return Number::New(font->w);
//}
//Handle<Value> GetFontHeight(Local<String> name, const AccessorInfo& info) {
//  TTF_Font* font = UnwrapFont(info.Holder());
//  return Number::New(font->h);
//}
//Handle<Value> GetFontPitch(Local<String> name, const AccessorInfo& info) {
//  TTF_Font* font = UnwrapFont(info.Holder());
//  return Number::New(font->pitch);
//}
//Handle<Value> GetFontRect(Local<String> name, const AccessorInfo& info) {
//  HandleScope scope;
//  TTF_Font* font = UnwrapFont(info.Holder());
//  return scope.Close(WrapRect(&font->clip_rect));
//}

Handle<ObjectTemplate> MakeFontTemplate() {
  HandleScope handle_scope;

  Handle<ObjectTemplate> result = ObjectTemplate::New();
  result->SetInternalFieldCount(1);

  // Add accessors for some of the fields of the font.
//  result->SetAccessor(String::NewSymbol("flags"), GetFontFlags);
//  result->SetAccessor(String::NewSymbol("format"), GetFontFormat);
//  result->SetAccessor(String::NewSymbol("w"), GetFontWidth);
//  result->SetAccessor(String::NewSymbol("h"), GetFontHeight);
//  result->SetAccessor(String::NewSymbol("pitch"), GetFontPitch);
//  result->SetAccessor(String::NewSymbol("clip_rect"), GetFontRect);

  // Again, return the result through the current handle scope.
  return handle_scope.Close(result);
}

Handle<Object> WrapFont(TTF_Font* font) {
  // Handle scope for temporary handles.
  HandleScope handle_scope;

  // Fetch the template for creating JavaScript http request wrappers.
  // It only has to be created once, which we do on demand.
  if (font_template_.IsEmpty()) {
    Handle<ObjectTemplate> raw_template = MakeFontTemplate();
    font_template_ = Persistent<ObjectTemplate>::New(raw_template);
  }
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


char* BufferData(Buffer *b) {
  return Buffer::Data(b->handle_);
}

size_t BufferLength(Buffer *b) {
  return Buffer::Length(b->handle_);
}

char* BufferData(Local<Object> buf_obj) {
  return Buffer::Data(buf_obj);
}

size_t BufferLength(Local<Object> buf_obj) {
  return Buffer::Length(buf_obj);
}

static Local<Object> SDLEventToJavascriptObject(const SDL_Event& event) {
  Local<Object> evt = Object::New();

  switch (event.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      evt->Set(String::New("type"), String::New(event.type == SDL_KEYDOWN ? "KEYDOWN" : "KEYUP"));
      evt->Set(String::New("scancode"), Number::New(event.key.keysym.scancode));
      evt->Set(String::New("sym"), Number::New(event.key.keysym.sym));
      evt->Set(String::New("mod"), Number::New(event.key.keysym.mod));
      break;
    case SDL_MOUSEMOTION:
      evt->Set(String::New("type"), String::New("MOUSEMOTION"));
      evt->Set(String::New("state"), Number::New(event.motion.state));
      evt->Set(String::New("which"), Number::New(event.motion.which));
      evt->Set(String::New("x"), Number::New(event.motion.x));
      evt->Set(String::New("y"), Number::New(event.motion.y));
      evt->Set(String::New("xrel"), Number::New(event.motion.xrel));
      evt->Set(String::New("yrel"), Number::New(event.motion.yrel));
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      evt->Set(String::New("type"), String::New(event.type == SDL_MOUSEBUTTONDOWN ? "MOUSEBUTTONDOWN" : "MOUSEBUTTONUP"));
      evt->Set(String::New("button"), Number::New(event.button.button));
      evt->Set(String::New("which"), Number::New(event.button.which));
      evt->Set(String::New("x"), Number::New(event.button.x));
      evt->Set(String::New("y"), Number::New(event.button.y));
      break;
    case SDL_JOYAXISMOTION:
      evt->Set(String::New("type"), String::New("JOYAXISMOTION"));
      evt->Set(String::New("which"), Number::New(event.jaxis.which));
      evt->Set(String::New("axis"), Number::New(event.jaxis.axis));
      evt->Set(String::New("value"), Number::New(event.jaxis.value));
      break;
    case SDL_JOYBALLMOTION:
      evt->Set(String::New("type"), String::New("JOYBALLMOTION"));
      evt->Set(String::New("which"), Number::New(event.jball.which));
      evt->Set(String::New("ball"), Number::New(event.jball.ball));
      evt->Set(String::New("xrel"), Number::New(event.jball.xrel));
      evt->Set(String::New("yrel"), Number::New(event.jball.yrel));
      break;
    case SDL_JOYHATMOTION:
      evt->Set(String::New("type"), String::New("JOYHATMOTION"));
      evt->Set(String::New("which"), Number::New(event.jhat.which));
      evt->Set(String::New("hat"), Number::New(event.jhat.hat));
      evt->Set(String::New("value"), Number::New(event.jhat.value));
      break;
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      evt->Set(String::New("type"), String::New(event.type == SDL_JOYBUTTONDOWN ? "JOYBUTTONDOWN" : "JOYBUTTONUP"));
      evt->Set(String::New("which"), Number::New(event.jbutton.which));
      evt->Set(String::New("button"), Number::New(event.jbutton.button));
      break;
    case SDL_QUIT:
      evt->Set(String::New("type"), String::New("QUIT"));
      break;
    default:
      evt->Set(String::New("type"), String::New("UNKNOWN"));
      evt->Set(String::New("typeCode"), Number::New(event.type));
      break;
  }

  return evt;
}

static Local<Object> SDLDisplayModeToJavascriptObject(const SDL_DisplayMode& mode) {
  Local<Object> jsMode = Object::New();
  jsMode->Set(String::New("format"), Number::New(mode.format));
  jsMode->Set(String::New("w"), Number::New(mode.w));
  jsMode->Set(String::New("h"), Number::New(mode.h));
  jsMode->Set(String::New("refreshRate"), Number::New(mode.refresh_rate));
  return jsMode;
}

} // node_sdl
