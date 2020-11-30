#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorCalls.h"
#include "Application.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "Input.h"
#include "Particles.h"

#include <iostream>
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

/*
Application* CreateApplication()
{
    Application* app = Application::Get();
    return app;
    //p;
   //eturn app;
}*/


Application::Application()
{
    WindowProps props(900, 900, "Bababooey", false);
    m_Window = std::make_shared<Window>(props);
    m_Window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
    m_Camera = std::make_shared<OrthoCamera>((float)props.Width, (float)props.Height);
    Renderer::Init();

    Random::Init();
    //set base particle if you want then init
    Particle p =
    {
    { 0.0f,0.0f },
    { 0.0f, 0.0f },
    { 50.0f, 50.0f},
    { 0.0f, 0.8f, 1.0f, 1.0f },//scolour
    { 0.9f, 1.0f,0.0f, 1.0f },//endcolor
    { 0.5f, 0.0f },//speed
    Random::Float() * 360.0f, //rotation
    Random::Float() * 10000.0f, //spin speed
    0.2f, //lifetime
    0.0f, //0
    false, //isactive
    };
    ParticleSystem::SetParticle(p);
    ParticleSystem::Init(999);

}
void Application::Shutdown()
{
    Renderer::Shutdown();
    m_Window->Shutdown();
}
Application::~Application()
{
    //Renderer::Shutdown();
    //m_Window->Shutdown();
}

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(OnKeyPress));
    dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNC(OnKeyRelease));
    dispatcher.Dispatch<MousePressedEvent>(BIND_EVENT_FUNC(OnMousePress));
    //std::cout << e.ToString() << std::endl;
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    IsRunning = false;
    return true;
}

bool Application::OnKeyPress(KeyPressedEvent& e)
{
    //switch on keycode for important thing like escape, pause whatnot
    //Otherwise use Input Polling
    return true;
}
bool Application::OnKeyRelease(KeyReleasedEvent& e)
{
    return true;
}
bool Application::OnMousePress(MousePressedEvent& e)
{
    if (e.GetKeyCode() == GLFW_MOUSE_BUTTON_1)
    {
        std::vector<UIElement*> elements = m_UIManager.GetElements();
        glm::vec2 mousePos = Input::GetMousePosOpenGLCoords(m_Window, m_Camera);
        for (uint32_t i = 0; i < elements.size(); i++)
        {
            
            if (elements[i]->IsInBounds(mousePos.x,mousePos.y))
            {
                elements[i]->colour.r = (((int)((elements[i]->colour.r + 0.3f) * 100) % 100) / 100.0f);
                std::cout << "boi" << std::endl;
            }
        }
    }
    return true;
}


void Application::Run()
{
    std::shared_ptr<Texture> goose = std::make_shared<Texture>("res/textures/goose.png");

    std::shared_ptr<Font> m_silkFont = std::make_shared<Font>();
    TextBox box({ 0.0f,0.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,1.0f }, "Ui Element its", m_silkFont);
    
    m_UIManager.AddTextBox(box);
    m_UIManager.AddTextBox({ 0.0f,-100.0f,0.0f }, { 100.0f,10.0f }, { 0.6f,0.5f,0.4f,1.0f,}, "Texbox Time.!#$%@", m_silkFont, { 1.0f,1.0f,1.0f,1.0f });
    //TextBox box2({ -100.0f,0.0f, 0.11f }, { 300.0f,100.0f }, { 0.0f,0.5f,1.0f,0.9f }, "Ui 2s", m_silkFont);
    box.SetTextSize(1.0f);
    double lastTime = glfwGetTime();
    double deltaTime = 0, nowTime = 0;

    while (IsRunning)
    {    
        Renderer::Clear();
        // - Measure time
        nowTime = glfwGetTime();
        deltaTime = (nowTime - lastTime);
        lastTime = nowTime;
        m_Camera->UpdateMovement(m_Window, deltaTime);
        if (Input::GetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1))
        {
            Particle& p = ParticleSystem::GetbaseParticle();
            p.Pos = Input::GetMousePosOpenGLCoords(m_Window, m_Camera);
            ParticleSystem::Add(1);
        }    
        
        Renderer::BeginScene(m_Camera);
        ParticleSystem::Draw((float)deltaTime);
        
        Renderer::DrawQuad({ 0.5f,0.0f,0.0f }, { 100.0f,100.0f }, goose, { 1.0f,1.0f,1.0f,1.0f });
        Renderer::DrawQuad({ 105.0f,-100.0f,0.1f }, { 1000.0f,100.0f }, { 0.5f,0.5f,0.2f,1.0f });
        Renderer::EndScene();
        Renderer::RenderUI(m_UIManager, m_Camera);
        m_Window->OnUpdate();
    }
}

int main()
{

    Application app;
    app.Run();
    app.Shutdown();
    return 0;
}