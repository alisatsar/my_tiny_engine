#include "engine.hxx"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string_view>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

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

template<class T>
void get_func_pointer(const char* func_name, T& result)
{
	///to get an OpenGL function by name
	void* gl_pointer(SDL_GL_GetProcAddress(func_name));
	if(gl_pointer == nullptr)
	{
		throw std::runtime_error("Can't load gl function");
	}
	result = reinterpret_cast<T>(gl_pointer);
}

struct bind
{
	SDL_Keycode key;
	std::string_view name;
	te::event pressed;
	te::event released;
};

const std::array<bind, 8> keys{
    { { SDLK_w, "W", te::event::up_pressed, te::event::up_released },
      { SDLK_a, "A", te::event::left_pressed, te::event::left_released },
      { SDLK_s, "S", te::event::down_pressed, te::event::down_released },
      { SDLK_d, "D", te::event::right_pressed, te::event::right_released },
      { SDLK_LCTRL, "button1", te::event::button_a_pressed,
    		  te::event::button_a_pressed },
      { SDLK_SPACE, "button2", te::event::button_b_pressed,
    		  te::event::button_b_pressed },
      { SDLK_ESCAPE, "select", te::event::select_pressed, te::event::select_released },
      { SDLK_RETURN, "start", te::event::start_pressed, te::event::start_released } }
};

bool check_input(const SDL_Event& e, const bind*& result)
{
	auto it = std::find_if(std::begin(keys), std::end(keys),[&](const ::bind& b) {
	        return b.key == e.key.keysym.sym;
		} );

	if(it != std::end(keys))
	{
		result = it;
		return true;
	}
	return true;
}

class my_tiny_engine : public te::engine
{
private:
	SDL_GLContext gl_context;
	SDL_Window* window = nullptr;

public:
	std::string check_version() final
	{
		std::string s;
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);

		bool result = SDL_VERSIONNUM(compiled.major, compiled.minor, compiled.patch) ==
				SDL_VERSIONNUM(linked.major, linked.minor, linked.patch);

		if(!result)
		{
			s = "WARNING: The compiled version is not equal linked version!";
		}
		else
		{
			s = "SUCCESS: The compiled version is equal linked version!";
		}
		return s;
	}

	bool initialize() final
	{
		const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
		if(init_result != 0)
		{
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}
		return true;
	}

	bool create_window(const char* title, int pos_x, int pos_y, int width, int height)
	{
		window = SDL_CreateWindow(
				title,
				pos_x,
				pos_y,
				height,
				width,
				SDL_WINDOW_OPENGL
				);

		if(window == NULL)
		{
			SDL_Log("Could not create window: %s\n", SDL_GetError());
			SDL_Quit();
			return false;
		}

        gl_context = SDL_GL_CreateContext(window);
		if(gl_context == NULL)
		{
			std::cout << "GLContext error";
		}
		assert(gl_context != nullptr);

		int gl_major_ver = 0;
		int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
		assert (result == 0);
		int gl_minor_ver = 0;
		int result2 = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
		assert (result2 == 0);

        if (gl_major_ver <= 2 && gl_minor_ver < 1)
		{
			std::clog << "current context opengl version: " << gl_major_ver
			    << '.' << gl_minor_ver << '\n'<< "need opengl version at least: 2.1\n"
			                      << std::flush;
			throw std::runtime_error("opengl version too low");
		}
		return true;
	}

	bool read_input(te::event& e)
	{
		SDL_Event sdl_event;

		if(SDL_PollEvent(&sdl_event))
		{
			const bind* binding = nullptr;

			if(sdl_event.type == SDL_QUIT)
			{
				e = te::event::turn_off;
				return true;
			}
			else if(sdl_event.type == SDL_KEYUP)
			{
				if(check_input(sdl_event, binding))
				{
					e = binding->released;
					return true;
				}
			}
			else if(sdl_event.type == SDL_KEYDOWN)
			{
				if(check_input(sdl_event, binding))
				{
					e = binding->pressed;
					return true;
				}
			}
		}
		return false;
	}

	void clear_color() final
	{
		glClearColor(0.f, 1.0, 0.f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void swap_buffers() final
	{
		SDL_GL_SwapWindow(window);
        glClearColor(0.f, 1.0, 0.f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);	}

	void unintialize() final
	{
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void create_my_shader() final
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
		}
		catch(std::exception& ex)
		{
			ex.what();
		}

		GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vertex_shader_src = R"(
                                          #version 300 es
                                          layout(location = 0)in vec4 vPosition;
                                          void main()
                                          {
                                                  gl_Position = vPosition;
                                          })";

		glShaderSource(vert_shader, 1, &vertex_shader_src, NULL);
		glCompileShader(vert_shader);

		GLint  success;
		char infoLog[512];
		glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
		    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fragment_shader_src = R"(
                                            #version 300 es
                                            precision mediump float;
                                            out vec4 fragColor;
                                            void main()
                                            {
                                                fragColor = vec4 (1.0, 0.0, 0.0, 1.0);
                                            })";
		glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
		glCompileShader(fragment_shader);

		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		GLuint shader_program = glCreateProgram();
		if (0 == shader_program)
		{
		    std::cout << "failed to create gl program";
		    glDeleteShader(vert_shader);
		    glDeleteShader(fragment_shader);
        }

		glAttachShader(shader_program, vert_shader);
		glAttachShader(shader_program, fragment_shader);
		glBindAttribLocation(shader_program, 0, "a_position");
		glLinkProgram(shader_program);
		GLint program_linked;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
		if (program_linked != GL_TRUE)
		{
		    GLsizei log_length = 0;
		    GLchar message[1024];
		    glGetProgramInfoLog(shader_program, 1024, &log_length, message);
		    std::cout << "ERROR::LINKED IS_FAILED\n" << message << std::endl;
		}

		glUseProgram(shader_program);

		glDeleteShader(vert_shader);
	    glDeleteShader(fragment_shader);
	}

	void render_triangle(te::triangle& t) final
	{
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(te::vertex), &t.v[0]);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

te::engine* te::create_engine()
{
	return new my_tiny_engine();
}

void te::destroy_engine(te::engine* e)
{
	delete e;
}

te::engine::~engine(){}
