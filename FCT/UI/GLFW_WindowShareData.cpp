#include "../headers.h"

GLFW_WindowShareData::GLFW_WindowShareData()
{
	g_inited = false;
	g_runing = true;
}

std::thread& GLFW_WindowShareData::getUiThread()
{
	return g_uiThread;
}

void GLFW_WindowShareData::init()
{
	g_uiThread = std::thread([this]() {
		glfwInit();
		g_inited = true;
		while (g_runing)
		{
			while (!g_taskQueue.empty()) {
				GLFW_UiTask task;
				g_taskQueue.consume_one(&task);
				task.func(task.param);
			}
            glfwPollEvents();
		}
		glfwTerminate();
		});
}

void GLFW_WindowShareData::postUiTask(UITaskFunction func, void* param, bool waited)
{
	if (g_inited)
	{
		GLFW_UiTask task;
		task.func = func;
		task.param = param;
		g_taskQueue.push(task);
		glfwPostEmptyEvent();
	}
}
