#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Window.h"
class OrthoCamera
{
private:
	float m_Width;
	float m_Height;
	glm::mat4 m_ViewMatrix = glm::ortho(-450.0f, 450.0f, -450.0f, 450.0f, -1.0f, 1.0f);
public:
	OrthoCamera(float width, float height);
	void Translate(const glm::vec2& trans);
	void Scale(float sc);
	void UpdateMovement(const std::shared_ptr<Window> window, float deltaTime);
	const glm::mat4& GetMatrix() const { return m_ViewMatrix; }
	void SetDimensions(float width, float height);
	
};