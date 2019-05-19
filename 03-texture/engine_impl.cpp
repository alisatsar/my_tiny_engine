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
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
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
    shader->get_uniform_location("s_texture");

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



void te::my_tiny_engine::create_texture()
{
    const int location =
            te::gl::glGetUniformLocation(shader->get_shader_program(), "outTexture");
        if (location == -1)
        {
            std::cerr << "can't get uniform location from shader\n";
        }
        unsigned int texture_unit = 0;
        glActiveTexture(GL_TEXTURE0 + texture_unit);



}

void te::my_tiny_engine::render_texture(GLuint texture_id)
{
    const int location =
            te::gl::glGetUniformLocation(shader->get_shader_program(), "outTexture");
        if (location == -1)
        {
            std::cerr << "can't get uniform location from shader\n";
            throw std::runtime_error("can't get uniform location");
        }
        unsigned int texture_unit = 0;
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    te::gl::glUniform1i(location, 0);
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

void te::my_tiny_engine::render_with_vbo(const te::triangle& t1,
                         const te::color& color)
{
    using namespace te::gl;
    const int vertex_pos_size = 2;
    const int vertex_color_size = 4;
    const int vertex_pos_index = 0;
    const int vertex_color_index = 1;
    const int vertex_stride = sizeof(GLfloat) * (vertex_color_size + vertex_pos_size);

    const GLint num_vertices = 3;
    const GLint num_indices = 3;
    GLfloat vtxBuf[3 * (vertex_pos_size + vertex_color_size)] =
    {
            t1.v[0].x, t1.v[0].y,
            color.get_r(), color.get_g(), color.get_b(), color.get_a(),
            t1.v[1].x, t1.v[1].y,
            1.0f, 0.0f, 0.0f, 1.0f,
            t1.v[2].x, t1.v[2].y,
            0.0f, 0.0f, 1.0f, 1.0f
    };

    GLushort indices[3] = { 0, 1, 2 };
    GLuint vboId[2] = {0, 0};

    GLuint offset = 0;

    if(vboId[0] == 0 && vboId[1] == 0)
    {
        te::gl::glGenBuffers(2, vboId);
        te::gl::glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
        te::gl::glBufferData(GL_ARRAY_BUFFER,
                             num_vertices * vertex_stride,
                             vtxBuf, GL_STATIC_DRAW);
        te::gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
        te::gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * num_indices,
                             indices, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);

    glEnableVertexAttribArray(vertex_pos_index);
    glEnableVertexAttribArray(vertex_color_index);

    glVertexAttribPointer(vertex_pos_index, vertex_pos_size,
                          GL_FLOAT, GL_FALSE, vertex_stride,
                          (const void*)offset);

    offset += vertex_pos_size * sizeof(GLfloat);
    glVertexAttribPointer(vertex_color_index, vertex_color_size,
                          GL_FLOAT, GL_FALSE, vertex_stride,
                          (const void*)offset);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(vertex_pos_index);
    glDisableVertexAttribArray(vertex_color_index);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void te::my_tiny_engine::render_with_vao(GLuint a_vao)
{
    te::gl::glBindVertexArray(a_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    te::gl::glBindVertexArray(0);
}

GLuint te::my_tiny_engine::create_vao(const te::triangle& t1,
                        const te::color& color)
{
    my_vao = std::make_unique<te::vao>(t1, color, 2);
    return my_vao->get_vao_id();
}

void te::my_tiny_engine::draw()
{
    using namespace te::gl;

    GLfloat vVertices[] = { -0.2f,  0.2f, 0.0f,  // Position 0
                             0.0f,  0.0f,        // TexCoord 0
                            -0.5f, -0.5f, 0.0f,  // Position 1
                             0.0f,  1.0f,        // TexCoord 1
                             0.5f, -0.5f, 0.0f,  // Position 2
                             1.0f,  1.0f,        // TexCoord 2
                             0.5f,  0.5f, 0.0f,  // Position 3
                             1.0f,  0.0f         // TexCoord 3
                          };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };


    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( shader->get_shader_program() );

    // Load the vertex position
    glVertexAttribPointer ( 0, 3, GL_FLOAT,
                            GL_FALSE, 5 * sizeof ( GLfloat ), vVertices );
    // Load the texture coordinate
    glVertexAttribPointer ( 1, 2, GL_FLOAT,
                            GL_FALSE, 5 * sizeof ( GLfloat ), &vVertices[3] );

    glEnableVertexAttribArray ( 0 );
    glEnableVertexAttribArray ( 1 );

    // Bind the texture
    glActiveTexture ( GL_TEXTURE0 );
    glBindTexture ( GL_TEXTURE_2D, textureId );

    GLint samplerLoc = glGetUniformLocation(shader->get_shader_program(), "s_texture" );
    // Set the sampler texture unit to 0
    glUniform1i ( samplerLoc, 0 );

    glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}

void te::my_tiny_engine::create_simple_texture_2D()
{
    // 2x2 Image, 3 bytes per pixel (R, G, B)
    GLubyte pixels[4 * 3] =
    {
       100,   0,   0, // Red
         0, 200,   0, // Green
         100,   100, 200, // Blue
       255, 255,   0  // Yellow
    };

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &textureId );

    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, textureId );

    // Load the texture
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void te::my_tiny_engine::init()
{
    create_simple_texture_2D ();

    glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );
}
