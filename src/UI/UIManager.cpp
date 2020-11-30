#include "UIManager.h"


void UIManager::AddBaseElement(const glm::vec3& location, const glm::vec2& size, const glm::vec4& colour)
{
	UIElement e(location, size, colour);
	m_UIElements.push_back(&e);
}
void UIManager::AddTextBox(const glm::vec3 location, const glm::vec2 size, const glm::vec4 colour, const std::string& text,
	const std::shared_ptr<Font>& font, const glm::vec4 textColour = { 1.0f,1.0f,1.0f,1.0f })
{
	//For now its okay because im not removing any elements,
	//But later if I want to remove an element and keep running I'll need to free that element.
	//Vectors do not automatically destruct items when they are pointers
	//delete *m_UIElements[i];
	//
	//Or to delete the whole thing:
	//for (std::vector<UIElement*>::iterator i = m_UIElements.begin(); i != m_UIElements.end(); ++i) {
	//	delete* i;
	//}
	TextBox* e = new TextBox(location, size, colour, text, font, textColour);
	m_UIElements.push_back(e);
}

