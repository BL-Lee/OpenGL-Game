
#include <GL/glew.h>
#include "Renderer.h"
#include "GLErrorCalls.h"
#include "glm/gtx/rotate_vector.hpp"
#include <array>

struct Vertex
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

    uint32_t IndexCount = 0;

    Vertex* VertexBufferBase = nullptr;
    Vertex* VertexBufferPtr = nullptr;

    uint32_t* IndexBufferBase = nullptr;
    uint32_t IndexOffset = 0;
    std::array<std::shared_ptr<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex;

    glm::vec4 VertexPositions[7];

};


static RendererData* s_RendererData;



void Renderer::Init()
{

    s_RendererData = new RendererData();
    s_RendererData->vertArray = std::make_shared<VertexArray>();

    std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>(sizeof(Vertex) * s_RendererData->MaxVertices);
    std::shared_ptr<VertexBufferLayout> layout = std::make_shared<VertexBufferLayout>();
    layout->Push<float>(3, GL_FALSE); //location
    layout->Push<float>(2, GL_FALSE); //Tex Coord
    layout->Push<float>(4, GL_FALSE); //Colour
    layout->Push<float>(1, GL_FALSE); //tex index
    vb->SetLayout(layout);

    s_RendererData->IndexBufferBase = new uint32_t[s_RendererData->MaxIndices];

    std::shared_ptr<IndexBuffer> ib = std::make_shared<IndexBuffer>(s_RendererData->MaxIndices);
    s_RendererData->vertArray->SetIndexBuffer(ib);

    s_RendererData->VertexBufferBase = new Vertex[s_RendererData->MaxVertices];

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

    s_RendererData->VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_RendererData->VertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
    s_RendererData->VertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
    s_RendererData->VertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

    //s_RendererData->VertexPositions[4] = {  0.0f,            1.0f, 0.0f, 1.0f };
    //s_RendererData->VertexPositions[5] = {  0.86602540378f, -0.5f, 0.0f, 1.0f };
    //s_RendererData->VertexPositions[6] = { -0.86602540378f, -0.5f, 0.0f, 1.0f };
    s_RendererData->VertexPositions[4] = { 1.0f,            0.0f, 0.0f, 1.0f };
    s_RendererData->VertexPositions[5] = { -0.5f,  -0.86602540378f, 0.0f, 1.0f };
    s_RendererData->VertexPositions[6] = { -0.5f, 0.86602540378f, 0.0f, 1.0f };
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

    s_RendererData->VertexBufferPtr = s_RendererData->VertexBufferBase;
    s_RendererData->IndexCount = 0;
    s_RendererData->IndexOffset = 0;

    s_RendererData->TextureSlotIndex = 1;
}
void Renderer::BeginScene(const std::shared_ptr<OrthoCamera>& camera, uint32_t renderType)
{
    //Honestly this is a big no no. Its not scalable for other shaders
    //Later should implement a way to render using multiple shaders, sort them and make
    //draw calls based on those.
    //This would also fix the issue of starting a scene and ending a scene multiple times
    //Maybe something like 
    // Start -> add all your shapes regardless of shader -> Draw() -> sorts added stuff in order of shaders -> DrawQuad() and stuff
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
    

    s_RendererData->VertexBufferPtr = s_RendererData->VertexBufferBase;
    s_RendererData->IndexCount = 0;
    s_RendererData->IndexOffset = 0;

    s_RendererData->TextureSlotIndex = 1;
}

void Renderer::EndScene()
{

    uint32_t dataSize = (uint8_t*)s_RendererData->VertexBufferPtr - (uint8_t*)s_RendererData->VertexBufferBase;
    s_RendererData->vertBuffer->SetData((const void*)s_RendererData->VertexBufferBase, dataSize);
    
    s_RendererData->vertArray->GetIndexBuffer()->SetData(s_RendererData->IndexBufferBase, s_RendererData->IndexCount);

    Flush();
}

void Renderer::Flush()
{
    for (uint32_t i = 0; i < s_RendererData->TextureSlotIndex; i++)
    {
        s_RendererData->TextureSlots[i]->Bind(i);
    }
    glDrawElements(GL_TRIANGLES, s_RendererData->IndexCount, GL_UNSIGNED_INT, nullptr);
}


void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
{
    //Probably not the best...
    //This searches for the blank texture again
    DrawQuad(pos, size, s_RendererData->TextureSlots[0], colour);
}

