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

        // ���廽���¼�
        void onWake(physx::PxActor** actors, physx::PxU32 count) override {
            for (auto it = wakeCallbacks.begin(); it != wakeCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(actors, count);
                }
            }
        }

        // ����˯���¼�
        void onSleep(physx::PxActor** actors, physx::PxU32 count) override {
            for (auto it = sleepCallbacks.begin(); it != sleepCallbacks.end(); ++it) {
                if (it->second) {
                    it->second(actors, count);
                }
            }
        }

        // �����ײ�ص�����
        int addContactCallback(ContactCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            contactCallbacks[id] = callback;
            return id;
        }

        // �Ƴ���ײ�ص�����
        void removeContactCallback(int id) {
            contactCallbacks.erase(id);
        }

        // ���������ײ�ص�����
        void clearContactCallbacks() {
            contactCallbacks.clear();
        }

        // ��Ӵ����ص�����
        int addTriggerCallback(TriggerCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            triggerCallbacks[id] = callback;
            return id;
        }

        // �Ƴ������ص�����
        void removeTriggerCallback(int id) {
            triggerCallbacks.erase(id);
        }

        // ������д����ص�����
        void clearTriggerCallbacks() {
            triggerCallbacks.clear();
        }

        // ���Լ�����ѻص�����
        int addConstraintBreakCallback(ConstraintBreakCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            constraintBreakCallbacks[id] = callback;
            return id;
        }

        // �Ƴ�Լ�����ѻص�����
        void removeConstraintBreakCallback(int id) {
            constraintBreakCallbacks.erase(id);
        }

        // �������Լ�����ѻص�����
        void clearConstraintBreakCallbacks() {
            constraintBreakCallbacks.clear();
        }

        // ������廽�ѻص�����
        int addWakeCallback(WakeCallback callback) {
            static int nextId = 1;
            int id = nextId++;
            wakeCallbacks[id] = callback;
            return id;
        }

        // �Ƴ����廽�ѻص�����
        void removeWakeCallback(int id) {
            wakeCallbacks.erase(id);
        }

        // ����������廽�ѻص�����
        void clearWakeCallbacks() {
            wakeCallbacks.clear();
        }

        // �������˯�߻ص�����
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