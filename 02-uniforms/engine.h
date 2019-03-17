#ifndef ENGINE_H
#define ENGINE_H

#include <cstdint>
#include <string>

namespace te {

struct vertex
{
        float x;
        float y;
        vertex() : x(0.f), y(0.f){}
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
        virtual void create_my_shader() = 0;
        virtual float get_time() = 0;

        virtual void render_dinamic_color() = 0;

        virtual void render_vertices(float vertices[]) = 0;
        virtual void render_with_buffer(float vertices[]) = 0;
};

    engine* create_engine();
    void destroy_engine(engine* e);

}//end namespace te
#endif // ENGINE_H
