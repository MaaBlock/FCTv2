#include "../headers.h"
namespace FCT {
	Runtime* CreateRuntime() {
		Runtime* ret = new Runtime;
		return ret;
	}
	void Runtime::init()
	{
		g_windowShareData = new GLFW_WindowShareData(this);
		g_windowShareData->init();
	}
	void Runtime::tern()
	{
		delete this;
	}
	Window* Runtime::createWindow(int w, int h, const char* title)
	{
		return g_windowShareData->createWindow(w,h,title);
	}
}
