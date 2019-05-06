#pragma once

#include <cstdint>

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

        std::size_t get_vertex_count() const { return sizeof(v); }
};

}//end namespace te
