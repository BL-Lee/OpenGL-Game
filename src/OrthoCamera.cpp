#include "OrthoCamera.h"
#include "Input.h"

OrthoCamera::OrthoCamera(float width, float height)
    :m_Width(width), m_Height(height)
{
    m_ViewMatrix = glm::ortho(-width / 2, width / 2, -height / 2, height / 2);
}
void OrthoCamera::Translate(const glm::vec2& translation)
{
	m_ViewMatrix = glm::translate(m_ViewMatrix, { translation, 0.0f });
}
void OrthoCamera::SetDimensions(float width, float height)
{
    m_ViewMatrix = glm::ortho(-width / 2, width / 2, -height / 2, height / 2);
}
void OrthoCamera::UpdateMovement(const std::shared_ptr<Window> window, float deltaTime)
{
    if (Input::GetKey(window, GLFW_KEY_D))
        Translate({ -1000.0f * deltaTime,0.0f });
    if (Input::GetKey(window, GLFW_KEY_A))
        Translate({ 1000.0f * deltaTime,0.0f });
    if (Input::GetKey(window, GLFW_KEY_S))
        Translate({ 0.0f,1000.0f * deltaTime });
    if (Input::GetKey(window, GLFW_KEY_W))
        Translate({ 0.0f,-1000.0f * deltaTime });
    if (Input::GetKey(window, GLFW_KEY_Q))
        Scale(0.1f);
    if (Input::GetKey(window, GLFW_KEY_E))
        Scale(-0.1f);
}
void OrthoCamera::Scale(float delta)
{
	m_ViewMatrix = glm::scale(m_ViewMatrix, { 0.0f,0.0f, delta });
}
