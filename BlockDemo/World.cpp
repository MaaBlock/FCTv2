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
    }
}
