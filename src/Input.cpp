
#include "Input.h"

bool Input::GetKey(std::shared_ptr<Window> h, int keycode)
{
	GLFWwindow* window = h->GetGLWindow();
	int state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS;
}
bool Input::GetMouseButton(std::shared_ptr<Window> h, int button)
{
	GLFWwindow* window  = h->GetGLWindow();
	int state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}
glm::vec2 Input::GetMousePos(std::shared_ptr<Window> h)
{
	GLFWwindow* window = h->GetGLWindow();
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glm::vec2 a((float)x, (float)y);
	return a;
	//return std::make_pair<float, float>((float)x, (float)y);
}

float Input::GetMouseXPos(std::shared_ptr<Window> h)
{
	return GetMousePos(h).x;
}
float Input::GetMouseYPos(std::shared_ptr<Window> h)
{
	return GetMousePos(h).y;
}

glm::vec2 Input::GetMousePosOpenGLCoords(std::shared_ptr<Window> h, std::shared_ptr<OrthoCamera> camera)
{
	//Probably shouldnt construct a vec4 but oh well
	glm::vec4 pos = { GetMouseXPos(h), GetMouseYPos(h) , 1.0f, 1.0f };
	//Converts screen space to OpenGl -1,1 
	float halfWidth = (float)h->GetWidth() / 2.0f;
	float halfHeight = (float)h->GetHeight() / 2.0f;
	pos.x = (pos.x - halfWidth) / halfWidth;
	pos.y = -(pos.y - halfHeight) / halfHeight;
	glm::vec2 ret(glm::inverse(camera->GetMatrix()) * pos);
	return ret;
}