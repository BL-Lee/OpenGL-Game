#pragma once
#include <vector>
#include <GL/glew.h>
#include "GLErrorCalls.h"
/**
* This class holds the layout of a vertex buffer
* For example: each vertex could contain a position and a normal, so
* 2 floats for position and 2 for normal
* each 
*/
struct VertexBufferElement
{
	uint32_t type;
	uint32_t count;
	char normalised;

	static uint32_t GetSizeOfType(uint32_t type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	uint32_t m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}
	//~VertexBufferLayout();

	template<typename T>
	void Push(uint32_t count, char normalised)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(uint32_t count, char normalised)
	{
		m_Elements.push_back({GL_FLOAT, count, normalised});
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<uint32_t>(uint32_t count, char normalised)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, normalised });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(uint32_t count, char normalised)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, normalised });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline uint32_t GetStride() const { return m_Stride; }
};
