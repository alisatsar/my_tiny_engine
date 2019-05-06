#pragma once
#include "engine.h"
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_opengl_glext.h>

#include "shader.h"
#include "texture.h"

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
    void create_my_shader(const std::string& vertex_file_path,
                          const std::string& fragment_file_path) final;

    //render
    void render_triangle(te::triangle& t) final;
    float get_time() final;

    void render_dinamic_color() final;


    //from learnopengl
    void render_vertices(float vertices[]) final;   
    void render_with_buffer(float vertices[]) final;
    void render_vertex_color(float vertices_color[]) final;
    void render_r_c() final;
    //texture
    void create_texture(const char* file_path) final;
    void render_texture();


    void render_color_triangle(const triangle &t, const te::color &color) final;

private:
    SDL_GLContext gl_context;
    SDL_Window* window;
    te::shader* shader;
    GLuint VBO;
    GLuint EBO;
    GLuint VAO;
    unsigned int texture_id;
};


}//end namespace te
