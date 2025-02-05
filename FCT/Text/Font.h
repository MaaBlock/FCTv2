#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_COLOR_H
#include "../MutilThreadBase/RefCount.h"
#include <unordered_map>
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
        virtual bool create(const char *fontFamily) = 0;
        virtual void translateGlyph(FT_UInt id) = 0;
        virtual void translateGlyph(char32_t id) = 0;
        virtual const GlyphInfo *getGlyphInfo(char32_t ch) = 0;
        virtual const GlyphInfo *getGlyphInfo(FT_UInt id) = 0;
        virtual bool hasGlyph(FT_UInt id) const = 0;
        virtual bool hasGlyph(char32_t ch) const = 0;
        virtual void *getBackendObject() = 0;
        void translateChinese()
        {
            for (char32_t ch = 0x4E00; ch <= 0x9FFF; ++ch)
            {
                translateGlyph(ch);
            }
        }
        void translateCommonChineseSymbols()
        {
            const std::vector<std::pair<char32_t, char32_t>> symbolRanges = {
                {0x3000, 0x303F}, // CJK 符号和标点
                {0xFF00, 0xFFEF}, // 全角ASCII、全角标点、半宽片假名、半宽谚语片假名、全角韩文字母
                {0x2000, 0x206F}, // 常用标点
                {0x2100, 0x214F}, // 字母式符号
                {0x2190, 0x21FF}, // 箭头
                {0x2200, 0x22FF}, // 数学运算符
                {0x25A0, 0x25FF}, // 几何图形
            };

            const std::vector<char32_t> specificSymbols = {
                0x00B7,         // 中间点
                0x2014,         // 破折号
                0x2018, 0x2019, // 左右单引号
                0x201C, 0x201D, // 左右双引号
                0x2026,         // 省略号
                0x3001,         // 顿号
                0x3002,         // 句号
                0x3010, 0x3011, // 方括号
                0xFF01,         // 全角感叹号
                0xFF08, 0xFF09, // 全角括号
                0xFF0C,         // 全角逗号
                0xFF1A,         // 全角冒号
                0xFF1B,         // 全角分号
                0xFF1F,         // 全角问号
            };

            for (const auto &range : symbolRanges)
            {
                for (char32_t ch = range.first; ch <= range.second; ++ch)
                {
                    translateGlyph(ch);
                }
            }

            for (char32_t ch : specificSymbols)
            {
                translateGlyph(ch);
            }
        }
        void translateAlpha()
        {
            for (char32_t ch = 'A'; ch <= 'Z'; ++ch)
            {
                translateGlyph(ch);
            }
            for (char32_t ch = 'a'; ch <= 'z'; ++ch)
            {
                translateGlyph(ch);
            }
        }
        void translateNums()
        {
            for (char32_t ch = '0'; ch <= '9'; ++ch)
            {
                translateGlyph(ch);
            }
        }
        void translagtEmoji()
        {
            const std::vector<std::pair<char32_t, char32_t>> emojiRanges = {
                {0x1F600, 0x1F64F},
                {0x1F300, 0x1F5FF},
                {0x1F680, 0x1F6FF},
                {0x2600, 0x26FF},
                {0x2700, 0x27BF},
            };

            for (const auto &range : emojiRanges)
            {
                for (char32_t ch = range.first; ch <= range.second; ++ch)
                {
                    translateGlyph(ch);
                }
            }
        }
        float getDescender() {
            return m_descender;
        }
        float getLineHeight() {
            return m_height;
        }
    protected:
        std::unordered_map<FT_UInt, GlyphInfo> m_glyphMap;
        float m_ascender;
        float m_descender;
        float m_height;
    };
}