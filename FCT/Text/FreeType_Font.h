#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_COLOR_H
#include "./Font.h"
namespace FCT {
	class FreeType_Font;
	class FreeType_FontShareData {
	public:
		void init();
		FreeType_Font* create();
		void tern();
		FT_Library g_library;
	};
	class FreeType_Font : public Font
	{
	public:
		FreeType_Font(FreeType_FontShareData* shareData);
		~FreeType_Font();
		bool create(const char* fontFamily);
		void translateGlyph(char32_t ch);
        const GlyphInfo* getGlyphInfo(char32_t ch) const;
	private:
		FT_Face m_face;
		FreeType_FontShareData* g_shareData;
		void outlineToCommands(FT_Outline* outline, std::vector<float>& commands);
	};
}

