
#include <GL/glew.h>
#include "Renderer.h"
#include "GLErrorCalls.h"
#include "glm/gtx/rotate_vector.hpp"
#include <array>

struct QuadVertex
{
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec4 Colour;
    float TextureIndex;
};


struct RendererData
{
    const uint32_t MaxQuads = 10000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    std::shared_ptr<VertexArray> vertArray;
    std::shared_ptr<VertexBuffer> vertBuffer;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> textShader;

    uint32_t QuadIndexCount = 0;

    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    std::array<std::shared_ptr<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex;

    glm::vec4 QuadVertexPositions[4];

};


static RendererData* s_RendererData;



void Renderer::Init()
{

    s_RendererData = new RendererData();
    s_RendererData->vertArray = std::make_shared<VertexArray>();

    std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(sizeof(QuadVertex) * s_RendererData->MaxVertices);
    std::shared_ptr<VertexBufferLayout> layout = std::make_shared<VertexBufferLayout>();
    layout->Push<float>(3, GL_FALSE); //location
    layout->Push<float>(2, GL_FALSE); //Tex Coord
    layout->Push<float>(4, GL_FALSE); //Colour
    layout->Push<float>(1, GL_FALSE); //tex index
    vb->SetLayout(layout);

    uint32_t* quadIndices = new uint32_t[s_RendererData->MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_RendererData->MaxIndices; i += 6)
    {
        quadIndices[i + 0] = 0 + offset;
        quadIndices[i + 1] = 1 + offset;
        quadIndices[i + 2] = 2 + offset;

        quadIndices[i + 3] = 2 + offset;
        quadIndices[i + 4] = 3 + offset;
        quadIndices[i + 5] = 0 + offset;
        offset += 4;
    }


    std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(quadIndices, s_RendererData->MaxIndices);
    s_RendererData->vertArray->SetIndexBuffer(ib);

    s_RendererData->QuadVertexBufferBase = new QuadVertex[s_RendererData->MaxVertices];
    delete[] quadIndices;

    s_RendererData->vertArray->AddBuffer(vb);
    s_RendererData->vertBuffer = vb;

    s_RendererData->shader = std::make_shared<Shader>("res/shaders/FlatTexture.shader");
    

    std::shared_ptr<Texture>whiteTexture = std::make_shared<Texture>("res/textures/White.png");
    //unsigned char white[] = { 0xff, 0xff, 0xff, 0xff};
    //s_RendererData->whiteTexture->SetData(white, sizeof(unsigned char) * 4);
    whiteTexture->Bind(0);
    s_RendererData->TextureSlots[0] = whiteTexture;

    int slots[s_RendererData->MaxTextureSlots];
    for (int i = 0; i < s_RendererData->MaxTextureSlots; i++)
    {
        slots[i] = i;
    }

    s_RendererData->shader->Bind();
    s_RendererData->shader->SetUniform1iv("u_Texture", s_RendererData->MaxTextureSlots, slots[0]);
    
    s_RendererData->textShader = std::make_shared<Shader>("res/shaders/TextShader.shader");
    s_RendererData->textShader->Bind();
    s_RendererData->textShader->SetUniform1iv("u_Texture", s_RendererData->MaxTextureSlots, slots[0]);

    s_RendererData->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_RendererData->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_RendererData->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_RendererData->QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
   
}

