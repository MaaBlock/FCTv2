#include "../FCT/headers.h"

int main() {
	FCT::Runtime* rt = FCT::CreateRuntime();
	rt->init();
	rt->setOpenGLVesion(1, 1);
	auto wnd = rt->createWindow(800, 600, "Hello,World!");
	auto ctx = rt->createContext(wnd);
	while (wnd->isRunning()) {
		ctx->clear(1.0,0,0);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(-0.5, -0.5);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(0.5, -0.5);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2d(0.0, 0.5);
		glEnd();
		wnd->swapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	rt->tern();
	return 0;
}