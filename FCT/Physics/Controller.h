#pragma once
#include <PxPhysicsAPI.h>
#include "./PhysicsSystem.h"
namespace FCT {
	using Scene = physx::PxScene ;
	class Controller;
	class BoxController;
	class ControllerManager {
	private:
		Scene* m_scene;
		physx::PxControllerManager* m_manager;
	public:
		ControllerManager(Scene* scene);
		BoxController* createBoxController();
		physx::PxControllerManager* manager() const;
	};
	class Controller {
	private:

	protected:
		physx::PxController* m_controller;
	public:
		Controller() {

		}

	};
	class BoxController : public Controller {
	private:
		ControllerManager* m_manager;
		physx::PxBoxControllerDesc m_desc;
		float m_mass;
		bool m_created;
	public:
		BoxController(ControllerManager* manager);
		void stepOffset(float stepOffset);
		void material(physx::PxMaterial* material);
		void create();
		void destory();
		void pos() const;
		void pos(Vec3 pos);
		void upDirection(Vec3 upDirection);
		void density(float density);
		void mass(float mass);
		void halfHeight(float halfHeight);
		void halfForward(float halfForward);
		void halfSide(float halfSide);
	private:
		void calculateDensity();
	};

}