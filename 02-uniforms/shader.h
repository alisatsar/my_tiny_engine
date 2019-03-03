#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <SDL2/SDL_opengl.h>

namespace te {

class shader
{
public:
    shader();
    GLuint create_program(const char* vertex_src, const char* fragment_src);
    GLuint create_program_string(const std::string& vertex_file_path, const std::string& fragment_file_path);
    void add_attribute(const std::string& attribute_name);
    void link_shaders();
    void use_program();

    //from book
    void get_info_about_active_uniform();
    void set_uniform_buffer(GLfloat data[], const char* name);

private:
    GLuint create_shader(GLenum shaderType, const GLchar* shader_src);

private:
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;

    int32_t num_attributes;
};

}//end namespace te


#endif // SHADER_H
