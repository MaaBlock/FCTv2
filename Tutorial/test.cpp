#include "../FCT/headers.h"

int main() {
	FCT::Runtime* rt = FCT::CreateRuntime();
	rt->Init();

	rt->Tern();
	return 0;
}