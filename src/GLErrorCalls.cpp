#include "GLErrorCalls.h"
#include <iostream>

//Method that gets called during an OpenGL Error
void GLAPIENTRY MessageCallback(GLenum source, GLenum type,
    GLuint id, GLenum severity,
    GLsizei length, const GLchar* message,
    const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
    {
        std::cout << "[OpenGL Error] (" << type << ") " << message << std::endl;
    }
}