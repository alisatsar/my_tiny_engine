#include "engine_impl.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string_view>
#include <cmath>


#include "shader.h"
#include "gl_initializer.h"
#include "texture.h"

#include "common/types_constants.h"

te::my_tiny_engine::my_tiny_engine()
{}

std::string te::my_tiny_engine::check_version()
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

bool te::my_tiny_engine::initialize()
{
    const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
    if(init_result != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool te::my_tiny_engine::create_window(const char* title, int32_t pos_x, int32_t pos_y, int32_t width, int32_t height)
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

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3); //OpenGL 3+
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 1); //OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

void te::my_tiny_engine::swap_buffers()
{
    SDL_GL_SwapWindow(window);
    glClearColor(0.f, 1.0, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void te::my_tiny_engine::unintialize()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void te::my_tiny_engine::create_my_shader(const std::string& vertex_file_path,
                                          const std::string& fragment_file_path)
{
    shader->create_program_string(vertex_file_path.c_str(), fragment_file_path.c_str());
    shader->add_attribute("a_position");

    shader->link_shaders();
    shader->use_program();
}

void te::my_tiny_engine::render_triangle(te::triangle& t)
{
    using namespace te::gl;

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(te::vertex),
                          &t.v[0]);

    te::gl::glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void te::my_tiny_engine::render_vertices(float vertices[])
{
    te::gl::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices), &vertices[0]);
    te::gl::glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

te::engine* te::create_engine()
{
	return new my_tiny_engine();
}

void te::destroy_engine(te::engine* e)
{
	delete e;
}

te::engine::~engine(){}

void te::my_tiny_engine::render_with_buffer(float vertices[])
{
    te::gl::glGenBuffers(1, &VBO);
    te::gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
    te::gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    te::gl::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices), &vertices[0]);
    te::gl::glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

float te::my_tiny_engine::get_time()
{
    std::uint32_t ms_from_library_initialization = SDL_GetTicks();
    return ms_from_library_initialization * 0.001f;
}

void te::my_tiny_engine::render_dinamic_color()
{
    float time_value = get_time();
    float green_value = (sin(time_value) /2.0f) + 0.5f;
    int vertexColorLocation = shader->get_uniform_location("ourColor");
    shader->use_program();
    te::gl::glUniform4f(vertexColorLocation, 0.0f, green_value, 0.0f, 1.0f);
}

void te::my_tiny_engine::render_vertex_color(float vertices_color[])
{
    using namespace te::gl;

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          vertices_color);

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          6 * sizeof(float),
                          (vertices_color + 3));

    te::gl::glEnableVertexAttribArray(0);
    te::gl::glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void te::my_tiny_engine::render_r_c()
{
    using namespace te::gl;
#define VERTEX_POS_SIZE 3   //x, y, z
#define VERTEX_COLOR_SIZE 3    //x, y, z

#define VERTEX_POS_INDEX 0
#define VERTEX_COLOR_INDEX 1

#define VERTEX_POS_OFFSET 0
#define VERTEX_COLOR_OFFSET 3

#define VERTEX_ATTRIB_SIZE (VERTEX_POS_SIZE +\
                             VERTEX_COLOR_OFFSET)
    float *p = (float*)malloc(3 * VERTEX_ATTRIB_SIZE * sizeof(float));
    p[0] = 0.5f;
    p[1] = -0.5f;
    p[2] = 0.5f;
    p[3] = 0.0f;
    p[4] = 1.0f;
    p[5] = 0.0f;

    p[6] = -0.5f;
    p[7] = -0.5f;
    p[8] = 0.0f;
    p[9] = 1.0f;
    p[10] = 0.0f;
    p[11] = 0.0f;

    p[12] = 0.0f;
    p[13] = 0.5f;
    p[14] = 0.0f;
    p[15] = 0.0f;
    p[16] = 0.0f;
    p[17] = 1.0f;

    glVertexAttribPointer(VERTEX_POS_INDEX,
                          VERTEX_POS_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_ATTRIB_SIZE * sizeof(float),
                          p);

    glVertexAttribPointer(VERTEX_COLOR_INDEX,
                          VERTEX_COLOR_SIZE,
                          GL_FLOAT,
                          GL_FALSE,
                          VERTEX_ATTRIB_SIZE * sizeof(float),
                          (p + VERTEX_COLOR_OFFSET));

    te::gl::glEnableVertexAttribArray(VERTEX_POS_INDEX);
    te::gl::glEnableVertexAttribArray(VERTEX_COLOR_INDEX);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}



void te::my_tiny_engine::create_texture(const char* file_path)
{
    using namespace te::gl;

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          vertices + 3);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          vertices + 6);
    glEnableVertexAttribArray(2);


    using namespace te::gl;
    glGenTextures(1, &texture_id);

    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    texture tex(file_path);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.get_width(),
                 tex.get_heigth(), 0, GL_RGB, GL_UNSIGNED_BYTE, tex.get_data());

}

void te::my_tiny_engine::render_texture()
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void te::my_tiny_engine::render_color_triangle(const te::triangle& t,
                                                 const te::color& color)
{
    GLfloat c[4] = {color.get_r(), color.get_g(), color.get_b(), color.get_a()};
    te::gl::glVertexAttrib4fv(0, c);
    te::gl::glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                                  &t.v[0]);
    te::gl::glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    te::gl::glDisableVertexAttribArray(1);

}



