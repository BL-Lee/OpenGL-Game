#pragma once

#include "glm/glm.hpp"
struct Vertex
{
	glm::vec2 Position;
	glm::vec2 TexCoord;
	glm::vec4 Colour;
	float TextureIndex;
};