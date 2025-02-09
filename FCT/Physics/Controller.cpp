#include "../headers.h"
namespace FCT {
	FCT::ControllerManager::ControllerManager(Scene* scene)
	{
		m_scene = scene;
		m_manager = PxCreateControllerManager(*scene);
	}

	FCT::BoxController* FCT::ControllerManager::createBoxController()
	{
		return new BoxController(this);
	}

	physx::PxControllerManager* ControllerManager::manager() const
	{
		return m_manager;
	}

	BoxController::BoxController(ControllerManager* manager)
	{
		m_manager = manager;
		m_created = false;
		m_mass = 0;
	}

	void BoxController::stepOffset(float stepOffset)
	{
		m_desc.stepOffset = stepOffset;
	}

	void BoxController::material(physx::PxMaterial* material)
	{
		m_desc.material = material;
	}

	void BoxController::create()
	{
		if (m_mass)
			calculateDensity();
		auto manager = m_manager->manager();
		m_controller = manager->createController(m_desc);
		m_created = true;
	}
	void BoxController::destory()
	{

	}
	void BoxController::pos() const
	{
		m_controller->getPosition();
	}

	void BoxController::pos(Vec3 pos)
	{
		if (m_created) {
			m_controller->setPosition(pos);
		}
		else {
			m_desc.position = pos;
		}
	}

	void BoxController::upDirection(Vec3 upDirection)
	{
		if (m_created) {
			m_controller->setUpDirection(upDirection);
		}
		else {
			m_desc.upDirection = upDirection;
		}
	}

	void BoxController::density(float density)
	{
		m_desc.density = density;
	}

	void BoxController::mass(float mass)
	{
		m_mass = mass;
	}

	void BoxController::halfHeight(float halfHeight)
	{
		m_desc.halfHeight = halfHeight;
	}

	void BoxController::halfForward(float halfForward)
	{
		m_desc.halfForwardExtent = halfForward;
	}

	void BoxController::halfSide(float halfSide)
	{
		m_desc.halfSideExtent = halfSide;
	}

	void BoxController::calculateDensity()
	{
		float length = m_desc.halfSideExtent * 2.0f;
		float width = m_desc.halfForwardExtent * 2.0f;
		float height = m_desc.halfHeight * 2.0f;
		float volume = length * width * height;
		m_desc.density = m_mass / volume;
	}



}