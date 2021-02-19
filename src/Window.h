#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorCalls.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <string>

struct WindowProps
{
	int Width = 640;
	int Height = 480;
	std::string Title = "Bababooey";
	bool IsFullScreen = false;

	WindowProps(int width = 640, int height = 480, std::string title = "Bababooey", bool fullscreen = false)
		:Width(width), Height(height), Title(title), IsFullScreen(fullscreen) {}
};

class Window
{
	using EventCallbackFunc = std::function<void(Event&)>;
public:
	
	
	Window(const WindowProps& props);
	~Window();
	void Shutdown();
	void OnUpdate();
	void SetVSync(bool b);

	inline void SetEventCallback(const EventCallbackFunc& func) { m_Data.CallbackFunc = func; }

	inline bool IsVSync() { return m_Data.IsVSync; }
	inline int GetWidth() { return m_Data.Width; }
	inline int GetHeight() { return m_Data.Height; }

	void SetDimensions(uint32_t x, uint32_t y);

	inline GLFWwindow* GetGLWindow() const { return m_Window; }
private:
	GLFWwindow* m_Window;

	struct WindowData
	{
		int32_t Width;
		int32_t Height;
		std::string Title;
		EventCallbackFunc CallbackFunc;
		bool IsVSync;
	};
	WindowData m_Data;
};
