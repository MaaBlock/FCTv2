#include "../headers.h"
namespace FCT {
	Runtime* CreateRuntime() {
		Runtime* ret = new Runtime;
		return ret;
	}
	void Runtime::init()
	{
		g_glfwWindowShareData = new GLFW_WindowShareData(this);
		g_glfwWindowShareData->init();
		g_glContextShareData = new GL_ContextShareData(this);
		g_glContextShareData->init();
		FreeImage_ImageLoader::Init();
	}
	void Runtime::tern()
	{
		FreeImage_ImageLoader::Tern();
		delete g_glfwWindowShareData;
		delete g_glContextShareData;
		delete this;

	}
	Window* Runtime::createWindow(int w, int h, const char* title)
	{
		return g_glfwWindowShareData->createWindow(w,h,title);
	}
	Context* Runtime::createContext(IRenderTarget* target)
	{
		return g_glContextShareData->createContext(target);
	}
	void Runtime::setOpenGLVesion(int major, int minor)
	{
		g_glContextShareData->setOpenGLVersion(major, minor);
	}
	ImageLoader* Runtime::createImageLoader()
	{
		return new FreeImage_ImageLoader();
	}
}
