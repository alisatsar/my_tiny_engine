#include "shader.h"

#include <iostream>
#include <fstream>

#include "gl_initializer.h"

te::shader::shader()
    : vertex_shader(0)
    , fragment_shader(0)
    , shader_program(0)
{
    te::gl::initializer_gl_functions();
}


GLuint te::shader::create_shader(GLenum shaderType, const GLchar* shader_src)
{
    if(te::gl::glCreateShader == nullptr)
    {
        std::cout << "nullptr" ;
    }
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
    te::gl::glBindAttribLocation(shader_program, 0, "a_position");
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

    te::gl::glUseProgram(shader_program);

    te::gl::glDeleteShader(vertex_shader);
    te::gl::glDeleteShader(fragment_shader);
}

GLuint te::shader::create_shader(const std::string& vertex_file_path,
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

    return create_shader(vertex_content, fragment_content);
}
