#include "../headers.h"
#include <iostream>
namespace FCT
{
	TextPipeline::TextPipeline(Context *ctx) : Pipeline(ctx, createFactory())
	{
		m_transform = ctx->createTexture();
        m_transform->setSlot(0);
        m_transform->create(4, 1, FCT::Texture::Format::RGBA32F);
		m_transform->setData(&m_cachedTransform,sizeof(m_cachedTransform));
		m_ctx = ctx;
		m_vs = ctx->createVertexShader(m_vf);
		m_vs->addCustomOutput(FCT::PipelineAttributeType::Position3f, "position");
		if (!m_vs->compileFromSource(R"(
layout(std140,binding = 5) uniform TextScreenInformation{
	vec3 g_screenPosition;
	vec3 g_screenXHalfSize;
	vec3 g_screenYHalfSize;
    vec2 g_screenLTCoord;
    vec2 g_screenRBCoord;
};
VertexOutput main(VertexInput vs_input) {
    VertexOutput vs_output;

    vec2 texCoordRange = g_screenRBCoord - g_screenLTCoord;
    float s = (vs_input.vectorCoord.x - g_screenLTCoord.x) / texCoordRange.x;
    float t = (vs_input.vectorCoord.y - g_screenLTCoord.y) / texCoordRange.y;

    vec3 xOffset = (s * 2.0 - 1.0) * g_screenXHalfSize;
    vec3 yOffset = (1.0 - t * 2.0) * g_screenYHalfSize;

    vs_output.position = g_screenPosition + xOffset + yOffset;
    vs_output.vectorCoord = vs_input.vectorCoord;
	vs_output.commandOffset = vs_input.commandOffset;
	vs_output.commandSize = vs_input.commandSize;

    return vs_output;
}
)"))
		{
			std::cout << "vs±¨´í:" << std::endl;
			std::cout << m_vs->getCompileError() << std::endl;
		}
		m_ps = ctx->createPixelShader(m_vs->getOutput());
		if (!m_ps->compileFromSource(R"(
PixelOutput main(PixelInput ps_input) {
	PixelOutput ps_output;
	ps_output.color = vec4(1,1,1,1);
	return ps_output;
}
)"))
		{
			std::cout << "ps±¨´í:" << std::endl;
			std::cout << m_ps->getCompileError() << std::endl;
		}
		m_material = ctx->createMaterial(m_vs, m_ps);
		m_material->compile();
		setDefaultMaterial(m_material);
		m_cb = ctx->createConstBuffer();
		m_cb->create(sizeof(TextScreenInformation), 5);
		screen(Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0, 1, 0));
		viewport(Vec2(0, 0), Vec2(800, 600));
		updataScreenInformation();
		m_arr = new VertexArray(m_defaultFactory, 0);
		m_dc = ctx->createDrawCall(PrimitiveType::Triangles, 0, 0);
	}
	TextPipeline::~TextPipeline()
	{
		m_vf->release();
	}
	void TextPipeline::screen(Vec3 pos, Vec3 xHalfVec, Vec3 yHalfVec)
	{
		m_si.g_screenPosition = pos;
		m_si.g_screenXHalfSize = xHalfVec;
		m_si.g_screenYHalfSize = yHalfVec;
	}
	void TextPipeline::viewport(Vec2 lt, Vec2 rb)
	{
		m_si.g_screenLTCoord = lt;
		m_si.g_screenRBCoord = rb;
	}
	void TextPipeline::beginDraw()
	{
	}
	void TextPipeline::drawText(float x, float y, float w, float h)
	{
		size_t begin = m_arr->getVertexCount();
		m_arr->addVertex(6);

		m_arr->setAttribute(begin + 0, "vectorCoord", Vec2(x, y));
		m_arr->setAttribute(begin + 0, "commandOffset", 0.0f);
		m_arr->setAttribute(begin + 0, "commandSize", 0.0f);

		m_arr->setAttribute(begin + 1, "vectorCoord", Vec2(x + w, y));
		m_arr->setAttribute(begin + 1, "commandOffset", 0.0f);
		m_arr->setAttribute(begin + 1, "commandSize", 0.0f);

		m_arr->setAttribute(begin + 2, "vectorCoord", Vec2(x, y + h));
		m_arr->setAttribute(begin + 2, "commandOffset", 0.0f);
		m_arr->setAttribute(begin + 2, "commandSize", 0.0f);

		m_arr->setAttribute(begin + 3, "vectorCoord", Vec2(x + w, y));
		m_arr->setAttribute(begin + 3, "commandOffset", 0.0f);
		m_arr->setAttribute(begin + 3, "commandSize", 0.0f);

		m_arr->setAttribute(begin + 4, "vectorCoord", Vec2(x + w, y + h));
		m_arr->setAttribute(begin + 4, "commandOffset", 0.0f);
		m_arr->setAttribute(begin + 4, "commandSize", 0.0f);

		m_arr->setAttribute(begin + 5, "vectorCoord", Vec2(x, y + h));
		m_arr->setAttribute(begin + 5, "commandOffset", 0.0f);
		m_arr->setAttribute(begin + 5, "commandSize", 0.0f);
	}
	void TextPipeline::endDraw()
	{
		begin();
		m_cb->bind();
		m_vb = m_ctx->createVertexBuffer(m_arr);
		m_vb->create(m_ctx);
		m_il = m_ctx->createInputLayout(m_vf);
		m_il->create(m_ctx,m_vb);
		m_dc->setCount(m_arr->getVertexCount());
		m_transform->bind();
		m_material->bind();
		m_vb->bind();
		m_il->bind();
		m_dc->bind();
		m_vb->release();
		m_il->release();
		m_arr->clear();
	}
	void TextPipeline::updataScreenInformation()
	{
		m_cb->setData(&m_si, sizeof(m_si));
	}
	VertexFactory *TextPipeline::createFactory()
	{
		m_vf = new VertexFactory();
		// m_vf->addAttribute(FCT::PipelineAttributeType::Position3f, "position");
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "vectorCoord", DataType::Vec2);
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandOffset", DataType::Float);
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandSize", DataType::Float);
		return m_vf;
	}
}