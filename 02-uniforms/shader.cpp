#include "shader.h"

#include <iostream>
#include <fstream>

#include "gl_initializer.h"

te::shader::shader()
    : vertex_shader(0)
    , fragment_shader(0)
    , shader_program(0)
    , num_attributes(0)
{
    te::gl::initializer_gl_functions();
}


GLuint te::shader::create_shader(GLenum shaderType, const GLchar* shader_src)
{
    GLuint shader = te::gl::glCreateShader(shaderType);

    te::gl::glShaderSource(shader, 1, &shader_src, NULL);
    te::gl::glCompileShader(shader);

    GLint  success;
    char infoLog[512];
    te::gl::glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
            te::gl::glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint te::shader::create_program_string(const std::string& vertex_file_path,
                                 const std::string& fragment_file_path)
{
    std::ifstream vertex_file(vertex_file_path);
    if(vertex_file.fail())
    {
        perror(vertex_file_path.c_str());
    }

    std::string vertex_content = "";
    std::string line;

    while(std::getline(vertex_file, line))
    {
        vertex_content += line + "\n";
    }

    vertex_file.close();

    std::ifstream fragment_file(fragment_file_path);
    if(fragment_file.fail())
    {
        perror(fragment_file_path.c_str());
    }

    std::string fragment_content = "";
    line.clear();

    while(std::getline(fragment_file, line))
    {
        fragment_content += line + "\n";
    }

    vertex_file.close();

    return create_program(vertex_content.c_str(), fragment_content.c_str());
}

GLuint te::shader::create_program(const char* vertex_src, const char* fragment_src)
{
    vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_src);
    fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_src);

    shader_program = te::gl::glCreateProgram();
    if (0 == shader_program)
    {
        std::cout << "failed to create gl program";
        te::gl::glDeleteShader(vertex_shader);
        te::gl::glDeleteShader(fragment_shader);
    }

    te::gl::glAttachShader(shader_program, vertex_shader);
    te::gl::glAttachShader(shader_program, fragment_shader);
}

void te::shader::add_attribute(const std::string& attribute_name)
{
    te::gl::glBindAttribLocation(shader_program, 0, attribute_name.c_str());
}

void te::shader::link_shaders()
{
    te::gl::glLinkProgram(shader_program);
    GLint program_linked;
    te::gl::glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        te::gl::glGetProgramInfoLog(shader_program, 1024, &log_length, message);
        std::cout << "ERROR::LINKED IS_FAILED\n" << message << std::endl;
    }
}

void te::shader::use_program()
{
    te::gl::glUseProgram(shader_program);

    te::gl::glDeleteShader(vertex_shader);
    te::gl::glDeleteShader(fragment_shader);
}

void te::shader::get_info_about_active_uniform()
{
    GLint num_uniforms;//count uniforms
    te::gl::glGetProgramiv(shader_program, GL_ACTIVE_UNIFORMS, &num_uniforms);
    
    GLint max_uniforms_lenght;//max uniform-name lenght
    te::gl::glGetProgramiv(shader_program, 
                           GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniforms_lenght);
    
    GLchar uniform_name[max_uniforms_lenght];
    GLint index;
    for(index = 0; index < num_uniforms; ++index)
    {
        GLint size;
        GLenum type;
        GLint location;

        te::gl::glGetActiveUniform(shader_program, index, max_uniforms_lenght,
                                   NULL, &size, &type, uniform_name);
        location = te::gl::glGetUniformLocation(shader_program, uniform_name);

        switch (type) {
        case GL_FLOAT:
            std::cout << uniform_name << " GL_FLOAT" << std::endl;
            break;
        case GL_FLOAT_VEC2:
            std::cout << uniform_name << " GL_FLOAT_VEC2" << std::endl;
            break;
        case GL_FLOAT_VEC3:
            std::cout << uniform_name << " GL_FLOAT_VEC3" << std::endl;
            break;
        case GL_FLOAT_VEC4:
            std::cout << uniform_name << " GL_FLOAT_VEC4" << std::endl;
            break;
        case GL_INT:
            std::cout << uniform_name << " GL_INT" << std::endl;
            break;
        default:
            break;
        }
    }
}

void te::shader::set_uniform_buffer(GLfloat data[], const char* name)
{
    GLuint block_id;
    GLuint buffer_id;
    GLint block_size;
    GLuint bindign_point = 1;

    block_id = te::gl::glGetUniformBlockIndex(shader_program, name);
    te::gl::glUniformBlockBinding(shader_program, block_id, bindign_point);

    te::gl::glGetActiveUniformBlockiv(shader_program, block_id, GL_UNIFORM_BLOCK_DATA_SIZE,
                                      &block_size);

    te::gl::glGenBuffers(1, &buffer_id);
    te::gl::glBindBuffer(GL_UNIFORM_BUFFER, buffer_id);
    te::gl::glBufferData(GL_UNIFORM_BUFFER, block_size, data, GL_DYNAMIC_DRAW);
    te::gl::glBindBufferBase(GL_UNIFORM_BUFFER, bindign_point, buffer_id);
}

