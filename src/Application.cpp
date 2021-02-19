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
#include "Registry.h"

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
    switch (e.GetKeyCode())
    {
        case GLFW_KEY_SPACE:
        {
            Entity* bullet = Registry::cloneEntity(2);  //for bullet for now CLEANUP CLEANUP
            Entity* player = Registry::getEntityByKey(0); // player
            bullet->velocity = player->velocity + glm::vec2{0.0f, 100.0f};
            bullet->pos = player->pos;
            printf("Entity added! key: %d\n", bullet->id);
        }break;
    }
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

    std::shared_ptr<Texture> outlineTex = std::make_shared<Texture>("res/textures/InvertexOutline.png");

    std::shared_ptr<Font> m_silkFont = std::make_shared<Font>();
    TextBox fps({ -250.0f,0.0f, 0.22f }, { 500.0f,50.0f }, { 1.0f,0.5f,1.0f,1.0f }, "Ui Element its", m_silkFont);
    m_UIManager.AddTextBox(fps);

    glm::vec2 basevertices[3] = { glm::vec2(1.0f, 0.0f), glm::vec2(-0.5f, -0.86602540378f), glm::vec2(-0.5f, 0.86602540378f) };

    Entity player = {};
    player.pos = { 0.0f,0.0f,-0.1f };
    player.size = { 20.0f,10.0f };
    player.acceleration = 200.0f;
    player.colour = { 0.7f,0.1f,0.2f,1.0f };
    player.Accelerate = AccelerateEntityForward;
    player.UpdateTransform = UpdateTransform;
    player.mass = 1.0f;
    player.inverseMass = 1.0f;
    player.restitution = 0.0f;
    player.gravityStrength = 0;//9.81f * 10;
    player.gravityDirection = { 0.0f, -1.0f };
    player.vertexCount = 3;
    SetEntityAABB(&player);
    memcpy(player.localVertices, basevertices, sizeof(glm::vec2) * player.vertexCount);
    
    glm::vec2 box[4] = { { -0.5f, -0.5f },
                         { 0.5f, -0.5f},
                         { 0.5f,  0.5f},
                         { -0.5f, 0.5f } };
    Entity asteroid = {};
    asteroid.pos = { 0.0f,-300.0f,0.0f };
    asteroid.size = { 10.0f,5.0f };
    asteroid.rotation = 0.0f;
    asteroid.vertexCount = 4;
    asteroid.angularVelocity = 0.0f;
    SetEntityAABB(&asteroid);
    memcpy(asteroid.localVertices, box, sizeof(glm::vec2) * asteroid.vertexCount); //make this a function
    SetEntityMass(&asteroid, 1.0f);
    asteroid.UpdateTransform = UpdateTransform;
    asteroid.restitution = 0.2f; 
    asteroid.colour = { 1.0f,1.0f,1.0f,1.0f };
    
    Entity bullet = {};
    bullet.size = { 2.0f, 2.0f };
    CopyVertices(&bullet, box, 4);
    SetEntityMass(&bullet, 1.0f);
    bullet.UpdateTransform = UpdateTransform;
    bullet.restitution = 1.0f;
    bullet.colour = { 1.0f,1.0f,1.0f,1.0f };

    Registry::addEntity(&player);
    Registry::addEntity(&asteroid);
    Registry::addEntity(&bullet);

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
            p.Pos      = player.pos;
            p.Rotation = player.rotation;
            p.Velocity = 100.0f * -(direction * (Random::Float() + 1.0f)) - player.velocity;
            ParticleSystem::Add(1);
            
            player.Accelerate(&player, player.rotation, deltaTime);
        }
        else
        {
            player.velocity = { 0.0f,0.0f };
        }
        if (Input::GetKey(window, GLFW_KEY_RIGHT))
            player.rotation -= glm::radians(200.0f) * deltaTime;
        if (Input::GetKey(window, GLFW_KEY_LEFT))
            player.rotation += glm::radians(200.0f) * deltaTime;
        
        for (int i = 0; i < Registry::validIndices.size(); i++)
        {
            Entity* e = Registry::getEntityByKey(i);
            {
                if (e)
                {
                    UpdateWorldVertices(e);
                }
            }
        }
        for (int index = 0; index < Registry::validIndices.size(); index++)
        {
            int i = Registry::validIndices[index];
            Entity* e = Registry::getEntityByKey(i);
            if (e)
            {
                if (e->vertexCount == 4)
                {
                    if (e->pos.x > window->GetWidth() / 2 ||
                        e->pos.x < -window->GetWidth() / 2 ||
                        e->pos.y > window->GetHeight() / 2 ||
                        e->pos.y < -window->GetHeight() / 2
                        && e->id != 2
                        )
                    {
                         Registry::deleteEntity(e->id);
                        continue;
                    }
                    Renderer::BeginScene(m_Camera);
                    Renderer::DrawRotatedQuad(e->pos, e->size, e->rotation, e->colour);
                    
                    Renderer::EndScene();
                }
                if (e->mass)
                {
                    e->UpdateTransform(e, deltaTime); //no mass means its a wall, so it doesnt move
                }
                
                for (int j = i+1; j < Registry::validIndices.size(); j++)
                {
                    
                    Entity* other = Registry::getEntityByKey(j);
                    if (other)
                    {
                        if (
                            e->pos.x - e->AABBWidth < other->pos.x + other->AABBWidth &&
                            e->pos.x + e->AABBWidth > other->pos.x - other->AABBWidth &&
                            e->pos.y + e->AABBHeight > other->pos.y - other->AABBHeight &&
                            e->pos.y - e->AABBHeight < other->pos.y + other->AABBHeight
                            )
                        {                       
                            CollisionOverlapData overlapData = Collision::IsColliding(e->vertices, e->vertexCount, other->vertices, other->vertexCount);
                            if (!(overlapData.AToCollisionPoint.x == 0.0f && overlapData.AToCollisionPoint.y == 0.0f))
                            {
                                Collision::ResolveCollisionNoRotation(*e, *other, overlapData.AToCollisionPoint, overlapData.normalOffB);
                            }
                        }
                    }                 
                }
            }
        }


        if (player.rotation > glm::two_pi<float>()) //really dont like this, add math constant header with pi as a float so i dont need to do this <float>() crap
            player.rotation -= glm::two_pi<float>();
        if (player.rotation < glm::two_pi<float>())
            player.rotation += glm::two_pi<float>();

        Renderer::BeginScene(m_Camera);
        ParticleSystem::Draw((float)deltaTime);
        Renderer::DrawRotatedTriangle(player.pos, player.size, player.rotation, player.colour);

        Renderer::EndScene();
        fps.SetText(std::to_string(deltaTime*1000) + "ms");
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