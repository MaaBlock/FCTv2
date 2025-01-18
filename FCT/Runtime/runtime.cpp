#include "../headers.h"
namespace FCT {
	Runtime* CreateRuntime() {
		Runtime* ret = new Runtime;
		return ret;
	}
	void Runtime::Init()
	{
		g_windowShareData = new GLFW_WindowShareData();
		g_windowShareData->init();
	}
	void Runtime::Tern()
	{
		delete this;
	}
}
