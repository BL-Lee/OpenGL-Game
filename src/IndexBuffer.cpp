#include "IndexBuffer.h"
#include <GL/glew.h>
#include "GLErrorCalls.h"
#include <cstdint>
//IndexBuffer holds the indices of vertices so that they can be reused

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
{
    m_count = count;
    //creating a buffer on gpu
    glGenBuffers(1, &m_RendererID);
    //Selecting/binding that buffer so that
    //further changes change this specific buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    //Adding data to thebuffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
}

IndexBuffer::IndexBuffer(uint32_t count)
{
    m_count = count;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::SetData(const uint32_t* data, uint32_t count)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), (const void*)data);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

