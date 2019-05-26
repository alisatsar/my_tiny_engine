#pragma once

#include <vector>
#include <map>

#include "gl_initializer.h"
#include "common/types.h"
#include "common/color.h"

namespace te
{

class vao
{
public:
    vao() = default;
    vao(const te::triangle& t, const te::color& color, int count_attributes);
    GLuint get_vao_id() const { return vao_id; }
    vao(const te::triangle& t1, const te::triangle& t2, const te::color& color);

private:
    std::vector<GLuint> attribute_pointer;
    GLuint vbo_ids[2];
    GLuint vao_id;
};

}//end namespace te
