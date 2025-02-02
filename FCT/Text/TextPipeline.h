#pragma once
#include "../Context/Pipeline.h"
namespace FCT {
	class TextPipeline : public Pipeline {
	public:
		TextPipeline(Context* ctx);
		~TextPipeline();
	private:
		VertexFactory* createFactory();
		Context* m_ctx;
		std::vector<float> m_commandQueue;
		VertexFactory* m_vf;
		VertexShader* m_vs;
	};
	/*
	ToDo:考虑是否有必要，把同一个字的命令，
	利用索引，重复使用
	*/
}