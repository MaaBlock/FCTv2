#pragma once
#include "../Context/DataTypes.h"
#include "./Font.h"
#include <string>
#include <vector>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
namespace FCT {

class TextLayout {
public:
    enum class Alignment {
        Left,
        Center,
        Right
    };

    struct PositionedGlyph {
        GlyphInfo info;
        Vec3 position;  // 3D position within the rectangle
        Vec2 size;      // Size of the glyph in 3D space
    };

    TextLayout();
    ~TextLayout();

    void setFont(Font* font);
    void setText(const std::string& text);
    void setRectangle(const Vec3& position, const Vec3& size);
    void setAlignment(Alignment alignment);
    void layout();

    const std::vector<PositionedGlyph>& getPositionedGlyphs() const;

private:
    Font* m_font;
    std::string m_text;
    Vec3 m_rectPosition;
    Vec3 m_rectSize;
    Alignment m_alignment;
    std::vector<PositionedGlyph> m_positionedGlyphs;

    hb_buffer_t* m_hbBuffer;
    hb_font_t* m_hbFont;
    void calculateGlyphPositions();
    void cleanupHarfbuzz();
};

}  // namespace FCT
