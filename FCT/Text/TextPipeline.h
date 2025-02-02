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
	ToDo:�����Ƿ��б�Ҫ����ͬһ���ֵ����
	�����������ظ�ʹ��
	*/
}