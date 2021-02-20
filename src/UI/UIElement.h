#pragma once
#include "glm/glm.hpp"
#include "../Font.h"
#include <string>

enum UIType
{
    base,
    textBox
};
class UIElement
{
public:
    glm::vec3 location;
    glm::vec2 size;
    glm::vec4 colour;
    UIType type;

    UIElement(const glm::vec3&  l, const glm::vec2& s, const glm::vec4& c)
      :location(l), size(s), colour(c), type(UIType::base){}

     bool IsInBounds(float x, float y)
     {
       return (x >= location.x && x <= (location.x + size.x) && y >= location.y && y <= (location.y + size.y));
     }
     void SetLayer(float layer) { location.z = layer; }
     void SetLayer(int layer) { location.z = (float)layer; }

};

class TextBox : public UIElement
{
public:
    char* text;    
    const std::shared_ptr<Font>& font;
    float textSize;
    glm::vec4 textColour;
    int textLength;
    TextBox(const glm::vec3 l, const glm::vec2 s, const glm::vec4 c, const char* t, const std::shared_ptr<Font>& fnt, glm::vec4 txtC = { 1.0f,1.0f,1.0f,1.0f })
        :UIElement(l, s, c), text(), font(fnt), textSize(1.0f), textColour(txtC)
    {
        text = (char*)malloc(strlen(t) + 1);
        textLength = strlen(text + 1);
        strcpy_s(text, textLength, t);        
        type = UIType::textBox;
    }
    void SetText(char* t)
    {
        strcpy_s(text, textLength,  t);
    }
    void SetTextSize(float s) { textSize = s; }

    

};
