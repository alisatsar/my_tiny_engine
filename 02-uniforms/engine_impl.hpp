#pragma once
#include "engine.h"
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "shader.h"

//this means tiny_engine
namespace te
{
class my_tiny_engine : public te::engine
{
public:
    my_tiny_engine();
    std::string check_version() final;
    bool initialize() final;
    bool create_window(const char* title, int32_t pos_x, int32_t pos_y, int32_t width, int32_t height) final;
    void swap_buffers() final;
    void unintialize() final;
    void create_my_shader() final;
    void render_triangle(te::triangle& t) final;

    //from learnopengl
    void render_vertices(float vertices[]) final;   
    void render_with_buffer(float vertices[]) final;

    //from book

private:
    SDL_GLContext gl_context;
    SDL_Window* window;
    te::shader* shader;
    GLuint VBO;
    GLuint EBO;
    GLuint VAO;
};


}//end namespace te
