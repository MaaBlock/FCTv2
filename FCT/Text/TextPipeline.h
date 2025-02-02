#pragma once
#include "../Context/Pipeline.h"
namespace FCT {
	struct TextScreenInformation {
		Vec3 g_screenPosition;
		float blocked_0;
		Vec3 g_screenXHalfSize;
		float blocked_1;
		Vec3 g_screenYHalfSize;
		float blocked_2;
		Vec2 g_screenLTCoord;
		Vec2 g_screenRBCoord;
	};
	class TextPipeline : public Pipeline {
	public:
		TextPipeline(Context* ctx);
		~TextPipeline();
		void screen(Vec3 pos, Vec3 xHalfVec, Vec3 yHalfVec);
		void viewport(Vec2 lt, Vec2 rb);
		void beginDraw();
		void drawText(float x, float y, float w, float h);
		void endDraw();
	private:
		void updataScreenInformation();
		VertexFactory* createFactory();
		Context* m_ctx;
		std::vector<float> m_commandQueue;
		VertexFactory* m_vf;
		VertexShader* m_vs;
		PixelShader* m_ps;
		ConstBuffer* m_cb;
		Material* m_material;
		TextScreenInformation m_si;
		VertexArray* m_arr;
		VertexBuffer* m_vb;
		InputLayout* m_il;
		DrawCall* m_dc;
        Texture* m_transform;
		Mat4 m_cachedTransform;
	};
	/*
	ToDo:
	*/
}