//#include <gl\GL.h>
//#include <gl\GLU.h>
#include "../FCT/headers.h"

int main() {
	FCT::Runtime* rt = FCT::CreateRuntime();
	rt->init();
	rt->setOpenGLVesion(1, 1);
	auto wnd = rt->createWindow(1600, 1200, "Hello,World!");
	auto ctx = rt->createContext(wnd);
	bool needViewPort = false;
	wnd->getCallBack()->addResizeCallback([&needViewPort](FCT::Window* wnd, int w, int h) {
		needViewPort = true;
		});
	while (wnd->isRunning()) {
		if (needViewPort) {
			ctx->viewport(0, 0, wnd->getWidth(), wnd->getHeight());
		}
		ctx->clear(1.0,0,0);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0, 0);
		glVertex2d(-0.5, -0.5);
		glColor3f(0, 0, 1.0);
		glVertex2d(0.5, -0.5);
		glColor3f(0, 1.0, 0);
		glVertex2d(0.0, 0.5);
		glEnd();
		wnd->swapBuffers();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	rt->tern();
	return 0;
}