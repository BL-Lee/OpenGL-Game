#pragma once

#include "Vertex.h"
#include "VertexBufferLayout.h"
#include <memory>
class VertexBuffer
{
private:
	uint32_t m_RendererID;
	std::shared_ptr<VertexBufferLayout> m_Layout;
public:
	VertexBuffer(const void* data, uint32_t size);
	VertexBuffer(uint32_t size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	
	void Allocate(uint32_t size);
	void SetData(const void* data, uint32_t size);

	inline const std::shared_ptr<VertexBufferLayout> GetLayout() const { return m_Layout; }
	void SetLayout(std::shared_ptr<VertexBufferLayout> layout) { m_Layout = layout; }
};
