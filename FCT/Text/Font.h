#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_COLOR_H
#include "../MutilThreadBase/RefCount.h"
#include <map>
#include <vector>
namespace FCT
{
    struct GlyphInfo
    {
        float advanceX;
        float advanceY;
        float bitmapWidth;
        float bitmapHeight;
        float bitmapLeft;
        float bitmapTop;
        std::vector<float> outlineCommands;
    };
    class Font : public RefCount
    {
    public:

        virtual ~Font() {}
        virtual bool create(const char* fontFamily) = 0;
        virtual void translateGlyph(FT_UInt id) = 0;
        virtual void translateGlyph(char32_t id) = 0;
        virtual const GlyphInfo* getGlyphInfo(char32_t ch) = 0;
        virtual const GlyphInfo* getGlyphInfo(FT_UInt id) = 0;
        virtual bool hasGlyph(FT_UInt id) const = 0;
        virtual bool hasGlyph(char32_t ch) const = 0;
        virtual void* getBackendObject() = 0;
    protected:
        std::map<FT_UInt, GlyphInfo> m_glyphMap;
    };
}