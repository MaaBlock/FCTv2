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
		void translateGlyph(FT_UInt id);
        const GlyphInfo* getGlyphInfo(FT_UInt id);
        const GlyphInfo* getGlyphInfo(char32_t ch);
		bool hasGlyph(FT_UInt id) const override;
		bool hasGlyph(char32_t ch) const override;
        void* getBackendObject() override;
	private:
		FT_Face m_face;
		FreeType_FontShareData* g_shareData;
		void outlineToCommands(FT_Outline* outline, std::vector<float>& commands);

	};
}

