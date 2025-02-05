#include "TextLayout.h"
#include <algorithm>
#include <iostream>
#include <string_view>
#include "..\Tools\Timer.h"

namespace FCT
{
    TextLayout::TextLayout()
    {
    }
    TextLayout::~TextLayout()
    {
    }
    void TextLayout::textSize(Vec2 size)
    {
        m_textSize = size;
        m_cacheScale.clear();
        for (auto font : m_fonts)
        {
            Vec2 scale;
            auto height = font->getLineHeight() * 2.5;
            scale.y = m_textSize.y / height;
            scale.x = scale.y;
            m_cacheScale.push_back(scale);
        }
    }
    void TextLayout::addFont(Font *font)
    {
        m_fonts.push_back(font);
        Vec2 scale;
        auto height = font->getLineHeight() * 2.5;
        scale.y = m_textSize.y / height;
        scale.x = scale.y;
        m_cacheScale.push_back(scale);
    }

    void TextLayout::layoutRect(Vec2 lt, Vec2 rb)
    {
        m_layoutRectLT = lt;
        m_layoutRectRB = rb;
    }

    void TextLayout::layoutText(const std::u32string &text)
    {
        m_text = text;
    }
    std::vector<TextLayout::LayoutRet> TextLayout::layout()
    {
        //AutoTimer timer("layoutText");
        std::vector<TextLayout::LayoutRet> rets;
        Vec2 currentPos = m_layoutRectLT;
        for (auto it : m_text)
        {
            FT_UInt glyphIndex;
            LayoutRet ret;
            Font *currenFont = NULL;
            size_t fontSize = m_fonts.size();
            for (size_t i = 0; i < fontSize; i++)
            {
                auto font = m_fonts[i];
                FT_Face face = (FT_Face)font->getBackendObject();
                glyphIndex = FT_Get_Char_Index(face, it);
                if (glyphIndex)
                {
                    currenFont = font;
                    ret.scale = m_cacheScale[i];
                    break;
                }
            }
            if (!glyphIndex)
            {
                continue;
            }
            auto glyph = currenFont->getGlyphInfo(glyphIndex);
            if (!glyph)
            {
                continue;
            }
            ret.pos.x = currentPos.x;
            ret.pos.y = currentPos.y + (m_textSize.y + currenFont->getDescender() * ret.scale.y * 3.0);
            ret.bbox.min = Vec2(ret.pos.x, currentPos.y);
            ret.bbox.size = Vec2(glyph->advanceX * ret.scale.x, m_textSize.y);
            ret.transform = Mat4(
                ret.scale.x, 0, 0, 0,
                0, -ret.scale.y, 0, 0,
                ret.pos.x, ret.pos.y, 1, 0,
                0, 0, 0, 1);
            ret.font = currenFont;
            ret.id = glyphIndex;
            rets.push_back(ret);
            currentPos.x += glyph->advanceX * ret.scale.x;
        }
        return rets;
    }

} // namespace FCT