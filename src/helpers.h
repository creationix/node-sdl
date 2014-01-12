#ifndef HELPERS_H_
#define HELPERS_H_

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace node;
using namespace v8;

namespace sdl {

  // Error reporting helpers
  Handle<Value> ThrowSDLException(const char* name);
  Local<Value> MakeSDLException(const char* name);
  
  // Helpers to work with buffers
  char* BufferData(Buffer *b);
  size_t BufferLength(Buffer *b);
  char* BufferData(Local<Object> buf_obj);
  size_t BufferLength(Local<Object> buf_obj);

  static Local<Object> SDLEventToJavascriptObject(const SDL_Event& event);
  static Local<Object> SDLDisplayModeToJavascriptObject(const SDL_DisplayMode& mode);

} // sdl

#endif  // HELPERS_H_
