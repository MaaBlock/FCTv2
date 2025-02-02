#include "../headers.h"
namespace FCT {
	TextPipeline::TextPipeline(Context* ctx) : Pipeline(ctx,createFactory())
	{
		m_vs = ctx->createVertexShader(m_vf);
		m_vs->addCustomOutput(FCT::PipelineAttributeType::Position3f, "position");
		m_vs->compileFromSource(R"(
layout(binding = 5) uniform TextScreenInformation{
	Vec3 g_screenLTPosition;
	Vec3 g_screenRBPosition;
    Vec2 g_screenLTCoord;
    Vec2 g_screenRBCoord;
};
VertexOutput main(VertexInput input)
{
	VertexOutput output;
	output.position = 
	output.vectorCoord = input.vectorCoord;
	return output;
}
)");
	}
	TextPipeline::~TextPipeline()
	{
		m_vf->release();
	}
	VertexFactory* TextPipeline::createFactory()
	{
		m_vf = new VertexFactory();
		//m_vf->addAttribute(FCT::PipelineAttributeType::Position3f, "position");
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "vectorCoord", DataType::Vec2);
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandOffset", DataType::Float);
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandSize", DataType::Float);
		return m_vf;
	}
}