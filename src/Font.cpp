#include "Font.h"
#include <iostream>
#include <algorithm>
#include "Renderer.h"

Font::Font()
{
    //Initialize library
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    //load face 
    //TODO (Ben): take font as const string input
    FT_Face face;
    if (FT_New_Face(ft, "res/fonts/arial.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    //Load pixel size
    //TODO (Ben): Also take size as input
    FT_Set_Pixel_Sizes(face, 0, 72);
    m_fontptSize = 72;

    /*
    * Build a texture horizontally one character at a time,
    * total height will be maximum height of one character,
    * total width will be the sum of all the widths
    */
    FT_GlyphSlot g = face->glyph;
    
    uint32_t w = 0;
    uint32_t h = 0;

    for (uint32_t i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }

        w += g->bitmap.width;
        h = std::max(h, g->bitmap.rows);
    }

    /* you might as well save this value as it is needed later on */
    uint32_t atlas_width = w;

    //Create texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_atlas = std::make_shared<Texture>(w, h, GL_RED, GL_RED);
    m_atlas->Bind();
    //Load characters into texture
    uint32_t x = 0;


    for (uint32_t i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        m_chars[i].advanceX = g->advance.x >> 6;
        m_chars[i].advanceY = g->advance.y >> 6;

        m_chars[i].bitmapW = g->bitmap.width;
        m_chars[i].bitmapR = g->bitmap.rows;

        m_chars[i].bitmapL = g->bitmap_left;
        m_chars[i].bitmapT = g->bitmap_top;

        m_chars[i].xTexOffset = (float)x / w;
       
       
        m_atlas->SetData(g->bitmap.buffer, x, g->bitmap.width, g->bitmap.rows, GL_RED);
        x += g->bitmap.width;
    }
}
