#pragma once
#include "../Context/Pipeline.h"
#include "./TextLayout.h"
#include <boost/locale.hpp>
namespace FCT
{
	struct TextScreenInformation
	{
		Vec3 g_screenPosition;
		float blocked_0;
		Vec3 g_screenXHalfSize;
		float blocked_1;
		Vec3 g_screenYHalfSize;
		float blocked_2;
		Vec2 g_screenLTCoord;
		Vec2 g_screenRBCoord;
	};
	class TextPipeline : public Pipeline
	{
	public:
		TextPipeline(Context *ctx);
		~TextPipeline();
		void screen(Vec3 pos, Vec3 xHalfVec, Vec3 yHalfVec);
		void viewport(Vec2 lt, Vec2 rb);
		void addFont(Font *font);
		void beginDraw();
		void color(Vec4 color);
		void drawText(const char32_t *str, float x, float y, float w, float h);
		void flush();
		void toggleOriginVertical(bool toggle);
		void updataScreenInformation();
		void drawText(const std::string &str, float x, float y)
		{
			boost::locale::generator gen;
			std::locale loc = gen("");

			std::u32string u32str = boost::locale::conv::utf_to_utf<char32_t>(str);
			drawText(u32str.c_str(), x, y, 0, 0);
		}

	private:
		VertexFactory *createFactory();
		Context *m_ctx;
		std::vector<float> m_commandQueue;
		VertexFactory *m_vf;
		VertexShader *m_vs;
		PixelShader *m_ps;
		ConstBuffer *m_cb;
		Material *m_material;
		TextScreenInformation m_si;
		VertexArray *m_arr;
		VertexBuffer *m_vb;
		InputLayout *m_il;
		DrawCall *m_dc;
		Texture *m_transform;
		Texture *m_commandTexture;
		Mat4 m_cachedTransform;
		std::vector<Font*> m_fonts;
		Vec4 m_color;
	};
	/*
	ToDo:
	*/
}