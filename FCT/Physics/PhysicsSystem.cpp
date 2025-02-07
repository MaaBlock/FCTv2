#include "../headers.h"
#include "PhysicsSystem.h"
namespace FCT
{

	PhysicsSystem::PhysicsSystem(Runtime *rt, FhsyShareData *shareData)
		: m_rt(rt), m_shareData(shareData), m_physics(nullptr)
	{
		if (!m_shareData)
		{
			std::cerr << "FhsyShareData is null!" << std::endl;
			return;
		}

		// 创建 PxPhysics 实例
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_shareData->getFoundation(),
									physx::PxTolerancesScale(), true, nullptr);
		if (!m_physics)
		{
			std::cerr << "PxCreatePhysics failed!" << std::endl;
			return;
		}

		std::cout << "PhysicsSystem initialized successfully." << std::endl;
	}

	PhysicsSystem::~PhysicsSystem()
	{

		// 释放所有场景
		for (auto scene : m_scenes)
		{
			if (scene)
			{
				scene->release();
			}
		}
		m_scenes.clear();

		// 释放 PxPhysics
		if (m_physics)
		{
			m_physics->release();
			m_physics = nullptr;
		}

		std::cout << "PhysicsSystem terminated successfully." << std::endl;
	}

	physx::PxScene *PhysicsSystem::createScene(const physx::PxSceneDesc &desc)
	{
		if (!m_physics)
		{
			std::cerr << "Physics not initialized!" << std::endl;
			return nullptr;
		}

		physx::PxScene *scene = m_physics->createScene(desc);
		if (scene)
		{
			m_scenes.push_back(scene);
		}
		return scene;
	}

	physx::PxScene *PhysicsSystem::getScene(size_t index) const
	{
		if (index < m_scenes.size())
		{
			return m_scenes[index];
		}
		return nullptr;
	}

	void PhysicsSystem::removeScene(physx::PxScene *scene)
	{
		auto it = std::find(m_scenes.begin(), m_scenes.end(), scene);
		if (it != m_scenes.end())
		{
			(*it)->release();
			m_scenes.erase(it);
		}
	}
	physx::PxTriangleMesh *PhysicsSystem::createTriangleMeshNoIndices(
		const void *vertexData,
		uint32_t vertexCount,
		uint32_t vertexStride,
		uint32_t positionOffset)
	{
		if (!m_physics)
		{
			std::cerr << "Physics not initialized!" << std::endl;
			return nullptr;
		}

		physx::PxTriangleMeshDesc meshDesc = {};
		meshDesc.points.count = vertexCount;
		meshDesc.points.stride = vertexStride;
		meshDesc.points.data = static_cast<const uint8_t *>(vertexData) + positionOffset;
		meshDesc.triangles.count = vertexCount / 3;

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum result;
		bool status = PxCookTriangleMesh(m_physics->getTolerancesScale(), meshDesc, writeBuffer, &result);

		physx::PxTriangleMesh *triangleMesh = nullptr;
		if (status)
		{
			physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			triangleMesh = m_physics->createTriangleMesh(readBuffer);
		}

		if (!triangleMesh)
		{
			std::cerr << "Failed to create triangle mesh!" << std::endl;
		}

		return triangleMesh;
	}

	physx::PxMaterial *PhysicsSystem::createMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		return m_physics->createMaterial(staticFriction, dynamicFriction, restitution);
	}
	physx::PxShape *PhysicsSystem::createTriangleMeshShape(physx::PxTriangleMesh *mesh, const physx::PxMaterial *material, bool isExclusive)
	{
		physx::PxTriangleMeshGeometry geom(mesh);
		physx::PxShape *shape = m_physics->createShape(geom, *material, isExclusive);
		return shape;
	}
	physx::PxTriangleMesh *PhysicsSystem::createTriangleMeshNoIndices(VertexArray *array)
	{
		return createTriangleMeshNoIndices(array->data(), array->getVertexCount(), array->getFactory()->getStride(), array->getAttributeOffsetBytype(PipelineAttributeType::Position3f));
	}
	physx::PxRigidStatic *PhysicsSystem::createStaticRigidBody(const Vec3 &position)
	{
		physx::PxVec3 pxPosition(position.x, position.y, position.z);
		physx::PxRigidStatic *staticActor = m_physics->createRigidStatic(physx::PxTransform(pxPosition));
		return staticActor;
	}
	physx::PxScene *PhysicsSystem::createBasicScene(const Vec3 &gravity)
	{
		physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

		physx::PxScene *scene = createScene(sceneDesc);
		m_scenes.push_back(scene);

		return scene;
	}
} // namespace FCT