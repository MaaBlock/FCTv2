#pragma once
#include "../MutilThreadBase/RefCount.h"
namespace FCT {
	class Window : public RefCount {
	public:
		virtual bool isRunning() const = 0;
	private:

	};
}