#pragma once
#include "Application.h"
#include <utility>
class Input
{
public:
	static bool GetKey(std::shared_ptr<Window> h, int keycode);
	static bool GetMouseButton(std::shared_ptr<Window> h, int button);
	static glm::vec2 GetMousePos(std::shared_ptr<Window> h);
	static float GetMouseXPos(std::shared_ptr<Window> h);
	static float GetMouseYPos(std::shared_ptr<Window> h);
	static glm::vec2 GetMousePosOpenGLCoords(std::shared_ptr<Window> h, std::shared_ptr<OrthoCamera> camera);
};
