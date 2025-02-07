#pragma once
#include <physx/PxPhysicsAPI.h>
#include <unordered_map>
#include <string>

namespace FCT {

class FhsyShareData {
public:

    void init();
    void term();

    physx::PxFoundation* getFoundation() { return mFoundation; }
    physx::PxPhysics* getPhysics() { return mPhysics; }
    physx::PxDefaultCpuDispatcher* getDispatcher() { return mDispatcher; }

    // 创建新的场景
    physx::PxScene* createScene(const std::string& name, const physx::PxSceneDesc& desc);
    // 获取已存在的场景
    physx::PxScene* getScene(const std::string& name);
    // 删除场景
    void removeScene(const std::string& name);
private:

    physx::PxDefaultAllocator mAllocator;
    physx::PxDefaultErrorCallback mErrorCallback;
    physx::PxFoundation* mFoundation = nullptr;
    physx::PxPhysics* mPhysics = nullptr;
    physx::PxDefaultCpuDispatcher* mDispatcher = nullptr;
    std::unordered_map<std::string, physx::PxScene*> mScenes;
};

} // namespace FCT
