#pragma once
#include <string>

namespace spc//shader_path_constans
{
    const std::string shader_directory("/home/alisatsar/my_tiny_engine/my_tiny_engine/03-texture/shaders/");
    //render simple 2D triangle (red color default)
    const std::string triangle_2D_vert(shader_directory + "triangle_2d.vert");
    const std::string triangle_2D_frag(shader_directory + "triangle_2d.frag");
}
