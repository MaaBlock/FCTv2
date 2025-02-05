#include "../headers.h"
#include <iostream>
#include "FreeType_Font.h"

void FCT::FreeType_FontShareData::init()
{
	if (FT_Init_FreeType(&g_library))
		std::cout << "Could not initialize FreeType library" << std::endl;
}

FCT::FreeType_Font *FCT::FreeType_FontShareData::create()
{
	return new FreeType_Font(this);
}

void FCT::FreeType_FontShareData::tern()
{
	FT_Done_FreeType(g_library);
}
FCT::FreeType_Font::FreeType_Font(FreeType_FontShareData *shareData)
{
	g_shareData = shareData;
}

FCT::FreeType_Font::~FreeType_Font()
{
}
const FCT::GlyphInfo* FCT::FreeType_Font::getGlyphInfo(char32_t ch)
{
    FT_UInt glyph_index = FT_Get_Char_Index(m_face, ch);
    
    if (glyph_index == 0)
    {
        std::cout << "Character " << ch << " not found in font." << std::endl;
        return nullptr;
    }

    return getGlyphInfo(glyph_index);
}
bool FCT::FreeType_Font::create(const char *fontPath)
{
	if (FT_New_Face(g_shareData->g_library, fontPath, 0, &m_face))
	{
		std::cout << "Could not load font: " << fontPath << std::endl;
		return false;
	}

	FT_Bool hasColor = FT_HAS_COLOR(m_face);
	if (hasColor)
	{
		std::cout << "Font supports color glyphs." << std::endl;
	}
	else
	{
		std::cout << "Font does not support color glyphs." << std::endl;
	}

	if (m_face->num_fixed_sizes > 0)
	{
		std::cout << "Font has " << m_face->num_fixed_sizes << " fixed size(s)." << std::endl;

		FT_Error error = FT_Select_Size(m_face, 0);
		if (error)
		{
			std::cout << "Could not select font size. Error code: " << error << std::endl;
			FT_Done_Face(m_face);
			m_face = nullptr;
			return false;
		}
	}
	else
	{
		FT_Error error = FT_Set_Pixel_Sizes(m_face, 0, 64);
		if (error)
		{
			std::cout << "Could not set font pixel size. Error code: " << error << std::endl;
			FT_Done_Face(m_face);
			m_face = nullptr;
			return false;
		}
	}
	/*
	if (FT_Set_Char_Size(m_face, 0, 64 * 64, 72, 72))
	{
		std::cout << "Could not set font char size" << std::endl;
		FT_Done_Face(m_face);
		m_face = nullptr;
		return false;
	}*/
	m_ascender = m_face->ascender / 64.0f;
    m_descender = m_face->descender / 64.0f;
    m_height = m_face->height / 64.0f;
	return true;
}

void FCT::FreeType_Font::translateGlyph(char32_t ch)
{
	FT_UInt glyph_index = FT_Get_Char_Index(m_face, ch);
	if (glyph_index == 0)
	{
		std::cout << "Character " << ch << " not found in font." << std::endl;
		return;
	}
	translateGlyph(glyph_index);
}

void FCT::FreeType_Font::translateGlyph(FT_UInt glyph_index)
{
    if (m_glyphMap.find(glyph_index) != m_glyphMap.end())
        return;

    FT_Int32 load_flags = FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP;
    if (FT_HAS_COLOR(m_face))
    {
        load_flags |= FT_LOAD_COLOR;
    }

    if (FT_Load_Glyph(m_face, glyph_index, load_flags))
    {
        std::cout << "Failed to load glyph for glyph index: " << glyph_index << std::endl;
        return;
    }

    GlyphInfo glyphInfo;
    glyphInfo.advanceX = m_face->glyph->advance.x / 64.0f;
    glyphInfo.advanceY = m_face->glyph->advance.y / 64.0f;
    glyphInfo.bitmapWidth = m_face->glyph->metrics.width / 64.0f;
    glyphInfo.bitmapHeight = m_face->glyph->metrics.height / 64.0f;
    glyphInfo.bitmapLeft = m_face->glyph->metrics.horiBearingX / 64.0f;
    glyphInfo.bitmapTop = m_face->glyph->metrics.horiBearingY / 64.0f;

    outlineToCommands(&m_face->glyph->outline, glyphInfo.outlineCommands);

    m_glyphMap[glyph_index] = glyphInfo;
}

