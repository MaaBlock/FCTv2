#pragma once
namespace FCT {
	class Runtime {
	public:
		void Init();
		void Tern();
	private:
		GLFW_WindowShareData* g_windowShareData;

	};
	Runtime* CreateRuntime();
}