void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
{

    if (s_RendererData->IndexCount >= s_RendererData->MaxIndices 
        || s_RendererData->TextureSlotIndex  == s_RendererData->MaxTextureSlots)
        FlushAndReset();

    float texIndex = GetOrAddTextureIndex(tex);

    //Don't really need to do a matrix multiplication if we're not rotating
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)      
        * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[0];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f,0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[1];
    s_RendererData->VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[2];
    s_RendererData->VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[3];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 0] = 0 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 1] = 1 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 2] = 2 + s_RendererData->IndexOffset;
                                                                                
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 3] = 2 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 4] = 3 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 5] = 0 + s_RendererData->IndexOffset;
    s_RendererData->IndexOffset += 4;

    s_RendererData->IndexCount += 6;
}

void Renderer::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
{

    if (s_RendererData->IndexCount >= s_RendererData->MaxIndices
        || s_RendererData->TextureSlotIndex == s_RendererData->MaxTextureSlots)
        FlushAndReset();

    float texIndex = GetOrAddTextureIndex(tex);

    //can just explicitly write out the matrix most likely THEN DO THAT
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[0];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f,0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[1];
    s_RendererData->VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[2];
    s_RendererData->VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[3];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 0] = 0 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 1] = 1 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 2] = 2 + s_RendererData->IndexOffset;
                                                    
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 3] = 2 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 4] = 3 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 5] = 0 + s_RendererData->IndexOffset;
    s_RendererData->IndexOffset += 4;

    s_RendererData->IndexCount += 6;

 
}

void Renderer::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotation ,const glm::vec4& colour)
{
    DrawRotatedQuad(pos, size, rotation, s_RendererData->TextureSlots[0], colour);
}

void Renderer::DrawTriangle(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
{
    DrawTriangle(pos, size, s_RendererData->TextureSlots[0], colour);
}
void Renderer::DrawTriangle(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
{
    if (s_RendererData->IndexCount >= s_RendererData->MaxIndices
        || s_RendererData->TextureSlotIndex == s_RendererData->MaxTextureSlots)
        FlushAndReset();

    float texIndex = GetOrAddTextureIndex(tex);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
        * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[4];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f,0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[5];
    s_RendererData->VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[6];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 0] = 0 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 1] = 1 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 2] = 2 + s_RendererData->IndexOffset;
    s_RendererData->IndexOffset += 3;

    s_RendererData->IndexCount += 3;
}

void Renderer::DrawRotatedTriangle(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& colour)
{
    DrawRotatedTriangle(pos, size, rotation, s_RendererData->TextureSlots[0], colour);
}
void Renderer::DrawRotatedTriangle(const glm::vec3& pos, const glm::vec2& size, float rotation, const std::shared_ptr<Texture>& tex, const glm::vec4& colour)
{
    if (s_RendererData->IndexCount >= s_RendererData->MaxIndices
        || s_RendererData->TextureSlotIndex == s_RendererData->MaxTextureSlots)
        FlushAndReset();

    float texIndex = GetOrAddTextureIndex(tex);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
        * glm::rotate(glm::mat4(1.0f),rotation, { 0.0f,0.0f,1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[4];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f,0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[5];
    s_RendererData->VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->VertexBufferPtr->Position = transform * s_RendererData->VertexPositions[6];
    s_RendererData->VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
    s_RendererData->VertexBufferPtr->Colour = colour;
    s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
    s_RendererData->VertexBufferPtr++;

    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 0] = 0 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 1] = 1 + s_RendererData->IndexOffset;
    s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 2] = 2 + s_RendererData->IndexOffset;
    s_RendererData->IndexOffset += 3;

    s_RendererData->IndexCount += 3;
}

void Renderer::DrawLine(const glm::vec3& start, const glm::vec3& end, float width, const glm::vec4& colour)
{
    glm::vec3 line = start - end;
    if (start.x > end.x)
    {
        line = end - start;
    } 
    float lineLength = glm::length(start-end);
    float angle = glm::acos(glm::dot(glm::normalize(line), { 0.0,1.0f,0.0 }));
   
    DrawRotatedQuad((start+end)/2.0f, { width, lineLength },  angle, colour);
}

