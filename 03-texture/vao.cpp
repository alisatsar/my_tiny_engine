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

vao::vao(const te::triangle& t1, const te::triangle& t2, const te::color& color)
{
    float ver[8] = { t1.v[0].x, t1.v[0].y,
                  t1.v[1].x, t1.v[1].y,
                  t1.v[2].x, t1.v[2].y,
                  t2.v[1].x, t2.v[1].y };

    GLushort indices[6] = { 0, 1, 2, 0, 3, 2 };
    gl::glGenBuffers(2, vbo_ids);
    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_STATIC_DRAW);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);
    gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);

    gl::glEnableVertexAttribArray(VERTEX_POS_INDEX);
    gl::glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE_2D,
                              GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                              (const void *) 0);

    gl::glBindVertexArray(0);
}

vao::vao(const te::triangle& t1, const te::triangle& t2, const te::color& color1,
       const te::color& color2, const te::color& color3, const te::color& color4,
    const te::triangle& te1, const te::triangle& te2)
{
    float ver[4 * VERTEX_POS_SIZE_2D * VERTEX_COLOR_SIZE * VERTEX_TEXTURE_SIZE] = { t1.v[0].x, t1.v[0].y,
                            color1.get_r(), color1.get_g(), color1.get_b(), color1.get_a(),
                            te1.v[0].x, te1.v[0].y,
                            t1.v[1].x, t1.v[1].y,
                            color1.get_r(), color1.get_g(), color1.get_b(), color1.get_a(),
                            te1.v[1].x, te1.v[1].y,
                            t1.v[2].x, t1.v[2].y,
                            color1.get_r(), color1.get_g(), color1.get_b(), color1.get_a(),
                            te1.v[2].x, te1.v[2].y,
                            t2.v[1].x, t2.v[1].y,
                            color1.get_r(), color1.get_g(), color1.get_b(), color1.get_a(),
                            te2.v[1].x, te2.v[1].y
    };

    GLushort indices[6] = { 0, 1, 2, 0, 3, 2 };
    const GLint vertex_stride = sizeof(GLfloat) * (VERTEX_POS_SIZE_2D +
                                                   VERTEX_COLOR_SIZE +
                                                   VERTEX_TEXTURE_SIZE);

    gl::glGenBuffers(2, vbo_ids);
    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_STATIC_DRAW);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);
    gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);

    gl::glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE_2D,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) VERTEX_POS_OFFSET);

    gl::glVertexAttribPointer(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) VERTEX_COLOR_OFFSET);


    gl::glVertexAttribPointer(VERTEX_TEXTURE_INDEX, VERTEX_TEXTURE_SIZE,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) VERTEX_TEXTURE_OFFSET );
    gl::glEnableVertexAttribArray(VERTEX_POS_INDEX);
    gl::glEnableVertexAttribArray(VERTEX_COLOR_INDEX);
    gl::glEnableVertexAttribArray(VERTEX_TEXTURE_INDEX);

    gl::glBindVertexArray(0);
}

vao::vao(const te::triangle& t1, const te::triangle& t2,
    const te::triangle& te1, const te::triangle& te2)
{
    float ver[4 * VERTEX_POS_SIZE_2D *  VERTEX_TEXTURE_SIZE] = { t1.v[0].x, t1.v[0].y,
                            te1.v[0].x, te1.v[0].y,
                            t1.v[1].x, t1.v[1].y,
                            te1.v[1].x, te1.v[1].y,
                            t1.v[2].x, t1.v[2].y,
                            te1.v[2].x, te1.v[2].y,
                            t2.v[1].x, t2.v[1].y,
                            te2.v[1].x, te2.v[1].y
    };

    GLushort indices[6] = { 0, 1, 2, 0, 3, 2 };
    const GLint vertex_stride = sizeof(GLfloat) * (VERTEX_POS_SIZE_2D +
                                                   VERTEX_TEXTURE_SIZE);

    gl::glGenBuffers(2, vbo_ids);
    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_STATIC_DRAW);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);
    gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);

    gl::glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE_2D,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) VERTEX_POS_OFFSET);

    gl::glVertexAttribPointer(VERTEX_TEXTURE_INDEX, VERTEX_TEXTURE_SIZE,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) 2 );
    gl::glEnableVertexAttribArray(VERTEX_POS_INDEX);
    gl::glEnableVertexAttribArray(VERTEX_TEXTURE_INDEX);

    gl::glBindVertexArray(0);
}


vao::vao(const te::triangle& t1)
{
    float ver[4 * 2 *  2] =
    {
            -0.5, -0.5, 0.0f, 0.0f,
            -0.5, 0.5, 0.0f, 1.0f,
            0.5, 0.5, 1.0f, 1.f,
            0.5, -0.5, 1.0f, 0.0f
    };

    GLushort indices[6] = { 0, 1, 2, 0, 3, 2 };
    const GLint vertex_stride = sizeof(GLfloat) * (2 +
                                                   2);

    gl::glGenBuffers(2, vbo_ids);
    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_STATIC_DRAW);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);
    gl::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    gl::glGenVertexArrays(1, &vao_id);
    gl::glBindVertexArray(vao_id);

    gl::glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[0]);
    gl::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[1]);

    gl::glVertexAttribPointer(0, 2,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) 0);

    gl::glVertexAttribPointer(1, 2,
                              GL_FLOAT, GL_FALSE, vertex_stride,
                              (const void *) 2 );
    gl::glEnableVertexAttribArray(0);
    gl::glEnableVertexAttribArray(1);

    gl::glBindVertexArray(0);
}



