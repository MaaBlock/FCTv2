#include "../FCT/headers.h"

int main() {
	FCT::Runtime* rt = FCT::CreateRuntime();
	rt->init();
	auto wnd = rt->createWindow(800, 600, "Hello,World!");
	auto ctx = rt->createContext(wnd);
	while (wnd->isRunning()) {
		ctx->clear(1.0,0,0);
		wnd->swapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	rt->tern();
	return 0;
}