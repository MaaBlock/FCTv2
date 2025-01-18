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
				g_taskQueue.pop(task);
				task.func(task.data);
			}
            glfwPollEvents();
		}
		glfwTerminate();
		});
}

void GLFW_WindowShareData::postUiTask(UITaskFunction func, void* param, bool wait)
{
	if (g_inited)
	{
		std::shared_ptr<bool> waited = std::make_shared<bool>(wait);
		GLFW_UiTask task;
		task.data = new GLFW_UiTaskData;
		task.data->param = param;
		task.data->func = func;
		task.data->waited = waited;
		task.func = [](GLFW_UiTaskData* data) {
			data->func(data->param);
			*data->waited = false;
			delete data;
			};
		g_taskQueue.push(task);
		glfwPostEmptyEvent();
		while (waited) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void GLFW_WindowShareData::createWindow(int x, int y, const char* title)
{

}
