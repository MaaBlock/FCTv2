#pragma once
#include "./Pipeline.h"
namespace FCT {
	class VertexRenderPipeline : public Pipeline {
	public:
		VertexRenderPipeline(Context* context, VertexFactory* defaultFactory);

	};
}