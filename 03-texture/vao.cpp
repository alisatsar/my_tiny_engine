#include "vao.h"

#include "common/types_constants.h"

using namespace te;

vao::vao(const te::triangle& t, const te::color& color,
         int count_attributes)
{  
    GLfloat vtxBuf[3 * (VERTEX_POS_SIZE_2D + VERTEX_COLOR_SIZE)] =
    {
            t.v[0].x, t.v[0].y,
            1.0f, 0.0f, 0.0f, 1.0f,
            t.v[1].x, t.v[1].y,
            0.0f, 1.0f, 0.0f, 1.0f,
            t.v[2].x, t.v[2].y,
            0.0f, 0.0f, 1.0f, 1.0f,
    };

    GLushort indices[3] = { 0, 1, 2 };

    for(int i = 0; i < count_attributes; ++i)
    {
        attribute_pointer.push_back(i);
    }

    const GLint vertex_stride = sizeof(GLfloat) * (VERTEX_POS_SIZE_2D + VERTEX_COLOR_SIZE);

    te::gl::glGenBuffers(2, vbo_ids);
    te::gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    te::gl::glBufferData(GL_ARRAY_BUFFER, sizeof(vtxBuf),
    vtxBuf, GL_STATIC_DRAW);
    te::gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);
    te::gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    te::gl::glGenVertexArrays(1, &vao_id);
    te::gl::glBindVertexArray(vao_id);

    te::gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    te::gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);

    te::gl::glEnableVertexAttribArray(VERTEX_POS_INDEX);
    te::gl::glEnableVertexAttribArray(VERTEX_COLOR_INDEX);

    te::gl::glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE_2D,
                                GL_FLOAT, GL_FALSE, vertex_stride,
                                (const void *) 0);
    te::gl::glVertexAttribPointer(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE,
                                GL_FLOAT, GL_FALSE, vertex_stride,
                                (const void *) (VERTEX_POS_SIZE_2D * sizeof(GLfloat)));
    te::gl::glBindVertexArray(0);
}

