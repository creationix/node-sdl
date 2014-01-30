#ifndef NODESDL_JOYSTICK_H
#define NODESDL_JOYSTICK_H

#include <v8.h>
#include <node.h>

#include "SDL.h"


namespace sdl {
	namespace joystick {
		void Init(v8::Handle<v8::Object> exports);
	}

	class JoystickWrapper : public node::ObjectWrap {
	public:
		static v8::Persistent<v8::FunctionTemplate> wrap_template_;

		JoystickWrapper();
		~JoystickWrapper();

		static void Init(v8::Handle<v8::Object> exports);
		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetAttached(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetAxis(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetBall(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetButton(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetGUID(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetHat(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetName(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetNumAxes(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetNumButtons(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetNumBalls(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetNumHats(const v8::Arguments& args);

		SDL_Joystick* joystick_;
	};

	v8::Handle<v8::Value> NumJoysticks(const v8::Arguments& args);

	v8::Handle<v8::Value> JoystickNameForIndex(const v8::Arguments& args);
	v8::Handle<v8::Value> JoystickGetDeviceGUID(const v8::Arguments& args);
	v8::Handle<v8::Value> JoystickGetGUIDFromString(const v8::Arguments& args);
	v8::Handle<v8::Value> JoystickGetGUIDString(const v8::Arguments& args);

	v8::Handle<v8::Value> JoystickUpdate(const v8::Arguments& args);
	v8::Handle<v8::Value> JoystickEventState(const v8::Arguments& args);
}

#endif