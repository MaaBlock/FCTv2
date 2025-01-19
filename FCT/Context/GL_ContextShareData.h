#pragma once
#include "../Runtime/runtime.h"
namespace FCT {
	class Runtime;
	class GL_ContextShareData {
	public:
		GL_ContextShareData(Runtime* runtime);
		Context* createContext(IRenderTarget* target);
		void init();
		void setOpenGLVersion(int major, int minor);
		int getGLVersionMajor() const { return g_glVersionMajor; }
		int getGLVersionMinor() const { return g_glVersionMinor; }
	private:
		Runtime* g_runtime;
		int g_glVersionMajor;
		int g_glVersionMinor;
	};
}