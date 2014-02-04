#ifndef HELPERS_H_
#define HELPERS_H_

#include <v8.h>
#include <node.h>
#include <node_buffer.h>
#include <sstream>

#define S(x) #x
#ifdef ENABLE_ARG_CHECKING
#define CHECK_ARGLEN(function_name, num_args) \
	if(args.Length() < num_args) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected "; \
		ss << num_args; \
		ss << " arguments for function '"; \
		ss << S(function_name); \
		ss << "'."; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}

#define CHECK_STRING(arg_n) \
	if(!args[arg_n]->IsString()) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected argument "; \
		ss << arg_n; \
		ss << " to be a String."; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}
#define CHECK_STRING_F(arg_n, function_name) \
	if(!args[arg_n]->IsString()) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected argument "; \
		ss << arg_n; \
		ss << " to be a String. (function: "; \
		ss << S(function_name); \
		ss << ")"; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}

#define CHECK_NUMBER(arg_n) \
	if(!args[arg_n]->IsNumber()) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected argument "; \
		ss << arg_n; \
		ss << " to be a Number."; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}
#define CHECK_NUMBER_F(arg_n, function_name) \
	if(!args[arg_n]->IsNumber()) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected argument "; \
		ss << arg_n; \
		ss << " to be a Number. (function: "; \
		ss << S(function_name); \
		ss << ")"; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}

#define CHECK_BOOL(arg_n) \
	if(!args[arg_n]->IsBoolean()) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected argument "; \
		ss << arg_n; \
		ss << " to be a Boolean."; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}
#define CHECK_BOOL_F(arg_n, function_name) \
	if(!args[arg_n]->IsBoolean()) { \
		std::stringstream ss; \
		ss << "Invalid arguments: Expected argument "; \
		ss << arg_n; \
		ss << " to be a Boolean. (function: "; \
		ss << S(function_name); \
		ss << ")"; \
		return v8::ThrowException(v8::Exception::TypeError( \
			v8::String::New(ss.str().c_str()))); \
	}
#else
#define CHECK_ARGLEN(function_name, num_args)

#define CHECK_STRING(arg_n)
#define CHECK_STRING_F(arg_n, function_name)

#define CHECK_NUMBER(arg_n)
#define CHECK_NUMBER_F(arg_n, function_name)

#define CHECK_BOOL(arg_n)
#define CHECK_BOOL_F(arg_n, function_name)
#endif

#define FUNCTION_DEF(name) v8::Handle<v8::Value> name(const v8::Arguments& args)
#define FUNCTION_DEFP(prefix, name) v8::Handle<v8::Value> prefix::name(const v8::Arguments& args)
#define FUNCTION_BEGIN(name, num_args) FUNCTION_DEF(name) { \
	v8::HandleScope scope; \
	CHECK_ARGLEN(name, num_args);
#define FUNCTION_BEGINP(prefix, name, num_args) v8::Handle<v8::Value> prefix::name(const v8::Arguments& args) { \
	v8::HandleScope scope; \
	CHECK_ARGLEN(name, num_args);
#define FUNCTION_END(ret) return scope.Close(ret); \
}
#define FUNCTION_UNDEFINED return Undefined(); \
}

#define EXTRACT_STRING(name, arg_n) \
	CHECK_STRING(arg_n); \
	v8::String::Utf8Value name(args[arg_n])
#define EXTRACT_BOOL(name, arg_n) \
	CHECK_BOOL(arg_n); \
	bool name = args[arg_n]->BooleanValue()
#define EXTRACT_NUMBER(name, arg_n) \
	CHECK_NUMBER(arg_n); \
	float name = args[arg_n]->NumberValue()
#define EXTRACT_INT64(name, arg_n) \
	CHECK_NUMBER(arg_n); \
	int64_t name = args[arg_n]->IntegerValue()
#define EXTRACT_INT32(name, arg_n) \
	CHECK_NUMBER(arg_n); \
	int name = args[arg_n]->Int32Value()
#define EXTRACT_UINT32(name, arg_n) \
	CHECK_NUMBER(arg_n); \
	uint32_t name = args[arg_n]->Uint32Value()


namespace sdl {

  // Error reporting helpers
  v8::Handle<v8::Value> ThrowSDLException(const char* name);
  v8::Local<v8::Value> MakeSDLException(const char* name);
  
  // Helpers to work with buffers
  char* BufferData(node::Buffer *b);
  size_t BufferLength(node::Buffer *b);
  char* BufferData(v8::Local<v8::Object> buf_obj);
  size_t BufferLength(v8::Local<v8::Object> buf_obj);

  v8::Local<v8::Object> SDLDisplayModeToJavascriptObject(const SDL_DisplayMode& mode);

} // sdl

#endif  // HELPERS_H_
