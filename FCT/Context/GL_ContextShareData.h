#pragma once
#include "../Runtime/runtime.h"
namespace FCT {
	class Runtime;
	class GL_ContextShareData {
	public:
		GL_ContextShareData(Runtime* runtime);
		Context* createContext(IRenderTarget* target);
		void init();
	private:
		Runtime* g_runtime;
	};
}