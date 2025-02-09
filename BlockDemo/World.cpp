#include "Chunk.h"
#include "player.h"
#include "BlockMesh.h"
#include "World.h"
#include "Camera.h"
/*
World::World(Player *player)
{
    this->player = player;
}

*/

World::World(Pipeline* pl, PhysicsSystem* phySys, physx::PxScene* scene, Camera* camera)
    : pipeline(pl), m_phySys(phySys),
    m_scene(scene), m_camera(camera)
{
    for (int x = -64; x <= 64; ++x)
    {
        for (int z = -64; z <= 64; ++z)
        {
            for (int y = 0; y < 1; ++y)
            {
                setBlock(Vec3(x, y, z));
            }
        }
    }
    generateChunkMeshes();
}

void World::selectBlock(bool& isLPress, bool& isRPress)
{
    Vec3 rayDirection = m_camera->getRayDirection();
    Vec3 normal;
    Vec3 hitPos;
    auto hit = mousePickFace(m_camera->position, rayDirection, hitPos, normal);
    auto hitBlockPos = getBlockPositionFromHit(hitPos, normal);
    if (hit)
    {
        selectBlock(hitBlockPos, normal);
        if (isLPress)
        {
            destroyBlock(hitBlockPos);
            isLPress = false;
        }
        if (isRPress)
        {
            placeBlock(getAdjacentBlockPosition(hitPos, normal));
            isRPress = false;
        }
    }
    else {
        unselect();
    }
}
void World::addFace(Vec3 pos, BlockFace face, Vec4 color)
{
    Vec2 chunkPos = getChunkPos(pos);
    auto meshIt = chunkMeshes.find(chunkPos);
    if (meshIt != chunkMeshes.end()) {
        meshIt->second->addFace(pos, face, color);
        meshIt->second->updataResource();
    }
}

void World::clearFace(Vec3 vec, BlockFace face)
{
    Vec2 chunkPos = getChunkPos(vec);
    auto meshIt = chunkMeshes.find(chunkPos);
    if (meshIt != chunkMeshes.end()) {
        meshIt->second->clearFace(vec, face);
        meshIt->second->updataResource();
    }
}

void World::lightenFace(const Vec3& pos, BlockFace face, float light)
{
	Vec2 chunkPos = getChunkPos(pos);
	auto meshIt = chunkMeshes.find(chunkPos);
	if (meshIt != chunkMeshes.end()) {
		meshIt->second->lightenFace(pos, face, light);
	}
}

void World::updataMeshRenderSource(const Vec3& pos)
{
	Vec2 chunkPos = getChunkPos(pos);
	auto meshIt = chunkMeshes.find(chunkPos);
	if (meshIt != chunkMeshes.end()) {
		meshIt->second->updataRenderResource();
	}
}

void World::selectBlock(const Vec3& pos, Vec3 normal)
{
    Block* block = getBlock(pos);
    if (block)
    {
        unselect();
        selectedBlock = block;
        selectedPos = pos;
        lightenFace(selectedPos, getFaceFromNormal(normal), 1.2f);
        updataMeshRenderSource(selectedPos);
    }
}

void World::unselect()

{
    if (selectedBlock)
    {
        for (int i = 1; i <= 6; i++)
        {
            lightenFace(selectedPos, BlockFace(i), 1.0f);
        }
        updataMeshRenderSource(selectedPos);
    }
    selectedBlock = nullptr;
}
void World::destroyBlock(const Vec3& pos)
{
    removeBlock(pos);
    if (selectedBlock && pos == selectedPos)
    {
        selectedBlock = nullptr;
    }
}

void World::placeBlock(const Vec3& pos)
{
    Vec3 placePos = pos;
    if (!isBlockAt(placePos))
    {
        addBlock(placePos);
    }
}
