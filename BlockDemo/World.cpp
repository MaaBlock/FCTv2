#include "Chunk.h"
#include "Player.h"
#include "BlockMesh.h"
#include "World.h"
#include "Camera.h"

World::World(Pipeline* pl, PhysicsSystem* phySys, physx::PxScene* scene, Camera* camera)
    : pipeline(pl), m_phySys(phySys),
    m_scene(scene), m_camera(camera)
{
    /*
    for (int x = -64; x <= 64; ++x)
    {
        for (int z = -64; z <= 64; ++z)
        {
            for (int y = 0; y < 3; ++y)
            {
                setBlock(Vec3(x, y, z),0);
            }
            for (int y = 3; y < 5; ++y)
            {
                setBlock(Vec3(x, y, z), 1);
            }
            for (int y = 5; y < 6; ++y)
            {
                setBlock(Vec3(x, y, z), 2);
            }
        }
    }
    */
    generateChunkMeshes();
}

void World::player(Player* player)
{
    m_player = player;
}

void World::updata()
{
    for (auto& pair : chunkMeshes)
    {
        pair.second->updata();
    }
    Block::texture->bind();
    Vec2 playerChunkPos = getChunkPos(m_player->pos());
    for (int x = -1; x <= 1; ++x)
    {
        for (int z = -1; z <= 1; ++z)
        {
            Vec2 chunkPos = playerChunkPos + Vec2(x, z);
            getOrCreateChunk(chunkPos);
        }
    }
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

void World::addFace(Vec3 pos, BlockFace face,unsigned id, Vec4 color)
{
    Vec2 chunkPos = getChunkPos(pos);
    auto meshIt = chunkMeshes.find(chunkPos);
    if (meshIt != chunkMeshes.end()) {
        meshIt->second->addFace(pos, face,id, color);
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
