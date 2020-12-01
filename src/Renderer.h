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
    
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture>& tex, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });

    static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
    static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& tex, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
 
    static void DrawTriangle(const glm::vec3&, const glm::vec2& size, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    static void DrawTriangle(const glm::vec3&, const glm::vec2& size, const std::shared_ptr<Texture>& tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    
    static void DrawRotatedTriangle(const glm::vec3&, const glm::vec2& size, float rotation, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });
    static void DrawRotatedTriangle(const glm::vec3&, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& tex, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void RenderText(const glm::vec3& pos, const glm::vec2& size, const std::string& text, const std::shared_ptr<Font>& f, const glm::vec4& colour = { 1.0f,1.0f,1.0f,1.0f });

    static void BeginScene();
    static void BeginScene(const std::shared_ptr<OrthoCamera>& camera, uint32_t renderType = 0);
    static void EndScene();
    static void Flush();

    static void RenderUI(UIManager& manager, const std::shared_ptr<OrthoCamera>& camera);

    
private:
    static void FlushAndReset();
    static float GetOrAddTextureIndex(const std::shared_ptr<Texture>& tex);
};