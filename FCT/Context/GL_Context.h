#pragma once
#include "./Context.h"
namespace FCT {
	class GL_Context : public Context {
	public:
		GL_Context(GL_ContextShareData* shareData, Runtime* runtime);
		void create(IRenderTarget* target);
		void clear(float r, float g, float b) override;
		int getGLVersionMajor() const { return g_shareData->getGLVersionMajor(); }
		int getGLVersionMinor() const { return g_shareData->getGLVersionMinor(); }
		void viewport(int x, int y, int width, int height) override;
	private:
		GL_ContextShareData* g_shareData;
		Runtime* g_runtime;
		int g_glVersionMajor;
		int g_glVersionMinor;
		IRenderTarget* m_target;

	};
}