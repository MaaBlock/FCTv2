#pragma once
#include "./Window.h"
#include "./GLFW_WindowShareData.h"
namespace FCT {
	class GLFW_Window : public Window {
	public:
		GLFW_Window(GLFW_WindowShareData* shareData, Runtime* runtime);
		void create(int x, int y, const char* title);
		bool isRunning() const;
	private:
		GLFW_WindowShareData* m_shareData;
		Runtime* m_runtime;
		GLFWwindow* m_window;
	};
}