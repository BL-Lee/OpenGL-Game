#pragma once
#include "glm/glm.hpp"
#include "Texture.h"
#include "Window.h"
#include <memory>
#include "OrthoCamera.h"
#include "Font.h"
#include "UI/UIManager.h"

struct PlayerData
{
	glm::vec3 pos;
	glm::vec2 size;
	float rotation;
	glm::vec2 velocity;
	float acceleration;
};

class Application
{
private:
	std::shared_ptr<Window> m_Window;
	bool IsRunning = true;
	float m_LastFrameTime = 0.0f;
	std::shared_ptr<OrthoCamera> m_Camera;
	UIManager m_UIManager;
	PlayerData player = {};
public:
	Application();
	~Application();

	void OnEvent(Event& e);
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnKeyPress(KeyPressedEvent& e);
	bool OnKeyRelease(KeyReleasedEvent& e);
	bool OnMousePress(MousePressedEvent& e);
	void Shutdown();
	void Run();
};
