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
	inline uint32_t GetWidth() { return m_Data.Width; }
	inline uint32_t GetHeight() { return m_Data.Height; }

	inline GLFWwindow* GetGLWindow() const { return m_Window; }
private:
	GLFWwindow* m_Window;

	struct WindowData
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;
		EventCallbackFunc CallbackFunc;
		bool IsVSync;
	};
	WindowData m_Data;
};
