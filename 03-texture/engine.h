#ifndef ENGINE_H
#define ENGINE_H

#include <cstdint>
#include <string>

#include "common/types.h"
#include "common/color.h"
#include "gl_initializer.h"

namespace te {

class engine
{
public:
        virtual ~engine();
        virtual std::string check_version() = 0;
        virtual bool initialize() = 0;
        virtual bool create_window(const char* title, int32_t pos_x, int32_t pos_y, int32_t width, int32_t height) = 0;
        virtual void unintialize() = 0;
        virtual void swap_buffers() = 0;
        //triangle_2d
        virtual void render_triangle(triangle& t) = 0;
        virtual void create_my_shader(const std::string& vertex_file_path,
                                      const std::string& fragment_file_path) = 0;
        virtual float get_time() = 0;

        virtual void render_dinamic_color() = 0;

        virtual void render_vertices(float vertices[]) = 0;
        virtual void render_vertex_color(float vertices_color[]) = 0;

        virtual void render_r_c() = 0;

        virtual void create_texture(const char* file_path) = 0;
        virtual void render_texture() = 0;
        //color_vertex
        virtual void render_color_triangle(const triangle &t, const te::color &color) = 0;
        virtual void render_with_vbo(const te::triangle& t1, const te::color& color) = 0;

        virtual GLuint create_vao(const te::triangle& t1,
                                const te::color& color) = 0;


        virtual void render_with_vao(GLuint a_vao) = 0;

};

    engine* create_engine();
    void destroy_engine(engine* e);

}//end namespace te
#endif // ENGINE_H
