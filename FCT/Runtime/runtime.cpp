#include "../headers.h"
namespace FCT {
	void Init()
	{
	}

	void Tern()
	{
	}
	Runtime* CreateRuntime() {
		Runtime* ret = new Runtime;
		return ret;
	}
	void Runtime::Init()
	{

	}
	void Runtime::Tern()
	{
		delete this;
	}
}
