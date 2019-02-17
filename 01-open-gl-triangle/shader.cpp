#include "shader.h"

#include <iostream>

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
