#include "VertexArray.h"
#include "GLErrorCalls.h"


/**
* Vertex Arrays store both the vertex buffer (what all the vertex data sits in)
* as well as the layout for the buffer.
* For example: each vertex could contain a position and a normal, so
* 2 floats for position and 2 for normal
*/
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}
void VertexArray::Init()
{
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
}
void VertexArray::AddBuffer(std::shared_ptr<VertexBuffer>& vb)
{
	//Bind the buffer and this vertex array
	//Lets OpenGL know when we edit something (enable, add attribpointer), we will be editing these buffers/arrays
	Bind();
	vb->Bind();
	const auto& layout = vb->GetLayout();
	const auto& elements = layout->GetElements();
	uint32_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, 
			element.normalised, layout->GetStride(), 
			(const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

	//m_vertexBuffers.push_back(vb);
}

void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& ib)
{
	m_IndexBuffer = ib;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}