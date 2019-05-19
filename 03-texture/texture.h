#pragma once
#include <iostream>
#include "gl_initializer.h"

class texture
{
public:
    texture();
    texture(const char* file_path);
    texture(const std::string& file_path);
    int get_width();
    int get_heigth();
    int get_nr_channels();

    unsigned char *get_data();
    void free_data();

    GLuint get_tex_id() const;

private:
    int width;
    int height;
    int nr_channels;
    unsigned char *data;

    GLuint tex_id;
};
