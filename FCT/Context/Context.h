#pragma once
#include "../MutilThreadBase/RefCount.h" 
namespace FCT {
	class Context : public RefCount {
	public:
		virtual ~Context() {};
		virtual void clear(float r, float g, float b) = 0;
		virtual void viewport(int x, int y, int width, int height) = 0;
	private:

	};
}