#include "../FCT/headers.h"

int main() {
	FCT::Runtime* rt = FCT::CreateRuntime();
	rt->init();
	auto wnd = rt->createWindow(800, 600, "Hello,World!");
	while (wnd->isRunning()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	rt->tern();
	return 0;
}