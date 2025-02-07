#include "../headers.h"

namespace FCT
{
    void FhsyShareData::init()
    {
        g_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, g_allocator, g_rrrorCallback);
        if (!g_foundation)
        {
            std::cerr << "PxCreateFoundation failed!" << std::endl;
            return;
        }

        g_dispatcher = physx::PxDefaultCpuDispatcherCreate(2); 
        if (!g_dispatcher)
        {
            std::cerr << "PxDefaultCpuDispatcherCreate failed!" << std::endl;
            return;
        }

        std::cout << "FhsyShareData initialized successfully." << std::endl;
    }

    void FhsyShareData::term()
    {
        // 释放 CPU Dispatcher
        if (g_dispatcher)
        {
            g_dispatcher->release();
            g_dispatcher = nullptr;
        }

        // 释放 Foundation
        if (g_foundation)
        {
            g_foundation->release();
            g_foundation = nullptr;
        }

        std::cout << "FhsyShareData terminated successfully." << std::endl;
    }
} // namespace FCT
