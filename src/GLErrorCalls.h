#pragma once
#include <GL/glew.h>
#define ASSERT(x) if (!(x)) __debugbreak();
//Function that gets called when an error occurs
void GLAPIENTRY MessageCallback(GLenum source, GLenum type,
    GLuint id, GLenum severity,
    GLsizei length, const GLchar * message,
    const void* userParam);