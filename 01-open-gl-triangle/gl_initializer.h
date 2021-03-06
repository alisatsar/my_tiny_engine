#ifndef GL_INITIALIZER_H
#define GL_INITIALIZER_H

#include <exception>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

namespace te {

namespace gl {

PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;

template<class T>
void get_func_pointer(const char* func_name, T& result)
{
        void* gl_pointer(SDL_GL_GetProcAddress(func_name));
        if(gl_pointer == nullptr)
        {
                throw std::runtime_error("Can't load gl function");
        }
        result = reinterpret_cast<T>(gl_pointer);
}

void initializer_gl_functions()
{
    try
    {
        get_func_pointer("glGenBuffers", glGenBuffers);
        get_func_pointer("glBindBuffer", glBindBuffer);
        get_func_pointer("glBufferData", glBufferData);
        get_func_pointer("glCreateShader", glCreateShader);
        get_func_pointer("glShaderSource", glShaderSource);
        get_func_pointer("glCompileShader", glCompileShader);
        get_func_pointer("glGetShaderiv", glGetShaderiv);
        get_func_pointer("glGetShaderInfoLog", glGetShaderInfoLog);
        get_func_pointer("glCreateProgram", glCreateProgram);
        get_func_pointer("glAttachShader", glAttachShader);
        get_func_pointer("glGetProgramiv", glGetProgramiv);
        get_func_pointer("glGetProgramInfoLog", glGetProgramInfoLog);
        get_func_pointer("glLinkProgram", glLinkProgram);
        get_func_pointer("glUseProgram", glUseProgram);
        get_func_pointer("glDeleteShader", glDeleteShader);
        get_func_pointer("glVertexAttribPointer", glVertexAttribPointer);
        get_func_pointer("glBindAttribLocation", glBindAttribLocation);
        get_func_pointer("glEnableVertexAttribArray", glEnableVertexAttribArray);
        get_func_pointer("glGenVertexArrays", glGenVertexArrays);
        get_func_pointer("glBindVertexArray", glBindVertexArray);
    }
    catch(std::exception& ex)
    {
        ex.what();
    }
}

}//end namespace gl

}//end namespace te

#endif // GL_INITIALIZER_H
