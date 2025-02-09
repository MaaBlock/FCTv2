#include "Chunk.h"
#include "player.h"
#include "BlockMesh.h"
#include "World.h"
/*
World::World(Player *player)
{
    this->player = player;
}

*/

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
