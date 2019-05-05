#ifndef ENGINE_H
#define ENGINE_H

#include <cstdint>
#include <string>

namespace te {

struct vertex
{
        float x;
        float y;
        vertex()
            : x(0.f), y(0.f)
        {}
};

struct triangle
{
        vertex v[3];
        triangle()
        {
                v[0] = vertex();
                v[1] = vertex();
                v[2] = vertex();
        }
        triangle(float x0, float y0, float x1, float y1, float x2, float y2)
        {
            v[0].x = x0;
            v[0].y = y0;
            v[1].x = x1;
            v[1].y = y1;
            v[2].x = x2;
            v[2].y = y2;
        }
};

class engine
{
public:
        virtual ~engine();
        virtual std::string check_version() = 0;
        virtual bool initialize() = 0;
        virtual bool create_window(const char* title, int32_t pos_x, int32_t pos_y, int32_t width, int32_t height) = 0;
        virtual void unintialize() = 0;
        virtual void swap_buffers() = 0;
        virtual void render_triangle(triangle& t) = 0;
        virtual void create_my_shader(const std::string& vertex_file_path,
                                      const std::string& fragment_file_path) = 0;
        virtual float get_time() = 0;

        virtual void render_dinamic_color() = 0;

        virtual void render_vertices(float vertices[]) = 0;
        virtual void render_with_buffer(float vertices[]) = 0;
        virtual void render_vertex_color(float vertices_color[]) = 0;

        virtual void render_r_c() = 0;

        virtual void create_texture(const char* file_path) = 0;
        virtual void render_texture() = 0;


};

    engine* create_engine();
    void destroy_engine(engine* e);

}//end namespace te
#endif // ENGINE_H
