#pragma once
#include <GLFW/glfw3.h>
#include <boost/lockfree/queue.hpp>
#include <thread>
#include "./Window.h"
#include "../Runtime/runtime.h"
namespace FCT {
	using UITaskFunction = std::function<void(void*)>;
	struct GLFW_UiTaskData {
		UITaskFunction func;
		void* param;
		std::shared_ptr<bool> waited;
	};
	struct GLFW_UiTask {
		void (*func)(GLFW_UiTaskData*);
		GLFW_UiTaskData* data;
	};
	class GLFW_WindowShareData {
	public:
		GLFW_WindowShareData(Runtime* runtime);
		std::thread* getUiThread();
		void init();
		void postUiTask(UITaskFunction func, void* param = nullptr, bool waited = true);
		Window* createWindow(int x, int y, const char* title);
	private:
		Runtime* g_runtime;
		boost::lockfree::queue<GLFW_UiTask> g_taskQueue;
		std::thread* g_uiThread;
		bool g_inited;
		bool g_runing;
	};
}