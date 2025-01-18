#include "../headers.h"

FCT::GLFW_Window::GLFW_Window(GLFW_WindowShareData* shareData, Runtime* runtime)
{
	m_shareData = shareData;
	m_runtime = runtime;
}

void FCT::GLFW_Window::create(int x, int y, const char* title)
{
	m_window = glfwCreateWindow(x, y, title, nullptr, nullptr);
}

bool FCT::GLFW_Window::isRunning() const
{
	return glfwWindowShouldClose(m_window);
}
