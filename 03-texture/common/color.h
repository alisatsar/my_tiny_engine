#pragma once

#include <cstdint>
#include <iostream>

namespace te
{

class color
{
public:
    color() = default;
    explicit color(std::uint32_t a_rgba);
    color(float a_r, float a_g, float a_b, float a_a);

    float get_r() const;
    float get_g() const;
    float get_b() const;
    float get_a() const;

    void set_r(const float r);
    void set_g(const float g);
    void set_b(const float b);
    void set_a(const float a);

private:
    std::uint32_t rgba = 0;
};

}//end namespace te

