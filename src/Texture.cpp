#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filepath)
	: m_RendererID(0),
	m_Width(0), m_Height(0),
	m_BitsPerPixel(0)
{

	stbi_set_flip_vertically_on_load(1);
	unsigned char* m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 4);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
		m_Width, m_Height, 0, 
		GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
	}
}


Texture::Texture(const uint32_t width, const uint32_t height, GLenum channel, GLenum format)
:	m_RendererID(0),
	m_Width(width), m_Height(height),m_BitsPerPixel(4)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, channel,
		m_Width, m_Height, 0,
		format, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetData(unsigned char* data)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
void Texture::SetData(unsigned char* data, uint32_t xOffset, uint32_t width, uint32_t height, GLenum format)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, 0, width, height, format, GL_UNSIGNED_BYTE, data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}


void Texture::Bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnitBind(uint32_t slot) const
{
	//glActiveTexture(GL_TEXTURE0 + slot);
	//glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glBindTextureUnit(GL_TEXTURE0 + slot, m_RendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