const FCT::GlyphInfo *FCT::FreeType_Font::getGlyphInfo(FT_UInt glyph_index)
{
    auto it = m_glyphMap.find(glyph_index);
    if (it != m_glyphMap.end())
        return &it->second;

    std::cout << "Warning: 没有已翻译的字形信息，现在翻译，建议提前进行翻译." << std::endl;
    translateGlyph(glyph_index);
	it = m_glyphMap.find(glyph_index);
	if (it != m_glyphMap.end())
		return &it->second;
	return NULL;
}
bool FCT::FreeType_Font::hasGlyph(FT_UInt glyph_index) const
{
    return m_glyphMap.find(glyph_index) != m_glyphMap.end();
}
bool FCT::FreeType_Font::hasGlyph(char32_t ch) const
{
    if (m_face == nullptr)
    {
        return false;
    }

    FT_UInt glyph_index = FT_Get_Char_Index(m_face, ch);
    
    return glyph_index != 0;
}
void *FCT::FreeType_Font::getBackendObject()
{
	return (void*)m_face;
}
void FCT::FreeType_Font::outlineToCommands(FT_Outline *outline, std::vector<float> &commands)
{

	FT_LayerIterator layer_iterator;
	FT_UInt layer_index;
	FT_UInt color_index;
	FT_Bool have_layers;

	layer_iterator.p = nullptr;
	have_layers = FT_Get_Color_Glyph_Layer(m_face, m_face->glyph->glyph_index, &layer_index, &color_index, &layer_iterator);
	commands.push_back(Command_BeginPath);
	commands.push_back(0);
	commands.push_back(Command_EndPath);
	do
	{
		commands.push_back(Command_BeginPath);
		commands.push_back(1);
		if (have_layers)
		{
			FT_Palette_Data palette_data;
			FT_Error palette_data_error = FT_Palette_Data_Get(m_face, &palette_data);
			if (palette_data_error == 0)
			{
				FT_Color *palette;
				FT_Error palette_select_error = FT_Palette_Select(m_face, 0, &palette);
				if (palette_select_error == 0 && color_index < palette_data.num_palette_entries)
				{
					FT_Color color = palette[color_index];
					commands.push_back(Command_SetColor);
					commands.push_back(color.red / 255.0f);
					commands.push_back(color.green / 255.0f);
					commands.push_back(color.blue / 255.0f);
					commands.push_back(color.alpha / 255.0f);
				}
			}
			FT_Error error = FT_Load_Glyph(m_face, layer_index, FT_LOAD_DEFAULT);
            if (error)
            {
                std::cout << "Error loading glyph layer: " << error << std::endl;
                continue;
            }
		}

		FT_Vector* points = outline->points;
		unsigned char* tags = outline->tags;
		unsigned short* contours = outline->contours;
		int n_points = outline->n_points;
		int n_contours = outline->n_contours;
		int start = 0;
		for (int i = 0; i < n_contours; i++)
		{
			int end = contours[i];
			Vec2 last;
			Vec2 control;
			bool hasControl = false;

			for (int j = start; j <= end; j++)
			{
				Vec2 point(points[j].x / 64.0f, points[j].y / 64.0f);

				if (j == start)
				{
					commands.push_back(Command_MoveTo);
					commands.push_back(point.x);
					commands.push_back(point.y);
					last = point;
				}
				else
				{
					if (FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON)
					{
						if (hasControl)
						{
							commands.push_back(Command_BezierCurveTo);
							commands.push_back(control.x);
							commands.push_back(control.y);
							commands.push_back(point.x);
							commands.push_back(point.y);
							//std::cout << "BezierCurveTo: (" << control.x << ", " << control.y << ") (" << point.x << ", " << point.y << ")" << std::endl;
							hasControl = false;
						}
						else
						{
							commands.push_back(Command_LineTo);
							commands.push_back(point.x);
							commands.push_back(point.y);
							//std::cout << "LineTo: (" << point.x << ", " << point.y << ")" << std::endl;
						}
						last = point;
					}
					else if (FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CONIC)
					{
						if (hasControl)
						{
							Vec2 mid((control.x + point.x) / 2, (control.y + point.y) / 2);
							commands.push_back(Command_BezierCurveTo);
							commands.push_back(control.x);
							commands.push_back(control.y);
							commands.push_back(mid.x);
							commands.push_back(mid.y);
							//std::cout << "BezierCurveTo (mid-point): (" << control.x << ", " << control.y << ") (" << mid.x << ", " << mid.y << ")" << std::endl;
							last = mid;
						}
						control = point;
						hasControl = true;
					}
					else if (FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CUBIC)
					{
						std::cout << "Warning: Cubic Bezier curves are not supported in this implementation." << std::endl;
					}
				}
			}
			if (hasControl)
			{
				Vec2 start_point(points[start].x / 64.0f, points[start].y / 64.0f);
				commands.push_back(Command_BezierCurveTo);
				commands.push_back(control.x);
				commands.push_back(control.y);
				commands.push_back(start_point.x);
				commands.push_back(start_point.y);
			}
			else
			{
				Vec2 start_point(points[start].x / 64.0f, points[start].y / 64.0f);
				commands.push_back(Command_LineTo);
				commands.push_back(start_point.x);
				commands.push_back(start_point.y);
			}

			start = end + 1;
		}

		// 结束当前颜色层的路径
		commands.push_back(Command_EndPath);

		// 处理下一个颜色层
		if (have_layers)
		{
			have_layers = FT_Get_Color_Glyph_Layer(m_face, m_face->glyph->glyph_index, &layer_index, &color_index, &layer_iterator);
		}
	} while (have_layers);
}
/*
void FCT::FreeType_Font::outlineToCommands(FT_Outline* outline, std::vector<float>& commands)
{
	FT_Vector* points = outline->points;
	unsigned char* tags = outline->tags;
	unsigned short* contours = outline->contours;
	int n_points = outline->n_points;
	int n_contours = outline->n_contours;

	int start = 0;
	for (int i = 0; i < n_contours; i++)
	{
		int end = contours[i];
		Vec2 last;
		Vec2 control;
		bool hasControl = false;

		for (int j = start; j <= end; j++)
		{
			Vec2 point(points[j].x / 64.0f, points[j].y / 64.0f);

			if (j == start)
			{
				commands.push_back(Command_MoveTo);
				commands.push_back(point.x);
				commands.push_back(point.y);
				last = point;
				std::cout << "MoveTo: (" << point.x << ", " << point.y << ")" << std::endl;
			}
			else
			{
				if (FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_ON)
				{
					if (hasControl)
					{
						commands.push_back(Command_BezierCurveTo);
						commands.push_back(control.x);
						commands.push_back(control.y);
						commands.push_back(point.x);
						commands.push_back(point.y);
						std::cout << "BezierCurveTo: (" << control.x << ", " << control.y << ") (" << point.x << ", " << point.y << ")" << std::endl;
						hasControl = false;
					}
					else
					{
						commands.push_back(Command_LineTo);
						commands.push_back(point.x);
						commands.push_back(point.y);
						std::cout << "LineTo: (" << point.x << ", " << point.y << ")" << std::endl;
					}
					last = point;
				}
				else if (FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CONIC)
				{
					if (hasControl)
					{
						Vec2 mid((control.x + point.x) / 2, (control.y + point.y) / 2);
						commands.push_back(Command_BezierCurveTo);
						commands.push_back(control.x);
						commands.push_back(control.y);
						commands.push_back(mid.x);
						commands.push_back(mid.y);
						std::cout << "BezierCurveTo (mid-point): (" << control.x << ", " << control.y << ") (" << mid.x << ", " << mid.y << ")" << std::endl;
						last = mid;
					}
					control = point;
					hasControl = true;
				}
				else if (FT_CURVE_TAG(tags[j]) == FT_CURVE_TAG_CUBIC)
				{
					std::cout << "Warning: Cubic Bezier curves are not supported in this implementation." << std::endl;
					// 如果需要，这里可以添加对三次贝塞尔曲线的处理
				}
			}
		}

		// 闭合轮廓
		if (hasControl)
		{
			Vec2 start_point(points[start].x / 64.0f, points[start].y / 64.0f);
			commands.push_back(Command_BezierCurveTo);
			commands.push_back(control.x);
			commands.push_back(control.y);
			commands.push_back(start_point.x);
			commands.push_back(start_point.y);
			std::cout << "Closing contour with BezierCurveTo: (" << control.x << ", " << control.y << ") (" << start_point.x << ", " << start_point.y << ")" << std::endl;
		}
		else
		{
			Vec2 start_point(points[start].x / 64.0f, points[start].y / 64.0f);
			commands.push_back(Command_LineTo);
			commands.push_back(start_point.x);
			commands.push_back(start_point.y);
			std::cout << "Closing contour with LineTo: (" << start_point.x << ", " << start_point.y << ")" << std::endl;
		}

		start = end + 1;
	}
}*/