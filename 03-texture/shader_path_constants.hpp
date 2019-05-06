#pragma once
#include <string>

///shader_path_constans
namespace spc
{
    const std::string shader_directory("/home/alisatsar/my_tiny_engine/my_tiny_engine/03-texture/shaders/");
    //render simple 2D triangle (red color default)  void render_triangle(te::triangle& t)
    const std::string triangle_2D_vert(shader_directory + "triangle_2d.vert");
    const std::string triangle_2D_frag(shader_directory + "triangle_2d.frag");

    //render 2D trianle with constant attribute color(vec4)
    //void render_color_triangle(const triangle &t, const te::color &color)
    const std::string triangle_3D_color_vert(shader_directory + "color_vertex.vert");
    const std::string triangle_3D_color_frag(shader_directory + "color_vertex.frag");
}
