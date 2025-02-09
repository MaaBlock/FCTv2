#pragma once
#include <PxPhysicsAPI.h>
#include <functional>
#include <map>
#include <iostream>
namespace FCT {

    class SimulationCallback : public physx::PxSimulationEventCallback {
    public:
        using TriggerCallback = std::function<void(physx::PxTriggerPair* pairs, physx::PxU32 count)>;
        using ConstraintBreakCallback = std::function<void(physx::PxConstraintInfo* constraints, physx::PxU32 count)>;
        using WakeCallback = std::function<void(physx::PxActor** actors, physx::PxU32 count)>;
        using SleepCallback = std::function<void(physx::PxActor** actors, physx::PxU32 count)>;
        using ContactCallback = std::function<void(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)>;
        void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override {
            for (auto it = contactCallbacks.begin(); it != contactCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(pairHeader, pairs, nbPairs);
                }
            }

        }
        void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override {
            for (auto it = triggerCallbacks.begin(); it != triggerCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(pairs, count);
                }
            }
        }
        void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override {
            for (auto it = constraintBreakCallbacks.begin(); it != constraintBreakCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(constraints, count);
                }
            }
        }

        // 物体唤醒事件
        void onWake(physx::PxActor** actors, physx::PxU32 count) override {
            for (auto it = wakeCallbacks.begin(); it != wakeCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(actors, count);
                }
            }
        }

        // 物体睡眠事件
        void onSleep(physx::PxActor** actors, physx::PxU32 count) override {
            for (auto it = sleepCallbacks.begin(); it != sleepCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(actors, count);
                }
            }
        }

        // 添加碰撞回调函数
        int addContactCallback(ContactCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            contactCallbacks[id] = callback;
            return id;
        }

        // 移除碰撞回调函数
        void removeContactCallback(int id) {
            contactCallbacks.erase(id);
        }

        // 清除所有碰撞回调函数
        void clearContactCallbacks() {
            contactCallbacks.clear();
        }

        // 添加触发回调函数
        int addTriggerCallback(TriggerCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            triggerCallbacks[id] = callback;
            return id;
        }

        // 移除触发回调函数
        void removeTriggerCallback(int id) {
            triggerCallbacks.erase(id);
        }

        // 清除所有触发回调函数
        void clearTriggerCallbacks() {
            triggerCallbacks.clear();
        }

        // 添加约束断裂回调函数
        int addConstraintBreakCallback(ConstraintBreakCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            constraintBreakCallbacks[id] = callback;
            return id;
        }

        // 移除约束断裂回调函数
        void removeConstraintBreakCallback(int id) {
            constraintBreakCallbacks.erase(id);
        }

        // 清除所有约束断裂回调函数
        void clearConstraintBreakCallbacks() {
            constraintBreakCallbacks.clear();
        }

        // 添加物体唤醒回调函数
        int addWakeCallback(WakeCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            wakeCallbacks[id] = callback;
            return id;
        }

        // 移除物体唤醒回调函数
        void removeWakeCallback(int id) {
            wakeCallbacks.erase(id);
        }

        // 清除所有物体唤醒回调函数
        void clearWakeCallbacks() {
            wakeCallbacks.clear();
        }

        // 添加物体睡眠回调函数
        int addSleepCallback(SleepCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            sleepCallbacks[id] = callback;
            return id;
        }

        void removeSleepCallback(int id) {
            sleepCallbacks.erase(id);
        }

        void clearSleepCallbacks() {
            sleepCallbacks.clear();
        }

        void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
        {
        }
        void bindToScene(physx::PxScene* scene) {
            if (scene) {
                scene->setSimulationEventCallback(this);
            }
        }
    private:
        std::map<int, ContactCallback> contactCallbacks;
        std::map<int, TriggerCallback> triggerCallbacks;
        std::map<int, ConstraintBreakCallback> constraintBreakCallbacks;
        std::map<int, WakeCallback> wakeCallbacks;
        std::map<int, SleepCallback> sleepCallbacks;
    };
}