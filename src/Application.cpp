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
    WindowProps props(1366,768,"Bababooey", false);
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
            
            bullet->velocity = player->velocity + glm::vec2{ cos(player->rotation), sin(player->rotation) } * 100.0f;
            bullet->pos = player->pos + glm::vec3{ cos(player->rotation), sin(player->rotation),0.0f } * 5.0f;
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
    TextBox fps({ -window->GetWidth() / 2, window->GetHeight() / 2 - 20, 0.22f }, { 100.0f,20.0f }, { 1.0f,0.5f,1.0f,1.0f }, "123456789101111111111111111111111111111111111111111111111111", m_silkFont);
    TextBox dt({ -window->GetWidth() / 2, window->GetHeight() / 2 - 40, 0.22f }, { 100.0f,20.0f }, { 1.0f,0.5f,1.0f,1.0f }, "123456789101111111111111111111111111111111111111111111111113", m_silkFont);
    TextBox gpudt({ -window->GetWidth() / 2, window->GetHeight() / 2 - 60, 0.22f }, { 140.0f,20.0f }, { 1.0f,0.5f,1.0f,1.0f }, "1234567891011111111111111111111111111111111111111111111111113", m_silkFont);
    TextBox rendererdt({ -window->GetWidth() / 2, window->GetHeight() / 2 - 80, 0.22f }, { 160.0f,20.0f }, { 1.0f,0.5f,1.0f,1.0f }, "1234567891011111111111111111111111111111111111111111111111113", m_silkFont);
    m_UIManager.AddTextBox(fps);
    m_UIManager.AddTextBox(dt);
    m_UIManager.AddTextBox(gpudt);
    m_UIManager.AddTextBox(rendererdt);
    glm::vec2 basevertices[3] = { glm::vec2(1.0f, 0.0f), glm::vec2(-0.5f, -0.86602540378f), glm::vec2(-0.5f, 0.86602540378f) };

    Entity player = {};
    player.pos = { 23.232323f,45.5f,-0.1f };
    player.size = { 20.0f,10.0f };
    player.rotation = 1.3f;
    player.acceleration = 200.0f;
    player.colour = { 0.7f,0.1f,0.2f,1.0f };
    player.Accelerate = AccelerateEntityForward;
    player.UpdateTransform = UpdateTransform;
    player.mass = 1.0f;
    player.inverseMass = 1.0f;
    player.restitution = 0.0f;
    player.gravityDirection = { 0.0f, -1.0f };
    player.vertexCount = 3;
    SetEntityAABB(&player);
    memcpy(player.localVertices, basevertices, sizeof(glm::vec2) * player.vertexCount);
    
    glm::vec2 box[4] = { { -0.5f, -0.5f },
                         { 0.5f, -0.5f},
                         { 0.5f,  0.5f},
                         { -0.5f, 0.5f } };
    Entity* asteroid = (Entity*)calloc(1, sizeof(Entity));
    asteroid->pos = { 0.0f,-300.0f,0.0f };
    asteroid->size = { 10.0f,5.0f };
    asteroid->rotation = 0.0f;
    asteroid->vertexCount = 4;
    asteroid->angularVelocity = 0.0f;
    SetEntityAABB(asteroid);
    CopyVertices(asteroid, box, 4);
    SetEntityMass(asteroid, 1.0f);
    asteroid->UpdateTransform = UpdateTransform;
    asteroid->restitution = 0.2f; 
    asteroid->colour = { 1.0f,0.0f,0.0f,1.0f };
    
    Entity bullet = {};
    bullet.size = { 10.0f, 10.0f };
    SetEntityAABB(&bullet);
    CopyVertices(&bullet, box, 4);
    SetEntityMass(&bullet, 1.0f);
    bullet.UpdateTransform = UpdateTransform;
    bullet.restitution = 1.0f;
    bullet.colour = { 1.0f,1.0f,1.0f,1.0f };

    Registry::addEntity(&player);
    Registry::addEntity(asteroid);
    Registry::addEntity(&bullet);

    double lastTime = glfwGetTime();
    double deltaTime = 0, nowTime = 0;
    double fpsTime = lastTime;
    double prevfpsTime = glfwGetTime();
    double particleCooldown = lastTime;

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
            
            if (nowTime - particleCooldown > 0.016)
            {
                particleCooldown = nowTime;
                Particle& p = ParticleSystem::GetbaseParticle();
                p.Pos = player.pos;
                p.Rotation = player.rotation;
                p.Velocity = 100.0f * -(direction * (Random::Float() + 1.0f)) - player.velocity;
                ParticleSystem::Add(1);
            }
            
            player.Accelerate(&player, player.rotation, deltaTime);
        }
        if (Input::GetKey(window, GLFW_KEY_RIGHT))
            player.rotation -= glm::radians(200.0f) * deltaTime;
        if (Input::GetKey(window, GLFW_KEY_LEFT))
            player.rotation += glm::radians(200.0f) * deltaTime;
        
        for (int i = 0; i < Registry::validIndices.size(); i++)
        {
            Entity* e = Registry::getEntityByKey(Registry::validIndices[i]);
            {
                if (e)
                {
                    UpdateWorldVertices(e);
                }
            }
        }
        Renderer::BeginScene(m_Camera);
        double renddt = 0;
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
                        )
                    {
                        Registry::deleteEntity(e->id);
                        index--;
                        continue;
                    }
                }
                if (e->mass)
                {
                    e->UpdateTransform(e, deltaTime); //no mass means its a wall, so it doesnt move
                }
                
                for (int otherIndex = index+1; otherIndex < Registry::validIndices.size(); otherIndex++)
                {
                    int j = Registry::validIndices[otherIndex];
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
                            
                            //printf("Collidingboi %d and %d\n", e->id, other->id);
                            CollisionOverlapData overlapData = Collision::IsColliding(e->vertices, e->vertexCount, other->vertices, other->vertexCount);
                            if (!(overlapData.AToCollisionPoint.x == 0.0f && overlapData.AToCollisionPoint.y == 0.0f))
                            {
                                Collision::ResolveCollision(*e, *other, overlapData.AToCollisionPoint, overlapData.normalOffB);
                                Renderer::DrawQuad(e->pos, { e->AABBWidth, e->AABBHeight }, { 0.0f,1.0f,0.0f,1.0f });
                            }
                        }
                    }                 
                }
                double draw = glfwGetTime();
                Renderer::DrawRotatedQuad(e->pos, e->size, e->rotation, e->colour);
                //Renderer::DrawQuad(e->pos, { e->AABBWidth, e->AABBHeight }, { 1.0f,0.0f,1.0f,1.0f });
                
                double drawdt = glfwGetTime() - draw;
                
                renddt += drawdt;
            }
        }
 
        if (player.rotation > glm::two_pi<float>()) //really dont like this, add math constant header with pi as a float so i dont need to do this <float>() crap
            player.rotation -= glm::two_pi<float>();
        if (player.rotation < glm::two_pi<float>())
            player.rotation += glm::two_pi<float>();

        double draw = glfwGetTime();
        ParticleSystem::Draw((float)deltaTime);
        Renderer::DrawRotatedTriangle(player.pos, player.size, player.rotation, player.colour);
        double drawdt = glfwGetTime() - draw;
        renddt += drawdt;
        double gpuTime = glfwGetTime();
        Renderer::EndScene();
        double gpuTimeElapsed = glfwGetTime() - gpuTime;
        if (fpsTime - prevfpsTime > 0.2)
        {
            prevfpsTime = glfwGetTime();
            fpsTime = prevfpsTime;
            sprintf_s(dt.text, dt.textLength, "%.2fms", deltaTime * 1000);
            sprintf_s(fps.text, fps.textLength, "%.0ffps", (1 / deltaTime));
            sprintf_s(gpudt.text, gpudt.textLength, "gpu time: %.2fms", (gpuTimeElapsed*1000));
            sprintf_s(rendererdt.text, rendererdt.textLength, "renderer time: %.2fms", (renddt * 1000));
        }
        renddt = 0;
        Renderer::RenderUI(m_UIManager, m_Camera);
        fpsTime += deltaTime;
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