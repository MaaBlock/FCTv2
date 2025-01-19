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
	return !glfwWindowShouldClose(m_window);
}

void FCT::GLFW_Window::bind(Context* ctx)
{
    if (dynamic_cast<GL_Context*>(ctx)) {
		GL_Context* glCtx = dynamic_cast<GL_Context*>(ctx);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glCtx->getGLVersionMajor());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glCtx->getGLVersionMinor());
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwMakeContextCurrent(m_window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}
	else if (dynamic_cast<D3D11_Context*>(ctx)) {
		throw std::runtime_error("GLFW_Window ²»Ö§³Öbind D3D11_Context");
	}
}

void FCT::GLFW_Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}
