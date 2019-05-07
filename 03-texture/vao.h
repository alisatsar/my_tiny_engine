#pragma once

#include <vector>
#include <map>

#include "gl_initializer.h"
#include "common/types.h"
#include "common/color.h"

namespace te
{

struct vbo
{
    vbo(const te::triangle& t, const te::color& color);

};

class vao
{
public:
    vao() = default;
    vao(const te::triangle& t, const te::color& color, int count_attributes);
    GLuint get_vao_id() const { return vao_id; }

private:
    std::vector<GLuint> attribute_pointer;
    GLuint vbo_ids[2];
    GLuint vao_id;
};

}//end namespace te
