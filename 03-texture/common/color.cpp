#include "color.h"
#include <cassert>

namespace te
{

color::color(std::uint32_t a_rgba)
    : rgba(a_rgba)
{}

color::color(float a_r, float a_g, float a_b, float a_a)
{
    assert(a_r <= 1 && a_r >= 0);
    assert(a_g <= 1 && a_g >= 0);
    assert(a_b <= 1 && a_b >= 0);
    assert(a_a <= 1 && a_a >= 0);

    std::uint32_t r = static_cast<std::uint32_t>(r * 255);
    std::uint32_t g = static_cast<std::uint32_t>(g * 255);
    std::uint32_t b = static_cast<std::uint32_t>(b * 255);
    std::uint32_t a = static_cast<std::uint32_t>(a * 255);

    rgba = a << 24 | b << 16 | g << 8 | r;
}

float color::get_r() const
{
    std::uint32_t r = (rgba & 0x000000FF) >> 0;
    return r / 255.f;
}

float color::get_g() const
{
    std::uint32_t g = (rgba & 0x0000FF00) >> 0;
    return g / 255.f;
}

float color::get_b() const
{
    std::uint32_t b = (rgba & 0x00FF0000) >> 0;
    return b / 255.f;
}

float color::get_a() const
{
    std::uint32_t a = (rgba & 0xFF000000) >> 0;
    return a / 255.f;
}

void color::set_r(const float r)
{
    std::uint32_t ra = static_cast<std::uint32_t>(r * 255);
    rgba &= 0xFFFFFF00;
    rgba |= (ra << 0);
}

void color::set_g(const float g)
{
    std::uint32_t ga = static_cast<std::uint32_t>(g * 255);
    rgba &= 0xFFFF00FF;
    rgba |= (ga << 8);
}

void color::set_b(const float b)
{
    std::uint32_t ba = static_cast<std::uint32_t>(b * 255);
    rgba &= 0xFF00FFFF;
    rgba |= (ba << 16);
}

void color::set_a(const float a)
{
    std::uint32_t aa = static_cast<std::uint32_t>(a * 255);
    rgba &= 0x00FFFFFF;
    rgba |= (aa << 24);
}

}//end namespace te

