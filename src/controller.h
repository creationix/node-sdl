#ifndef NODESDL_CONTROLLER_H
#define NODESDL_CONTROLLER_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace controller {
		void Init(v8::Handle<v8::Object> exports);

		class GameControllerWrapper : public node::ObjectWrap {
		public:
			static v8::Persistent<v8::FunctionTemplate> wrap_template_;

			GameControllerWrapper();
			~GameControllerWrapper();

			static void Init(v8::Handle<v8::Object> exports);
			static v8::Handle<v8::Value> New(const v8::Arguments& args);

			static v8::Handle<v8::Value> GetAttached(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetAxis(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetBindForAxis(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetBindForButton(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetButton(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetJoystick(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetMapping(const v8::Arguments& args);
			static v8::Handle<v8::Value> GetName(const v8::Arguments& args);

			SDL_GameController* controller_;
		};

		v8::Handle<v8::Value> AddMapping(const v8::Arguments& args);
		v8::Handle<v8::Value> AddMappingsFromFile(const v8::Arguments& args);

		v8::Handle<v8::Value> MappingForGUID(const v8::Arguments& args);
		v8::Handle<v8::Value> NameForIndex(const v8::Arguments& args);

		v8::Handle<v8::Value> EventState(const v8::Arguments& args);
		v8::Handle<v8::Value> GetAxisFromString(const v8::Arguments& args);
		v8::Handle<v8::Value> GetButtonFromString(const v8::Arguments& args);
		v8::Handle<v8::Value> GetStringForAxis(const v8::Arguments& args);
		v8::Handle<v8::Value> GetStringForButton(const v8::Arguments& args);

		v8::Handle<v8::Value> Update(const v8::Arguments& args);
		v8::Handle<v8::Value> IsController(const v8::Arguments& args);
	}
}

#endif