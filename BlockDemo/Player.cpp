#include "Player.h"

void Player::checkGround(physx::PxScene* scene, float deltaTime)

{
    physx::PxVec3 origin = m_actor->getGlobalPose().p;
    origin.y += 0.1f;
    physx::PxVec3 direction(0, -1, 0);
    physx::PxRaycastBuffer hit;
    physx::PxQueryFilterData filterData;
    filterData.flags = physx::PxQueryFlag::eSTATIC;
    //filterData.data.word0 = 1;
    if (scene->raycast(origin, direction, PlayerHalfHeght + 0.30f, hit, physx::PxHitFlag::eDEFAULT, filterData))
    {
        physx::PxVec3 normal = hit.block.normal;
        if (normal.y > 0.7f)
        {
            m_onGround = true;
        }
        return;
    }
    m_onGround = false;
    return;
}
