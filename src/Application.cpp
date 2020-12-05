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
    { 0.0f, 0.5f, 1.0f, 1.0f },//scolour
    { 0.9f, 0.3f, 0.15f, 1.0f },//endcolor
    { 0.0f, 0.0f },//speed
    90.0f,//Random::Float() * 360.0f, //rotation
    Random::Float() * 400.0f, //spin speed
    0.5f, //lifetime
    0.0f, //0
    false, //isactive
    };
    ParticleSystem::SetParticle(p);
    ParticleSystem::Init(999);
    player.pos = { 0.0f,0.0f,-0.1f };
    player.size = { 10.0f,20.0f };
    player.acceleration = 0.1f;
    

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
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OnWindowResize));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(OnKeyPress));
    dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FUNC(OnKeyRelease));
    dispatcher.Dispatch<MousePressedEvent>(BIND_EVENT_FUNC(OnMousePress));
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    IsRunning = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
    int x = e.GetX();
    int y = e.GetY();
    m_Window->SetDimensions(x, y);
    m_Camera->SetDimensions((float)x, (float)y);
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
            }
        }
    }
    return true;
}


void Application::Run()
{
    std::shared_ptr<Texture> goose = std::make_shared<Texture>("res/textures/goose.png");

    std::shared_ptr<Font> m_silkFont = std::make_shared<Font>();
    TextBox rotation({ -250.0f,0.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,1.0f }, "Ui Element its", m_silkFont);
    TextBox velocity({ -250.0f,-50.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,1.0f }, "Ui Element its", m_silkFont);
    TextBox position({ -250.0f,-100.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,1.0f }, "Ui Element its", m_silkFont);
    m_UIManager.AddTextBox(rotation);
    m_UIManager.AddTextBox(velocity);
    m_UIManager.AddTextBox(position);
    UIElement randomElement({ 0.0f,-300.0f,0.01f }, { 50.0f,50.0f }, { 1.0f,1.0f,1.0f,0.2f });
    m_UIManager.AddBaseElement(randomElement);

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

        if (Input::GetKey(m_Window, GLFW_KEY_UP))
        {
            glm::vec2 direction = {cos(glm::radians(player.rotation + 90.0f)), sin(glm::radians(player.rotation + 90.0))};

            Particle& p = ParticleSystem::GetbaseParticle();
            p.Pos = player.pos;
            p.Rotation = player.rotation + 90.0f;
            p.Velocity =  100.0f * -(direction * (Random::Float() + 1.0f)) - player.velocity;
            ParticleSystem::Add(1);

            player.velocity += direction * player.acceleration;
        }
        if (Input::GetKey(m_Window, GLFW_KEY_RIGHT))       
            player.rotation -= 5.0f;
        
        if (Input::GetKey(m_Window, GLFW_KEY_LEFT))        
            player.rotation += 5.0f;
        
        Renderer::BeginScene(m_Camera);
        ParticleSystem::Draw((float)deltaTime);
        Renderer::DrawRotatedTriangle(player.pos, player.size, player.rotation);

#if 0
        Renderer::DrawQuad({ 0.5f,0.0f,0.0f }, { 100.0f,100.0f }, goose, { 1.0f,1.0f,1.0f,1.0f });
        Renderer::DrawQuad({ 105.0f,-100.0f,0.1f }, { 1000.0f,100.0f }, { 0.5f,0.5f,0.2f,1.0f });
        Renderer::DrawTriangle({ 105.0f,200.0f,0.1f }, { 50.0f,100.0f }, { 0.5f,0.5f,0.2f,1.0f });
#endif
        Renderer::EndScene();
        rotation.SetText("Rotation: "+ std::to_string(player.rotation).substr(0,4));
        velocity.SetText("Velocity: " + std::to_string(player.velocity.x).substr(0,4) + "," +  std::to_string(player.velocity.y).substr(0,4));
        position.SetText("Position: " + std::to_string(player.pos.x).substr(0, 4) + "," + std::to_string(player.pos.y).substr(0, 4));
        Renderer::RenderUI(m_UIManager, m_Camera);
        m_Window->OnUpdate();

        player.pos.x += player.velocity.x;
        player.pos.y += player.velocity.y;
        if (player.rotation > 360.0f)
            player.rotation -= 360.0f;
        if (player.rotation < 0.0f)
            player.rotation += 360.0f;
#if 0
        int width = (int)m_Window->GetWidth();
        int height = (int)m_Window->GetHeight();
        if (player.pos.x >  width / 2)
            player.pos.x = -width / 2;
        if (player.pos.x < -width / 2)
            player.pos.x =  width / 2;

        if (player.pos.y >  height / 2)
            player.pos.y = -height / 2;
        if (player.pos.y < -height / 2)
            player.pos.y =  height / 2;
#endif
    }
}

int main()
{
    Application app;
    app.Run();
    app.Shutdown();
    return 0;
}