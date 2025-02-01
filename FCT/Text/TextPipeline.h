#pragma once
#include "../Context/Pipeline.h"
namespace FCT {
	class TextPipeline : public Pipeline {
	public:
		TextPipeline(Context* ctx);
		
	private:
		VertexFactory* createFactory();
		Context* m_ctx;
		std::vector<float> m_commandQueue;
		VertexFactory* m_vf;
	};
	/*
	ToDo:�����Ƿ��б�Ҫ����ͬһ���ֵ����
	�����������ظ�ʹ��
	*/
}