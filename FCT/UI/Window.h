#pragma once
#include "../MutilThreadBase/RefCount.h"
#include "../Context/IRenderTarget.h"
namespace FCT {
	class Window : public RefCount,public IRenderTarget {
	public:
		virtual bool isRunning() const = 0;
		virtual void swapBuffers() = 0;
	private:

	};
}