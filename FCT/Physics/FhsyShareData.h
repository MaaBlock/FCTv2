#pragma once
#include <PxPhysicsAPI.h>
#include <unordered_map>
#include <string>

namespace FCT
{
    class FhsyShareData
    {
    public:
        void init();
        void term();

        physx::PxFoundation* getFoundation() { return g_foundation; }
        physx::PxDefaultCpuDispatcher* getDispatcher() { return g_dispatcher; }

    private:
        physx::PxDefaultAllocator g_allocator;
        physx::PxDefaultErrorCallback g_rrrorCallback;
        physx::PxFoundation* g_foundation = nullptr;
        physx::PxDefaultCpuDispatcher* g_dispatcher = nullptr;
    };
} // namespace FCT
