#pragma once
#include <GLFW/glfw3.h>
#include <boost/lockfree/queue.hpp>
#include <thread>
using UITaskFunction = std::function<void(void*)>;
struct GLFW_UiTask {
	void* param;
	UITaskFunction func;
};
class GLFW_WindowShareData {
public:
	GLFW_WindowShareData();
	std::thread& getUiThread();
	void init();
	void postUiTask(UITaskFunction func,void* param = nullptr,bool waited = true);
private:
	boost::lockfree::queue<GLFW_UiTask> g_taskQueue;
	std::thread g_uiThread;
	bool g_inited;
	bool g_runing;
};