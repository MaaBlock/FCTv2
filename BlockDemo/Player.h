#pragma once
#include "Camera.h"
constexpr float PlayerHeght = 1.68;
constexpr float PlayerHalfHeght = PlayerHeght / 2;
constexpr float CameraOffset = 1.6 - PlayerHeght / 2;
class Player
{
private:
    World *m_world;
    Camera *m_camera;
    PhysicsSystem *m_phySys;
    physx::PxScene *m_scene;
    physx::PxRigidDynamic *m_actor;
    BoxController* m_controller;
public:
    float speed = 2.5f;
    float gravity = 9.8;
    float jumpVelocity = 0.0;
    void checkGround(physx::PxScene* scene, float deltaTime);
    bool m_onGround;
    static constexpr float GROUND_TOLERANCE = 0.01f;
    Player(World *world, Camera *camera, PhysicsSystem *phySys, physx::PxScene *scene)
    {
        m_onGround = true;
        m_world = world;
        m_camera = camera;
        m_phySys = phySys;
        m_scene = scene;
        auto material = m_phySys->createMaterial(0.8, 0.5, 0);
        m_actor = m_phySys->createDynamicBox(m_camera->pos(), Vec3(0.4, PlayerHalfHeght, 0.1), material, 75);
        m_actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
        m_actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
        m_actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, true);
        scene->addActor(*m_actor);
        physx::PxFilterData filterData;
        filterData.word0 = 1;
    }
    void processMouseMovement(float xoffset, float yoffset)
    {
        m_camera->rotate(xoffset, yoffset);
    }
    auto getActor() {
        return m_actor;
    }
    void setOnGround(bool onGround) {
        m_onGround = onGround;
    }
    void processKeyboard(char *keyState, float deltaTime, const World &world)
    {
        checkGround(m_scene, deltaTime);
        Vec3 position = m_camera->pos();
        float velocity = speed * deltaTime;
        FCT::Vec3 frontFlat = m_camera->flatFrontY();
        FCT::Vec3 rightFlat = m_camera->flatRightY();

        physx::PxVec3 currentVelocity = m_actor->getLinearVelocity();

        FCT::Vec3 movement(0.0f, 0.0f, 0.0f);

        if (keyState['W'])
            movement += frontFlat;
        if (keyState['S'])
            movement -= frontFlat;
        if (keyState['A'])
            movement -= rightFlat;
        if (keyState['D'])
            movement += rightFlat;
        physx::PxTransform pose = m_actor->getGlobalPose();
        if (keyState['T']) {
			physx::PxTransform newPose = pose;
            newPose.p = Vec3(0, 5, 0);
            m_actor->setGlobalPose(newPose);
        }
        if (movement.x != 0.0f || movement.z != 0.0f)
        {
            movement = FCT::normalize(movement);
        }

        physx::PxVec3 newVelocity(movement.x * speed, currentVelocity.y, movement.z * speed);
        m_actor->setLinearVelocity(newVelocity);

        if (keyState[FCT::KC_SPACE] && m_onGround)
        {
            m_onGround = false;
            newVelocity.y = 5.0f;
            m_actor->setLinearVelocity(newVelocity);
        }
        Vec3 actorPosition(pose.p.x, pose.p.y + CameraOffset, pose.p.z);
        m_camera->pos(actorPosition);
    }
};
/*
class Player {
public:
    Player() {
        m_pos = Vec3(0, 0, 0);
    }
    Vec2 getChunk() {
        int chunkX = static_cast<int>(std::floor(m_pos.x / 16));
        int chunkZ = static_cast<int>(std::floor(m_pos.z / 16));
        return Vec2(chunkX, chunkZ);
    }
private:
    Vec3 m_pos;
};
*/
