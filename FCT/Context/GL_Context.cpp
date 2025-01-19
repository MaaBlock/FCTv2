#include "../headers.h"
namespace FCT {
	GL_Context::GL_Context(GL_ContextShareData* shareData, Runtime* runtime)
	{
		g_shareData = shareData;
		g_runtime = runtime;
		g_glVersionMajor = g_shareData->getGLVersionMajor();
		g_glVersionMinor = g_shareData->getGLVersionMinor();
	}

	void GL_Context::create(IRenderTarget* target)
	{
		m_target = target;
		target->bind(this);
	}

	void GL_Context::clear(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

}