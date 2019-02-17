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

private:
    GLuint create_shader(GLenum shaderType, const GLchar* shader_src);
private:
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;
};

}//end namespace te


#endif // SHADER_H
