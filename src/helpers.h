#ifndef HELPERS_H_
#define HELPERS_H_

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

namespace sdl {

  // Error reporting helpers
  v8::Handle<v8::Value> ThrowSDLException(const char* name);
  v8::Local<v8::Value> MakeSDLException(const char* name);
  
  // Helpers to work with buffers
  char* BufferData(node::Buffer *b);
  size_t BufferLength(node::Buffer *b);
  char* BufferData(v8::Local<v8::Object> buf_obj);
  size_t BufferLength(v8::Local<v8::Object> buf_obj);

  v8::Local<v8::Object> SDLEventToJavascriptObject(const SDL_Event& event);
  v8::Local<v8::Object> SDLDisplayModeToJavascriptObject(const SDL_DisplayMode& mode);

} // sdl

#endif  // HELPERS_H_
