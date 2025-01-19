#include "../headers.h"
namespace FCT {
	GL_ContextShareData::GL_ContextShareData(Runtime* runtime)
	{
		g_runtime = runtime;
	}
	Context* GL_ContextShareData::createContext(IRenderTarget* target)
	{
		GL_Context* ctx = new GL_Context(this,g_runtime);
		ctx->create(target);
		return ctx;
	}
	void GL_ContextShareData::init()
	{
		setOpenGLVersion(3, 3);
	}
	void GL_ContextShareData::setOpenGLVersion(int major, int minor)
	{
		g_glVersionMajor = major;
		g_glVersionMinor = minor;
	}
}