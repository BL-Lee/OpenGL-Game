#pragma once

#include "VertexBuffer.h "
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
class VertexArray
{
private:
	uint32_t m_RendererID;
	//std::shared_ptr<VertexBuffer> m_vertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
public:
	VertexArray();
	~VertexArray();
	void Init();
	void AddBuffer(std::shared_ptr<VertexBuffer>& vb);
	void SetIndexBuffer(std::shared_ptr<IndexBuffer>& ib);

	void Bind() const;
	void Unbind() const;

	inline const std::shared_ptr<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }


};