/**
* Function to write a string of text
* 
* TODO:
*      flags: bit 0 is whether to wrap or not (for now this will default to wrap at the parent size.)
*             bit 1 is whether to 'lock' the text to the bottom left so that when it wraps it will 'push up' the previous line
*       Will also need to wrap on words, not letters as it does right now by default
* 
* TODO: weird 1 pixel artifact on sides of some letters? (try e in arial)
* Obviously this only works for languages that write left to right
*/
void Renderer::RenderText(const glm::vec3& pos, const glm::vec2& size, const char* text, const std::shared_ptr<Font>& f, const glm::vec4& colour)
{
    
    std::shared_ptr<Texture> atlas = f->GetAtlas();

    float texIndex = GetOrAddTextureIndex(atlas);

    float xOffset = pos.x;
    float yOffset = pos.y;

    glm::vec2 lettersize = { size.y / f->GetPtSize(), size.y / f->GetPtSize() };
    float lineLength = 0.0f;

    char c;
    const char* p = text;
    std::array<FontCharacterinfo,128>& chars = f->GetChars();
    for (c = *p; c; c = *++p)
    {
        //Check to make sure this doesnt need to be put in a new batch
        if (s_RendererData->IndexCount >= s_RendererData->MaxIndices
            || s_RendererData->TextureSlotIndex == s_RendererData->MaxTextureSlots)
        {
            FlushAndReset();
            texIndex = GetOrAddTextureIndex(atlas);
        }
        FontCharacterinfo i = chars[c];
        
        float x2 = xOffset + i.bitmapL * lettersize.x;
        float y2 = -yOffset - i.bitmapT * lettersize.y;
        float w = i.bitmapW * lettersize.x;
        float h = i.bitmapR * lettersize.y;

        //Wrap
        lineLength += w;
        if (lineLength >= size.x)
        {
            yOffset -= size.y;
            xOffset = pos.x;
            lineLength = 0.0f;
        }

        xOffset += i.advanceX * lettersize.x;
        yOffset += i.advanceY * lettersize.y;

        s_RendererData->VertexBufferPtr->Position = { (x2),
                                                           -y2,
                                                           pos.z };
        s_RendererData->VertexBufferPtr->TexCoord = { i.xTexOffset ,0.0f };
        s_RendererData->VertexBufferPtr->Colour = colour;
        s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->VertexBufferPtr++;

        s_RendererData->VertexBufferPtr->Position = { (x2 + w),
                                                           -y2,
                                                           pos.z };
        s_RendererData->VertexBufferPtr->TexCoord = { i.xTexOffset + i.bitmapW / atlas->GetWidth() ,0.0f };
        s_RendererData->VertexBufferPtr->Colour = colour;
        s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->VertexBufferPtr++;

        s_RendererData->VertexBufferPtr->Position = { (x2 + w),
                                                           -y2 - h,
                                                           pos.z };
        s_RendererData->VertexBufferPtr->TexCoord = { i.xTexOffset + i.bitmapW / atlas->GetWidth(), i.bitmapR / atlas->GetHeight() };
        s_RendererData->VertexBufferPtr->Colour = colour;
        s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->VertexBufferPtr++;

        s_RendererData->VertexBufferPtr->Position = { (x2),
                                                           -y2 - h,
                                                           pos.z };
        s_RendererData->VertexBufferPtr->TexCoord = { i.xTexOffset  , i.bitmapR / atlas->GetHeight() };
        s_RendererData->VertexBufferPtr->Colour = colour;
        s_RendererData->VertexBufferPtr->TextureIndex = texIndex;
        s_RendererData->VertexBufferPtr++;

        s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 0] = 0 + s_RendererData->IndexOffset;
        s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 1] = 1 + s_RendererData->IndexOffset;
        s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 2] = 2 + s_RendererData->IndexOffset;

        s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 3] = 2 + s_RendererData->IndexOffset;
        s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 4] = 3 + s_RendererData->IndexOffset;
        s_RendererData->IndexBufferBase[s_RendererData->IndexCount + 5] = 0 + s_RendererData->IndexOffset;
        s_RendererData->IndexOffset += 4;

        s_RendererData->IndexCount += 6;

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
            e->size,
            e->text, e->font, e->textColour);
    }
    EndScene();

}

//Used to start a new batch
void Renderer::FlushAndReset()
{
    EndScene();

    s_RendererData->VertexBufferPtr = s_RendererData->VertexBufferBase;
    s_RendererData->IndexCount = 0;

    s_RendererData->TextureSlotIndex = 1;
}

