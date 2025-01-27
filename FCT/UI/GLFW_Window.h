#pragma once
#include "./Window.h"
#include "./GLFW_WindowShareData.h"
namespace FCT {
	class GLFW_Window : public Window {
	public:
		GLFW_Window(GLFW_WindowShareData* shareData, Runtime* runtime);
		void invokeResizeCallbacks(int width, int height);
		void invokeMouseMoveCallbacks(int xpos, int ypos);
		void invokeMouseCallbacks(int button, int action, int mods);
		void invokeKeyCallbacks(int key, int scancode, int action, int mods);
		void invokeScrollCallbacks(int xoffset, int yoffset);
		void create(int x, int y, const char* title);
		bool isRunning() const override;
		void bind(Context* ctx) override;
		void swapBuffers() override;
		int getWidth() override;
		int getHeight() override;
		void setCursorPos(int x, int y) override;
	private:
		GLFW_WindowShareData* m_shareData;
		Runtime* m_runtime;
		GLFWwindow* m_window;
	};
}