void Renderer::Shutdown()
{
    delete s_RendererData;
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BeginScene()
{
    s_RendererData->shader->Bind();
    s_RendererData->shader->SetUniformMat4f("u_ViewMatrix", glm::mat4(1.0f)); 
    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    s_RendererData->shader->SetUniformMat4f("u_ProjMatrix", proj);
    s_RendererData->shader->SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.0f, 0)));

    s_RendererData->QuadVertexBufferPtr = s_RendererData->QuadVertexBufferBase;
    s_RendererData->QuadIndexCount = 0;

    s_RendererData->TextureSlotIndex = 1;
}
void Renderer::BeginScene(const std::shared_ptr<OrthoCamera>& camera, uint32_t renderType)
{
    switch (renderType)
    {
    case 0:
        {
            s_RendererData->shader->Bind();
            s_RendererData->shader->SetUniformMat4f("u_ViewMatrix", camera->GetMatrix()); //for default view for now, later use a camera
            glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
            s_RendererData->shader->SetUniformMat4f("u_ProjMatrix", proj);
            s_RendererData->shader->SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.0f, 0)));
        }break;
    case 1:
        {
            s_RendererData->textShader->Bind();
            s_RendererData->textShader->SetUniformMat4f("u_ViewMatrix", camera->GetMatrix()); //for default view for now, later use a camera
            glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
            s_RendererData->textShader->SetUniformMat4f("u_ProjMatrix", proj);
            s_RendererData->textShader->SetUniformMat4f("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.0f, 0)));
        }break;
    }
    

    s_RendererData->QuadVertexBufferPtr = s_RendererData->QuadVertexBufferBase;
    s_RendererData->QuadIndexCount = 0;

    s_RendererData->TextureSlotIndex = 1;
}

void Renderer::EndScene()
{

    uint32_t dataSize = (uint8_t*)s_RendererData->QuadVertexBufferPtr - (uint8_t*)s_RendererData->QuadVertexBufferBase;
    s_RendererData->vertBuffer->SetData((const void*)s_RendererData->QuadVertexBufferBase, dataSize);

    Flush();
}

void Renderer::Flush()
{
    for (uint32_t i = 0; i < s_RendererData->TextureSlotIndex; i++)
    {
        s_RendererData->TextureSlots[i]->Bind(i);
    }
    glDrawElements(GL_TRIANGLES, s_RendererData->QuadIndexCount, GL_UNSIGNED_INT, nullptr);
    //glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
{
    //Probably not the best...
    //This searches for the blank texture again
    DrawQuad(pos, size, s_RendererData->TextureSlots[0], colour);
}

void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
{

    if (s_RendererData->QuadIndexCount >= s_RendererData->MaxIndices 
        || s_RendererData->TextureSlotIndex  == s_RendererData->MaxTextureSlots)
        FlushAndReset();

    float texIndex = GetOrAddTextureIndex(tex);

    //Don't really need to do a matrix multiplication if we're not rotating, but oh well
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)      
        * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[0];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[1];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[2];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[3];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadIndexCount += 6;
    
}

void Renderer::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
{

    if (s_RendererData->QuadIndexCount >= s_RendererData->MaxIndices
        || s_RendererData->TextureSlotIndex == s_RendererData->MaxTextureSlots)
        FlushAndReset();

    float texIndex = GetOrAddTextureIndex(tex);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[0];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[1];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[2];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadVertexBufferPtr->Position = transform * s_RendererData->QuadVertexPositions[3];
    s_RendererData->QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_RendererData->QuadVertexBufferPtr->Colour = colour;
    s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->QuadVertexBufferPtr++;

    s_RendererData->QuadIndexCount += 6;
}

void Renderer::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation ,const glm::vec4& colour)
{
    DrawRotatedQuad(pos, size, rotation, s_RendererData->TextureSlots[0], colour);
}

