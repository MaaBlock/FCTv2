#pragma once
#include "./Context.h"
namespace FCT {
	class GL_Context : public Context {
	public:
		GL_Context(GL_ContextShareData* shareData, Runtime* runtime);
		void create(IRenderTarget* target);
		void clear(float r, float g, float b) override;
	private:
		GL_ContextShareData* g_shareData;
		Runtime* g_runtime;
		IRenderTarget* m_target;

	};
}