#include "FhsyShareData.h"
#include <stdexcept>

namespace FCT {
    void FhsyShareData::init() {
        mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocator, mErrorCallback);
        if (!mFoundation) throw std::runtime_error("PxCreateFoundation failed!");

        mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale());
        if (!mPhysics) throw std::runtime_error("PxCreatePhysics failed!");

        mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
    }

    void FhsyShareData::term() {
        for (auto& pair : mScenes) {
            if (pair.second) pair.second->release();
        }
        mScenes.clear();
        if (mDispatcher) mDispatcher->release();
        if (mPhysics) mPhysics->release();
        if (mFoundation) mFoundation->release();

        mDispatcher = nullptr;
        mPhysics = nullptr;
        mFoundation = nullptr;
    }

    physx::PxScene* FhsyShareData::createScene(const std::string& name, const physx::PxSceneDesc& desc) {
        if (mScenes.find(name) != mScenes.end()) {
            throw std::runtime_error("Scene with this name already exists");
        }
        physx::PxScene* scene = mPhysics->createScene(desc);
        if (!scene) throw std::runtime_error("Failed to create scene");
        mScenes[name] = scene;
        return scene;
    }

    physx::PxScene* FhsyShareData::getScene(const std::string& name) {
        auto it = mScenes.find(name);
        if (it == mScenes.end()) {
            return nullptr;
        }
        return it->second;
    }

    void FhsyShareData::removeScene(const std::string& name) {
        auto it = mScenes.find(name);
        if (it != mScenes.end()) {
            it->second->release();
            mScenes.erase(it);
        }
    }
}// namespace FCT