void Renderer::RenderText(const glm::vec3& pos, const glm::vec2& size, const std::string& text, const std::shared_ptr<Font>& f, const glm::vec4& colour)
{
 
    std::shared_ptr<Texture> atlas = f->GetAtlas();

    float texIndex = GetOrAddTextureIndex(atlas);

    float xOffset = pos.x;
    float yOffset = pos.y;
    std::string::const_iterator c;
    std::array<FontCharacterinfo,128>& chars = f->GetChars();
    for (c = text.begin(); c != text.end(); c++)
    {
        if (s_RendererData->QuadIndexCount >= s_RendererData->MaxIndices
            || s_RendererData->TextureSlotIndex == s_RendererData->MaxTextureSlots)
        {
            FlushAndReset();
            texIndex = GetOrAddTextureIndex(atlas);
        }
        FontCharacterinfo i = chars[*c];
        float x2 = xOffset + i.bitmapL * size.x;
        float y2 = -yOffset - i.bitmapT * size.y;
        float w = i.bitmapW * size.x;
        float h = i.bitmapR * size.y;

        xOffset += i.advanceX * size.x;
        yOffset += i.advanceY * size.y;

        s_RendererData->QuadVertexBufferPtr->Position = { (x2),
                                                           -y2,
                                                           pos.z };
        s_RendererData->QuadVertexBufferPtr->TexCoord = { i.xTexOffset ,0.0f };
        s_RendererData->QuadVertexBufferPtr->Colour = colour;
        s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->QuadVertexBufferPtr++;

        s_RendererData->QuadVertexBufferPtr->Position = { (x2 + w),
                                                           -y2,
                                                           pos.z };
        s_RendererData->QuadVertexBufferPtr->TexCoord = { i.xTexOffset + i.bitmapW / atlas->GetWidth() ,0.0f };
        s_RendererData->QuadVertexBufferPtr->Colour = colour;
        s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->QuadVertexBufferPtr++;

        s_RendererData->QuadVertexBufferPtr->Position = { (x2 + w),
                                                           -y2 - h,
                                                           pos.z };
        s_RendererData->QuadVertexBufferPtr->TexCoord = { i.xTexOffset + i.bitmapW / atlas->GetWidth(), i.bitmapR / atlas->GetHeight() };
        s_RendererData->QuadVertexBufferPtr->Colour = colour;
        s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->QuadVertexBufferPtr++;

        s_RendererData->QuadVertexBufferPtr->Position = { (x2),
                                                           -y2 - h,
                                                           pos.z };
        s_RendererData->QuadVertexBufferPtr->TexCoord = { i.xTexOffset  , i.bitmapR / atlas->GetHeight() };
        s_RendererData->QuadVertexBufferPtr->Colour = colour;
        s_RendererData->QuadVertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->QuadVertexBufferPtr++;

        s_RendererData->QuadIndexCount += 6;

    }
}
float Renderer::GetOrAddTextureIndex(const std::shared_ptr<Texture>& tex)
{
    float texIndex = 0.0f;
    for (uint32_t i = 1; i < s_RendererData->TextureSlotIndex; i++)
    {
        //Definitely fix this if statement later. Ugly as fuck
        if (*tex.get() == *s_RendererData->TextureSlots[i].get())
        {
            texIndex = (float)i;
            break;
        }
    }
    if (texIndex == 0.0f)
    {
        texIndex = (float)s_RendererData->TextureSlotIndex;
        s_RendererData->TextureSlots[s_RendererData->TextureSlotIndex] = tex;
        s_RendererData->TextureSlotIndex++;
    }
    return texIndex;
}

void Renderer::RenderUI(UIManager& manager, const std::shared_ptr<OrthoCamera>& camera)
{
    //TODO: Switch the members of the elements to public, having so many Get functions is stupid
    BeginScene(camera);

    std::vector<UIElement*> elements = manager.GetElements(); 
    std::vector<TextBox*> boxes;
    for (uint32_t i = 0; i < elements.size(); i++)
    {
        UIElement* e = elements[i];
        Renderer::DrawQuad({ e->location.x + e->size.x / 2.0f,  e->location.y + e->size.y / 2.0f, e->location.z }, e->size, e->colour);
        if (e->type == UIType::textBox)
        {
            boxes.push_back((TextBox*)e);
        }
    }
    
    EndScene();

    BeginScene(camera, 1);
    
    for (uint32_t i = 0; i < boxes.size(); i++)
    {
        TextBox* e = boxes[i];
        Renderer::RenderText({ e->location.x, e->location.y, e->location.z - 0.01f },
            { e->size.y / e->font->GetPtSize(),
            e->size.y / e->font->GetPtSize() },
            e->text, e->font, e->textColour);
    }
    EndScene();

}

//Used to start a new batch
void Renderer::FlushAndReset()
{
    EndScene();

    s_RendererData->QuadVertexBufferPtr = s_RendererData->QuadVertexBufferBase;
    s_RendererData->QuadIndexCount = 0;
    s_RendererData->TextureSlotIndex = 1;
}

