#pragma once
#include <iostream>

class texture
{
public:
    texture(const char* file_path);
    int get_width();
    int get_heigth();
    int get_nr_channels();

    unsigned char *get_data();
    void free_data();

private:
    int width;
    int height;
    int nr_channels;
    unsigned char *data;
};
