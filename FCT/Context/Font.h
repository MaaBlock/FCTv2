#pragma once
#include "../MutilThreadBase/RefCount.h"
#include <map>
#include <vector>
namespace FCT {
	struct GlyphInfo {
        float advanceX;
        float advanceY;
        float bitmapWidth;
        float bitmapHeight;
        float bitmapLeft;
        float bitmapTop;
        std::vector<float> outlineCommands;
    };
	class Font : public RefCount {
	public:
		virtual bool create(const char* fontFamily) = 0;
		virtual void translateGlyph(char32_t ch) = 0;
		virtual const GlyphInfo* getGlyphInfo(char32_t ch) const = 0;
	protected:
        std::map<char32_t, GlyphInfo> m_glyphMap;

	};
}