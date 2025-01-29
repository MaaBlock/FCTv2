#pragma once
#include "./Pipeline.h"
namespace FCT {
	class VertexRenderPipeline;
	class VertexRenderScreen : public Object {
	public:
		VertexRenderScreen(VertexRenderPipeline* pl);
		void size(Vec3 halfVec);
		void viewport(float x,float y, float w, float h);
		void create();
	private:
		VertexRenderPipeline* m_pl;
		VertexFactory* m_vf;
		Context* m_ctx;
		VertexArray* m_arr;
		InputLayout* m_il;
		VertexBuffer* m_vb;
		DrawCall* m_dc;
	};
	class VertexRenderPipeline : public Pipeline {
	public:
		VertexRenderPipeline(Context* context);
		void begin(VertexRenderScreen* screen);
		void setColor(Vec4 color);
		void moveTo(Vec2 to);
		void lineTo(Vec2 to);
		void fillrect(Vec2 pos, Vec2 size);
		void rectangle(Vec2 pos, Vec2 size, float width);
		void arcTo(Vec2 center, float beginAngle, float endAngle);
		void end();
	private:
		void unwrapperArcTo(Vec2 center, float beginAngle, float endAngle, float q);
		VertexShader* m_vrvs;
		PixelShader* m_vrps;
		VertexFactory* m_vrf;
		Material* m_vrMaterial;
		VertexRenderScreen* m_screeen;
		VertexFactory* createFactory();
		std::vector<float> m_commandQueue;
	};
}