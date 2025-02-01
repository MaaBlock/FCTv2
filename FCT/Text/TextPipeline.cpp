#include "../headers.h"
namespace FCT {
	TextPipeline::TextPipeline(Context* ctx) : Pipeline(ctx,createFactory())
	{
		
	}
	VertexFactory* TextPipeline::createFactory()
	{
		m_vf = new VertexFactory();
		m_vf->addAttribute(FCT::PipelineAttributeType::Position3f, "position");
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "vectorCoord", DataType::Vec2);
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandOffset", DataType::Float);
		m_vf->addAttribute(FCT::PipelineAttributeType::Custom, "commandSize", DataType::Float);
		return m_vf;
	}
}