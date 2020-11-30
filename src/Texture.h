#pragma once

#include "GLErrorCalls.h"
#include <string>
#include "glm/glm.hpp"

class Texture
{
private:
	uint32_t m_RendererID;
	int m_Width, m_Height, m_BitsPerPixel;
public:
	Texture(const std::string& path);
	Texture(const uint32_t width, const uint32_t height, GLenum channel, GLenum format);
	~Texture();

	void Bind(uint32_t slot = 0) const;
	void UnitBind(uint32_t slot) const;
	void Unbind() const;

	void SetData(unsigned char* data);
	void SetData(unsigned char* data, uint32_t xOffset, uint32_t width, uint32_t height, GLenum format);

	bool operator==(const Texture& other) const
	{
		return m_RendererID == other.GetRendererID();
	}

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetRendererID() const { return m_RendererID; }
};
