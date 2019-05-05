#include "texture.h"

#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


texture::texture(const char* file_path)
{
     data = stbi_load(file_path,
                                    &width,
                                    &height,
                                    &nr_channels,
                                    0);

     assert(data != nullptr);
}

int texture::get_width()
{
    return width;
}

int texture::get_heigth()
{
    return height;
}

int texture::get_nr_channels()
{
    return nr_channels;
}

unsigned char *texture::get_data()
{
    return data;
}

void texture::free_data()
{
    stbi_image_free(data);
}
