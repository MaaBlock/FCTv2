#include "TextLayout.h"
#include <algorithm>

namespace FCT {

TextLayout::TextLayout()
    : m_font(nullptr), m_alignment(Alignment::Left), m_hbBuffer(nullptr), m_hbFont(nullptr) {
    m_hbBuffer = hb_buffer_create();
}

TextLayout::~TextLayout() {
    cleanupHarfbuzz();
}
void TextLayout::setFont(Font* font) {
    m_font = font;
    cleanupHarfbuzz();
    if (m_font) {
        // Assuming your Font class has a method to get the FT_Face
        FT_Face ftFace = NULL;//m_font->getFTFace();
        m_hbFont = hb_ft_font_create(ftFace, nullptr);
    }
}

void TextLayout::setText(const std::string& text) {
    m_text = text;
}

void TextLayout::setRectangle(const Vec3& position, const Vec3& size) {
    m_rectPosition = position;
    m_rectSize = size;
}

void TextLayout::setAlignment(Alignment alignment) {
    m_alignment = alignment;
}

void TextLayout::layout() {
    if (!m_font || m_text.empty() || !m_hbFont) return;

    m_positionedGlyphs.clear();

    hb_buffer_reset(m_hbBuffer);
    hb_buffer_add_utf8(m_hbBuffer, m_text.c_str(), -1, 0, -1);
    hb_buffer_guess_segment_properties(m_hbBuffer);

    hb_shape(m_hbFont, m_hbBuffer, nullptr, 0);

    unsigned int glyphCount;
    hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(m_hbBuffer, &glyphCount);
    hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(m_hbBuffer, &glyphCount);

    for (unsigned int i = 0; i < glyphCount; ++i) {
        PositionedGlyph pg;
        pg.info = *m_font->getGlyphInfo(glyphInfo[i].codepoint);
        pg.position = Vec3(glyphPos[i].x_offset / 64.0f, -glyphPos[i].y_offset / 64.0f, 0);
        pg.size = Vec2(pg.info.bitmapWidth, pg.info.bitmapHeight);
        m_positionedGlyphs.push_back(pg);
    }

    calculateGlyphPositions();
}

void TextLayout::calculateGlyphPositions() {
    float totalWidth = 0;
    float maxHeight = 0;

    for (const auto& glyph : m_positionedGlyphs) {
        totalWidth += glyph.info.advanceX;
        maxHeight = std::max(maxHeight, glyph.info.bitmapHeight);
    }

    float startX = m_rectPosition.x;
    if (m_alignment == Alignment::Center) {
        startX += (m_rectSize.x - totalWidth) / 2;
    } else if (m_alignment == Alignment::Right) {
        startX += m_rectSize.x - totalWidth;
    }

    float startY = m_rectPosition.y + (m_rectSize.y - maxHeight) / 2;
    float startZ = m_rectPosition.z;

    float currentX = startX;
    for (auto& glyph : m_positionedGlyphs) {
        glyph.position.x = currentX + glyph.info.bitmapLeft + glyph.position.x;
        glyph.position.y = startY + maxHeight - glyph.info.bitmapTop - glyph.position.y;
        glyph.position.z = startZ;
        currentX += glyph.info.advanceX;
    }
}

const std::vector<TextLayout::PositionedGlyph>& TextLayout::getPositionedGlyphs() const {
    return m_positionedGlyphs;
}

void TextLayout::cleanupHarfbuzz() {
    if (m_hbFont) {
        hb_font_destroy(m_hbFont);
        m_hbFont = nullptr;
    }
}
}  // namespace FCT
