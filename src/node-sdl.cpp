#include <v8.h>
#include <node.h>
#include <SDL.h>

using namespace v8;
using namespace node;

namespace node_sdl {

  static SDL_Surface* screen;

  static Handle<Value> Init(const Arguments& args) {
    HandleScope scope;
    SDL_Init( SDL_INIT_VIDEO );
    int w = (args[0]->Int32Value());
    int h = (args[1]->Int32Value());
    screen = SDL_SetVideoMode( w, h, 0, SDL_SWSURFACE );
    SDL_FillRect (screen, NULL, 0x000000);
    return Undefined();
  }

  static Handle<Value> Quit(const Arguments& args) {
    HandleScope scope;
    SDL_Quit();
    return Undefined();
  }
  
  static Handle<Value> Clear(const Arguments& args) {
    HandleScope scope;

    SDL_FillRect (screen, NULL, 0); 

    return Undefined();
  }

  static Handle<Value> FillRect(const Arguments& args) {
    HandleScope scope;

    SDL_Rect rect;
    rect.x = (args[0]->Int32Value());
    rect.y = (args[1]->Int32Value());
    rect.w = (args[2]->Int32Value());
    rect.h = (args[3]->Int32Value());

    int color = (args[4]->Int32Value());

    SDL_FillRect (screen, &rect, color); 

    return Undefined();
  }

  static Handle<Value> Flip(const Arguments& args) {
    HandleScope scope;
    SDL_Flip (screen);
    return Undefined();
  }
}

extern "C" void
init(Handle<Object> target)
{
  HandleScope scope;

  target->Set(String::New("hello"), String::New("World"));
  NODE_SET_METHOD(target, "init", node_sdl::Init);
  NODE_SET_METHOD(target, "quit", node_sdl::Quit);
  NODE_SET_METHOD(target, "fillRect", node_sdl::FillRect);
  NODE_SET_METHOD(target, "clear", node_sdl::Clear);
  NODE_SET_METHOD(target, "flip", node_sdl::Flip);

}


