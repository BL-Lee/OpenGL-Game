#include "Window.h"
#include <iostream>
Window::Window(const WindowProps& props)
{
    
    m_Data.Height = props.Height;
    m_Data.Width = props.Width;
    m_Data.Title = props.Title;

    /* Initialize the library */
    if (!glfwInit())
        ASSERT(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer(m_Window, &m_Data);


    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    SetVSync(true);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW INIT Error" << std::endl;

    //Print OpenGL version
    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    //Setting up events

    //MouseEvents
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MousePressedEvent e(button);
                    data.CallbackFunc(e);
                }break;
                case GLFW_RELEASE:
                {
                    MouseReleasedEvent e(button);
                    data.CallbackFunc(e);
                }break;
            }
        }
    );
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseScrollEvent e(x, y);
            data.CallbackFunc(e);
        });
    
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            MouseMovedEvent e(x, y);
            data.CallbackFunc(e);
        }
    );
    //KeyEvents
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent e(key, 0);
                    data.CallbackFunc(e);
                }break;
                case GLFW_REPEAT:
                {
                    KeyPressedEvent e(key, 1);
                    data.CallbackFunc(e);
                }break;
                
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent e(key);
                    data.CallbackFunc(e);
                }break;
            }
        });
    //WindowEvents
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent e;
            data.CallbackFunc(e);
        }
    );
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowResizeEvent e(width,height);
            data.CallbackFunc(e);
        });
}
void Window::SetDimensions(uint32_t x, uint32_t y)
{
    m_Data.Width = x;
    m_Data.Height = y;
    glViewport(0, 0, x, y);
}
Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
void Window::Shutdown()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
void Window::OnUpdate()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_Window);

	/* Poll for and process events */
	glfwPollEvents();
}
void Window::SetVSync(bool b)
{
	glfwSwapInterval(b);
	m_Data.IsVSync = b;
}
