#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorCalls.h"
#include "Application.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "Input.h"
#include "Particles.h"
#include "Entity.h"
#include "Physics/Collision.h"

#include <iostream>
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

std::shared_ptr<Window> window;

Application::Application()
{
    WindowProps props(1366, 768, "Bababooey", false);
    window = std::make_shared<Window>(props);
    window->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
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
    Random::Float() * 2.0f, //spin speed
    0.5f, //lifetime
    0.0f, //0
    false, //isactive
    };
    ParticleSystem::SetParticle(p);
    ParticleSystem::Init(999);
    player.pos = { 0.0f,0.0f,-0.1f };
    player.size = { 20.0f,10.0f };
    player.acceleration = 0.1f;
    player.colour = { 0.7f,0.1f,0.2f,1.0f };
    player.Accelerate = AccelerateEntityForward;
    player.UpdatePosition = UpdatePosition;

}
void Application::Shutdown()
{
    Renderer::Shutdown();
    window->Shutdown();
}
Application::~Application()
{
    //Renderer::Shutdown();
    //window->Shutdown();
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
    window->SetDimensions(x, y);
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
        glm::vec2 mousePos = Input::GetMousePosOpenGLCoords(window, m_Camera);
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
    TextBox CollDist({ -250.0f, 100.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,1.0f }, "Ui Element its", m_silkFont);
    m_UIManager.AddTextBox(CollDist);

    TextBox wrapper({ -250.0f,-150.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,0.0f }, "Ui Eleeeeeeeeeement its wrap wrap wrap wrap awrap", m_silkFont);
    m_UIManager.AddTextBox(wrapper);

    glm::vec4 basevertices[3] = { glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec4(-0.5f, -0.86602540378f, 0.0f, 1.0f), glm::vec4(-0.5f, 0.86602540378f, 0.0f, 1.0f) };
    glm::vec2 vertices[3] = { {0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f} };

   
    player.vertices = vertices;


    glm::mat4 transform = glm::translate(glm::mat4(1.0f), player.pos)
        * glm::rotate(glm::mat4(1.0f), (player.rotation), { 0.0f,0.0f,1.0f })
        * glm::scale(glm::mat4(1.0f), { player.size.x,player.size.y,1.0f });

    glm::mat4 transformB =  glm::scale(glm::mat4(1.0f), { 200.0f,200.0f,1.0f });

    //glm::vec2 box[3] = { transformB * basevertices[0], transformB * basevertices[1],transformB * basevertices[2] };
    glm::vec2 box[4] = { {-100.0f,100.0f},{100.0f,100.0f},{100.0f,300.0f},{-100.0f,300.0f} };
    double lastTime = glfwGetTime();
    double deltaTime = 0, nowTime = 0;

    while (IsRunning)
    {
        Renderer::Clear();
        // - Measure time
        nowTime = glfwGetTime();
        deltaTime = (nowTime - lastTime);
        lastTime = nowTime;
        m_Camera->UpdateMovement(window, deltaTime);

        if (Input::GetMouseButton(window, GLFW_MOUSE_BUTTON_1))
        {
            Particle& p = ParticleSystem::GetbaseParticle();
            p.Pos = Input::GetMousePosOpenGLCoords(window, m_Camera);
            ParticleSystem::Add(1);
        }

        if (Input::GetKey(window, GLFW_KEY_UP))
        {
            glm::vec2 direction = { cos(player.rotation), sin(player.rotation) };

            Particle& p = ParticleSystem::GetbaseParticle();
            p.Pos = player.pos;
            p.Rotation = player.rotation;
            p.Velocity = 100.0f * -(direction * (Random::Float() + 1.0f)) - player.velocity;
            ParticleSystem::Add(1);

            player.Accelerate(player, player.rotation);

        }
        else
        {
            player.velocity = { 0.0f,0.0f };
        }
        if (Input::GetKey(window, GLFW_KEY_RIGHT))
            player.rotation -= glm::radians(5.0f);

        if (Input::GetKey(window, GLFW_KEY_LEFT))
            player.rotation += glm::radians(5.0f);

        //Later cycle through all entities?
        player.UpdatePosition(player);

        if (player.rotation > glm::two_pi<float>()) //really dont like this, add math constant header with pi as a float so i dont need to do this <float>() crap
            player.rotation -= glm::two_pi<float>();
        if (player.rotation < glm::two_pi<float>())
            player.rotation += glm::two_pi<float>();

        //Bit of collision tester stuff
        glm::vec2 test = Collision::isColliding(player.vertices, 3, box, 4);
        transform = glm::translate(glm::mat4(1.0f), player.pos)
            * glm::rotate(glm::mat4(1.0f), player.rotation, { 0.0f,0.0f,1.0f })
            * glm::scale(glm::mat4(1.0f), { player.size.x,player.size.y,1.0f });
        for (unsigned int i = 0; i < 3; i++)
        {
            player.vertices[i] = transform * basevertices[i];
        }
        //not colliding
        if (test.x == 0.0f && test.y == 0.0f)
            player.colour = { 0.7f,0.1f,0.2f,1.0f };
        //colliding
        else
        {
            player.colour = { 1.0f,0.0f,1.0f,1.0f };
            player.pos.x -= test.x;
            player.pos.y -= test.y;
        }
        Renderer::BeginScene(m_Camera);
        ParticleSystem::Draw((float)deltaTime);
        Renderer::DrawRotatedTriangle(player.pos, player.size, player.rotation, player.colour);
        Renderer::DrawQuad({ 0.0f,200.0f,0.0f }, { 200.0f,200.0f }, goose);
        Renderer::DrawLine(player.pos, { Input::GetMousePosOpenGLCoords(window,m_Camera),player.pos.z });
        Renderer::DrawLine(player.pos, { test.x + player.pos.x, test.y + player.pos.y ,player.pos.z });
#if 0
        Renderer::DrawQuad({ 0.5f,0.0f,0.0f }, { 100.0f,100.0f }, goose, { 1.0f,1.0f,1.0f,1.0f });
        Renderer::DrawQuad({ 105.0f,-100.0f,0.1f }, { 1000.0f,100.0f }, { 0.5f,0.5f,0.2f,1.0f });
        Renderer::DrawTriangle({ 105.0f,200.0f,0.1f }, { 50.0f,100.0f }, { 0.5f,0.5f,0.2f,1.0f });
#endif
        Renderer::EndScene();
        rotation.SetText("Rotation: "+ std::to_string(player.rotation).substr(0,4));
        velocity.SetText("Velocity: " + std::to_string(player.velocity.x).substr(0,4) + "," +  std::to_string(player.velocity.y).substr(0,4));
        position.SetText("Position: " + std::to_string(player.pos.x).substr(0, 4) + "," + std::to_string(player.pos.y).substr(0, 4));
        CollDist.SetText("Collision Distance: " + std::to_string(test.x).substr(0, 4) + "," + std::to_string(test.y).substr(0,4));
        Renderer::RenderUI(m_UIManager, m_Camera);
        window->OnUpdate();

        

    }
}

int main()
{
    Application app;
    app.Run();
    app.Shutdown();
    return 0;
}