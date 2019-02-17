#include "engine.hxx"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string_view>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>
#include "shader.h"
#include "gl_initializer.h"


class my_tiny_engine : public te::engine
{
private:
	SDL_GLContext gl_context;
	SDL_Window* window = nullptr;
    te::shader* shader;

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
        shader = new te::shader();
		return true;
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
        const char* vertex_shader_src = R"(
                                          #version 300 es
                                          layout(location = 0)in vec4 vPosition;
                                          void main()
                                          {
                                                  gl_Position = vPosition;
                                          })";


        const char* fragment_shader_src = R"(
                                            #version 300 es
                                            precision mediump float;
                                            out vec4 fragColor;
                                            void main()
                                            {
                                                fragColor = vec4 (1.0, 0.0, 0.0, 1.0);
                                            })";

        shader->create_program(vertex_shader_src, fragment_shader_src);
	}

	void render_triangle(te::triangle& t) final
	{
        te::gl::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(te::vertex), &t.v[0]);
        te::gl::glEnableVertexAttribArray(0);
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
