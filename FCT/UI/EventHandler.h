#pragma once
#include "../MutilThreadBase/RefCount.h"
#include "../UI/Window.h"
namespace FCT {
	class Window;
	class EventHandler : public RefCount {
	public:
		virtual void onResize(Window* wnd,int width,int height) = 0;
	private:

	};
}