#pragma once
#include "../UI/Window.h"
#include "../UI/GLFW_WindowShareData.h"
#include "../Context/GL_ContextShareData.h"
#include "../ImageLoader/ImageLoader.h"
namespace FCT {
	class GLFW_WindowShareData;
	class GL_ContextShareData;
	class Pipeline;
	class Runtime {
	public:
		void init();
		void tern();
		Window* createWindow(int w,int h,const char* title);
		Context* createContext(IRenderTarget* target);
		void setOpenGLVesion(int major, int minor);
		ImageLoader* createImageLoader();
		Pipeline* createVectorRenderPipeline(Context* ctx);
	private:
		GLFW_WindowShareData* g_glfwWindowShareData;
		GL_ContextShareData* g_glContextShareData;
	};
	Runtime* CreateRuntime();
}