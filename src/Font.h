#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <array>
#include "Texture.h"
struct FontCharacterinfo {
    float advanceX; // advance.x
    float advanceY; // advance.y

    float bitmapW; // bitmap.width;
    float bitmapR; // bitmap.rows;

    float bitmapL; // bitmap_left;
    float bitmapT; // bitmap_top;

    float xTexOffset; // x offset of glyph in texture coordinates
};
class Font
{
private:
    std::array<FontCharacterinfo,128> m_chars;
    std::shared_ptr<Texture> m_atlas;
    uint32_t m_fontptSize;
public:
    Font();
   
    std::shared_ptr<Texture> GetAtlas() const { return m_atlas; }
    std::array<FontCharacterinfo,128>& GetChars() { return m_chars; }
    uint32_t GetPtSize() { return m_fontptSize; }
};