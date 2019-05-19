#include "texture.h"

#include <cassert>
#include <fstream>
#include "picopng.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

texture::texture()
{
    GLubyte pixels[4 * 3] =
    {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
        255, 255, 0
    };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

texture::texture(const char* file_path)
{
    data = stbi_load(file_path,
                                    &width,
                                    &height,
                                    &nr_channels,
                                    0);

    assert(data != nullptr);

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, data);
    }


    glBindTexture(GL_TEXTURE_2D, 0);
}

texture::texture(const std::string& file_path)
{
    std::vector<unsigned char> png_file_in_memory;
       std::ifstream ifs(file_path.data(), std::ios_base::binary);
       if (!ifs)
       {
           throw std::runtime_error("can't load texture");
       }
       ifs.seekg(0, std::ios_base::end);
       std::streamoff pos_in_file = ifs.tellg();
       png_file_in_memory.resize(static_cast<size_t>(pos_in_file));
       ifs.seekg(0, std::ios_base::beg);
       if (!ifs)
       {
           throw std::runtime_error("can't load texture");
       }

       ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()), pos_in_file);
       if (!ifs.good())
       {
           throw std::runtime_error("can't load texture");
       }

       const om::png_image img = decode_png_file_from_memory(
               png_file_in_memory, om::convert_color::to_rgba32,
               om::origin_point::bottom_left);

       // if there's an error, display it
       if (img.error != 0)
       {
           std::cerr << "error: " << img.error << std::endl;
           throw std::runtime_error("can't load texture");
       }

       glGenTextures(1, &tex_id);
       glBindTexture(GL_TEXTURE_2D, tex_id);

       GLint   mipmap_level = 0;
       GLint   border       = 0;
       GLsizei width        = static_cast<GLsizei>(img.width);
       GLsizei height       = static_cast<GLsizei>(img.height);
       glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, width, height, border,
                    GL_RGBA, GL_UNSIGNED_BYTE, &img.raw_image[0]);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

GLuint texture::get_tex_id() const
{
    return tex_id;
}

