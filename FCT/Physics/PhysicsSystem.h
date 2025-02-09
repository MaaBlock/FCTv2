#pragma once

namespace FCT
{
    class Runtime;
    class FhsyShareData;

    class PhysicsSystem
    {
    public:
        PhysicsSystem(Runtime *rt, FhsyShareData *shareData);
        ~PhysicsSystem();

        physx::PxPhysics *getPhysics() { return m_physics; }
        physx::PxScene *createScene(const physx::PxSceneDesc &desc);
        physx::PxScene *getScene(size_t index) const;
        void removeScene(physx::PxScene *scene);

        physx::PxTriangleMesh *createTriangleMeshNoIndices(VertexArray *array);
        physx::PxTriangleMesh *createTriangleMeshNoIndices(
            const void *vertexData,
            uint32_t vertexCount,
            uint32_t vertexStride,
            uint32_t positionOffset);
        physx::PxMaterial *createMaterial(float staticFriction, float dynamicFriction, float restitution);
        physx::PxShape *createTriangleMeshShape(physx::PxTriangleMesh *mesh, const physx::PxMaterial *material, bool isExclusive = false);
        physx::PxRigidStatic *createStaticRigidBody(const Vec3 &position);
        physx::PxScene* createBasicScene(physx::PxSimulationFilterCallback* filter,const Vec3& gravity = Vec3(0.0f, -9.81f, 0.0f));
        physx::PxScene *createBasicScene(const Vec3 &gravity = Vec3(0.0f, -9.81f, 0.0f));

        physx::PxRigidDynamic *createDynamicRigidBody(const Vec3 &position, const Vec4 rotation);
        physx::PxRigidDynamic* createDynamicBox(const Vec3& position, const Vec3& halfExtents, physx::PxMaterial* material, float mass);

    private:
        Runtime *m_rt;
        FhsyShareData *m_shareData;

        physx::PxPhysics *m_physics = nullptr;
        std::vector<physx::PxScene *> m_scenes;
    };
}