#pragma once
#include "../Context/DataTypes.h"
#include "./Font.h"
#include <string>
#include <vector>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace FCT
{
    struct Rect
    {
        Vec2 min;
        Vec2 size;
        Rect() {
			min = Vec2(0, 0);
			size = Vec2(0, 0);
        }
        Rect(Vec2 min, Vec2 size) {
			this->min = min;
			this->size = size;
        }
    };
    class CharLayout {
    public:

    private:

    };
    class LineLayout {
    public:
    private:

    };
    class TextLayout
    {
    public:
        enum class VerticalAlignment
        {
            Top,
            Middle,
            Bottom
        };
        enum class HorizontalAlignment
        {
            Left,
            Center,
            Right
        };
        struct LayoutRet {
            Font* font;
            Vec2 pos;       
            Vec2 size;      
            FT_UInt id;     
            Vec2 scale;     
            Mat4 transform; 
            Rect bbox;      
        };
        TextLayout();
        ~TextLayout();
        void textSize(Vec2 size);
        void addFont(Font *font);
        void layoutRect(Vec2 lt, Vec2 rb);
        void layoutText(const std::u32string &text);
        std::vector<LayoutRet> layout();

    private:
        Vec2 m_textSize = Vec2(18, 18);
        std::vector<Font *> m_fonts;
        Vec2 m_layoutRectLT;
        Vec2 m_layoutRectRB;
        std::u32string m_text;
        VerticalAlignment m_verticalAlignment = VerticalAlignment::Top;
        HorizontalAlignment m_horizontalAlignment = HorizontalAlignment::Left;
        float m_lineSpacing = 0;
    };

} // namespace FCT