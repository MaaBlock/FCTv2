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

void FCT::GLFW_Window::invokeMouseMoveCallbacks(int xpos, int ypos)
{
    for (auto cb : m_handlers) {
        cb->onMouseMove(this, static_cast<int>(xpos), static_cast<int>(ypos));
    }
}

void FCT::GLFW_Window::invokeMouseCallbacks(int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(m_window, &xpos, &ypos);

    for (auto cb : m_handlers) {
        if (action == GLFW_PRESS) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                cb->onLButtonDown(this, static_cast<int>(xpos), static_cast<int>(ypos));
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                cb->onRButtonDown(this, static_cast<int>(xpos), static_cast<int>(ypos));
            }
        } else if (action == GLFW_RELEASE) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                cb->onLButtonUp(this, static_cast<int>(xpos), static_cast<int>(ypos));
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                cb->onRButtonUp(this, static_cast<int>(xpos), static_cast<int>(ypos));
            }
        }
    }
}

void FCT::GLFW_Window::invokeKeyCallbacks(int key, int scancode, int action, int mods)
{
	for (auto cb : m_handlers) {
        if (action == GLFW_PRESS) {
            cb->onKeyDown(this, key);
        } else if (action == GLFW_RELEASE) {
            cb->onKeyUp(this, key);
        }
    }
}

void FCT::GLFW_Window::invokeScrollCallbacks(int xoffset, int yoffset)
{
	for (auto cb : m_handlers) {
        cb->onMouseWheel(this, yoffset);
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
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
		FCT::GLFW_Window* wnd = reinterpret_cast<FCT::GLFW_Window*>(glfwGetWindowUserPointer(window));\
        wnd->invokeMouseCallbacks(button, action, mods);
        });
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
		FCT::GLFW_Window* wnd = reinterpret_cast<FCT::GLFW_Window*>(glfwGetWindowUserPointer(window));\
        wnd->invokeMouseMoveCallbacks(static_cast<int>(xpos), static_cast<int>(ypos));
        });
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        FCT::GLFW_Window* wnd = reinterpret_cast<FCT::GLFW_Window*>(glfwGetWindowUserPointer(window));
        wnd->invokeKeyCallbacks(key, scancode, action, mods);
    });
	 glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        FCT::GLFW_Window* wnd = reinterpret_cast<FCT::GLFW_Window*>(glfwGetWindowUserPointer(window));
        wnd->invokeScrollCallbacks(xoffset, yoffset);
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
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glCtx->getGLVersionMajor());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glCtx->getGLVersionMinor());
        //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwMakeContextCurrent(m_window);
        gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(0);
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

void FCT::GLFW_Window::setCursorPos(int x, int y)
{
    glfwSetCursorPos(m_window, x, y);
}
