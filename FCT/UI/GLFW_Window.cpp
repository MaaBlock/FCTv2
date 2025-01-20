#include "../headers.h"

FCT::GLFW_Window::GLFW_Window(GLFW_WindowShareData* shareData, Runtime* runtime)
{
	m_shareData = shareData;
	m_runtime = runtime;
}

void FCT::GLFW_Window::invokeResizeCallbacks(int width, int height)
{
	for (auto cb : m_handlers) {
		cb->onResize(this, width, height);
	}
}

void FCT::GLFW_Window::create(int x, int y, const char* title)
{
	m_window = glfwCreateWindow(x, y, title, nullptr, nullptr);
	glfwSetWindowUserPointer(m_window,this);
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		FCT::GLFW_Window* wnd = reinterpret_cast<FCT::GLFW_Window*>(glfwGetWindowUserPointer(window));\
		wnd->invokeResizeCallbacks(width, height);
		});
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
		throw std::runtime_error("GLFW_Window 不支持 bind D3D11_Context");
	}
	m_callbackHandler->addResizeCallback([this,ctx](Window* wnd, int w, int h) {
		ctx->viewport(0, 0, w, h);
		});
}

void FCT::GLFW_Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

int FCT::GLFW_Window::getWidth()
{
	int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return width;
}

int FCT::GLFW_Window::getHeight()
{
	int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return height;

}
