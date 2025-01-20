#pragma once
#include "./Window.h"
#include "./GLFW_WindowShareData.h"
namespace FCT {
	class GLFW_Window : public Window {
	public:
		GLFW_Window(GLFW_WindowShareData* shareData, Runtime* runtime);
		void invokeResizeCallbacks(int width, int height);
		void create(int x, int y, const char* title);
		bool isRunning() const override;
		void bind(Context* ctx) override;
		void swapBuffers() override;
		int getWidth() override;
		int getHeight() override;
	private:
		GLFW_WindowShareData* m_shareData;
		Runtime* m_runtime;
		GLFWwindow* m_window;

	};
}