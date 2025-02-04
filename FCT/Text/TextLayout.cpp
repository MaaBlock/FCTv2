#include "TextLayout.h"
#include <algorithm>
#include <iostream>
#include <string_view>

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
    }
    void TextLayout::addFont(Font *font)
    {
        m_fonts.push_back(font);
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
    std::vector<TextLayout::LayoutRet> TextLayout::layout() {
        std::vector<TextLayout::LayoutRet> rets;
		Vec2 currentPos = m_layoutRectLT;
        for (auto it : m_text) {
            FT_UInt glyphIndex;
            Font* currenFont = NULL;
			for (auto font : m_fonts) {
				FT_Face face = (FT_Face)font->getBackendObject();
                glyphIndex = FT_Get_Char_Index(face, it);
				if (glyphIndex) {
                    currenFont = font;
                    break;
				}
			}
            if (!glyphIndex) {
                continue;
            }
			auto glyph = currenFont->getGlyphInfo(glyphIndex);
			if (!glyph) {
				continue;
			}
			LayoutRet ret;
            ret.scale.y = m_textSize.y / glyph->bitmapHeight;
            ret.scale.x = ret.scale.y;
			ret.pos.x = currentPos.x;
			ret.pos.y = currentPos.y + glyph->bitmapTop * ret.scale.y;
			ret.bbox.min = Vec2(ret.pos.x, currentPos.y);
			ret.bbox.size = Vec2(glyph->advanceX * ret.scale.x, glyph->bitmapHeight * ret.scale.y);
			ret.transform = Mat4::Scale(ret.scale.x,-ret.scale.y) * Mat4::Translate(ret.pos.x, ret.pos.y);
            ret.font = currenFont;
            ret.id = glyphIndex;
            rets.push_back(ret);
            currentPos.x += glyph->advanceX * ret.scale.x;
        }
        return rets;
    }

} // namespace FCT