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
		g_freeTypeFontShareData = new FreeType_FontShareData;
		g_freeTypeFontShareData->init();
	}
	void Runtime::tern()
	{
		FreeImage_ImageLoader::Tern();
		g_freeTypeFontShareData->tern();
		delete g_glfwWindowShareData;
		delete g_glContextShareData;
		delete g_freeTypeFontShareData;
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
	Font* Runtime::createFont()
	{
		return g_freeTypeFontShareData->create();
	}
	ImageLoader* Runtime::createImageLoader()
	{
		return new FreeImage_ImageLoader();
	}
	Pipeline* Runtime::createVectorRenderPipeline(Context* ctx)
	{
		return nullptr;
	}
}
