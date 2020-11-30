#pragma once

#include <stdint.h>

class IndexBuffer
{
private:
	uint32_t m_RendererID;
	uint32_t m_count;
public:
	IndexBuffer(const uint32_t* data, uint32_t count);
	~IndexBuffer();

	void reset(const uint32_t* data, uint32_t count);
	void Bind() const;
	void Unbind() const;

	inline uint32_t GetCount() const { return m_count; }
};
