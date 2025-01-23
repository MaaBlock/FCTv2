//#include <gl\GL.h>
//#include <gl\GLU.h>
#include "../FCT/headers.h"
#include <iostream>

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
	FCT::VertexFactory* factory = new FCT::VertexFactory;
	factory->addAttribute(FCT::PipelineAttributeType::Color4f);
	factory->addAttribute(FCT::PipelineAttributeType::Position2f);
	FCT::Pipeline pipeline(ctx, factory);
	while (wnd->isRunning()) {
		if (needViewPort) {
			float x = 0,y = 0, w = wnd->getWidth(),h = wnd->getHeight();
			if (h / w > 0.75) {
				x = 0;
				y = (h - w * 3 / 4) / 2;
				h = w * 3 / 4;
			}
			else {
				x = (w - h * 4 / 3) / 2;
				y = 0;
				w = h * 4 / 3;
			}
			ctx->viewport(x, y, w, h);
		}
		ctx->clear(0,0,0);
		glBegin(GL_QUADS);
		glColor3f(1.0, 0, 0);
		glVertex2d(-1, -1);
		glColor3f(1.0, 0, 0);
		glVertex2d(1, -1);
		glColor3f(1.0, 0, 0);
		glVertex2d(1, 1);
		glColor3f(1.0, 0, 0);
		glVertex2d(-1, 1);
		glEnd();
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