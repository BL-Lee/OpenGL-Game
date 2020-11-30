#pragma once
#include "UIElement.h"
#include <vector>
class UIManager
{
private:
	std::vector<UIElement*> m_UIElements;
public:
	void AddBaseElement(const glm::vec3& location, const glm::vec2& size, const glm::vec4& colour);
	void AddBaseElement(UIElement& e) { m_UIElements.push_back(&e); }

	void AddTextBox(const glm::vec3 location, const glm::vec2 size, const glm::vec4 colour, const std::string& text,
	const std::shared_ptr<Font>& font, const glm::vec4 textColour);
	void AddTextBox(TextBox& box) { m_UIElements.push_back(&box); }

	std::vector<UIElement*>& GetElements() { return m_UIElements; }

};