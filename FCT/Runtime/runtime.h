#pragma once
#include "../UI/Window.h"
#include "../UI/GLFW_WindowShareData.h"
namespace FCT {
	class GLFW_WindowShareData;
	class Runtime {
	public:
		void init();
		void tern();
		Window* createWindow(int w,int h,const char* title);
	private:
		GLFW_WindowShareData* g_windowShareData;

	};
	Runtime* CreateRuntime();
}