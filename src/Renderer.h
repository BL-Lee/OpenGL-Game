#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>
#include "OrthoCamera.h"
#include "Font.h"
#include "UI/UIManager.h"

class Renderer
{
public:

    static void Init();
    static void Shutdown();

    static void Clear();
    
    //SERIOUSLY busted. maybe say fuck it and pass an entity
    
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Texture* tex, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawQuad(const glm::mat4& transform, const Texture* tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    //static void DrawQuad(const glm::mat4& transform, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    

    static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Texture* tex, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawRotatedQuad(const glm::mat4& transform, const Texture* tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void DrawTriangle(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    static void DrawTriangle(const glm::vec3& pos, const glm::vec2& size, const Texture* tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    
    static void DrawRotatedTriangle(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    static void DrawRotatedTriangle(const glm::vec3& pos, const glm::vec2& size, float rotation, const Texture* tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    static void DrawRotatedTriangle(const glm::mat4& transform, const Texture* tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void DrawLine(const glm::vec3& start, const glm::vec3& end, float width = 2.0f, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void RenderText(const glm::vec3& pos, const glm::vec2& size, const char* text, 
        const std::shared_ptr<Font>& f, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void BeginScene();
    static void BeginScene(const std::shared_ptr<OrthoCamera>& camera, uint32_t renderType = 0);
    static void EndScene();
    static void Flush();

    static void RenderUI(UIManager& manager, const std::shared_ptr<OrthoCamera>& camera);

    
private:
    static void FlushAndReset();
    static float GetOrAddTextureIndex(const Texture* tex);
};