#pragma once
#include <unordered_map>
#include "Chunk.h"
#include "BlockMesh.h"
struct Camera;
class World
{
public:
    Block *selectedBlock = nullptr;
    Vec3 selectedPos;
    Pipeline *pipeline;
    std::unordered_map<Vec2, BlockMesh *> chunkMeshes;
    Texture *m_texture;
    PhysicsSystem *m_phySys;
    physx::PxScene *m_scene;
    Camera* m_camera;
    World(Pipeline* pl, PhysicsSystem* phySys, physx::PxScene* scene, Camera* camera);
    Vec3 getBlockPositionFromHit(const Vec3 &hitPosition, const Vec3& normal)
    {
        return Vec3(
            std::round(hitPosition.x - 0.1 * normal.x),
            std::round(hitPosition.y - 0.1 * normal.y),
            std::round(hitPosition.z - 0.1 * normal.z));
    }
    unsigned getBlockType(Vec3 pos) {
        Vec2 chunkPos = getChunkPos(pos);
        auto chunkIt = chunks.find(chunkPos);
        if (chunkIt != chunks.end())
        {
            return chunkIt->second.getBlockType(pos);
        }
		return 0;
	}
    Vec3 getAdjacentBlockPosition(const Vec3 &blockPos, const Vec3&normal)
    {
        return Vec3(
            std::round(blockPos.x + 0.9 * normal.x),
            std::round(blockPos.y + 0.9 * normal.y),
            std::round(blockPos.z + 0.9 * normal.z));
    }

    bool mousePickFace(Vec3 rayOrigin, Vec3 rayDir, Vec3 &outHitPosition, Vec3 &outHitNormal)
    {
        physx::PxRaycastBuffer hit; 

        physx::PxQueryFilterData filterData;
        filterData.flags = physx::PxQueryFlag::eSTATIC;

        if (m_scene->raycast(rayOrigin, rayDir, 6.0f, hit, physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL, filterData))
        {
            outHitPosition = hit.block.position;
            outHitNormal = hit.block.normal;

            return true; 
        }

        return false; 
    }
    void updateChunkMesh(const Vec2 &chunkPos)
    {
        auto chunkIt = chunks.find(chunkPos);
        if (chunkIt == chunks.end())
            return;

        auto meshIt = chunkMeshes.find(chunkPos);
        if (meshIt == chunkMeshes.end())
        {
            meshIt = chunkMeshes.insert({chunkPos, new BlockMesh(pipeline)}).first;
            meshIt->second->setWorld(this);
            meshIt->second->texture(Block::texture);
            meshIt->second->create();
        }

        meshIt->second->generateMesh(chunkIt->second);
    }
    ~World()
    {
        for (auto &pair : chunkMeshes)
        {
            delete pair.second;
        }
    }
    void setBlock(Vec3 pos,unsigned id)
    {

        Vec2 chunkPos = getChunkPos(pos);
        Chunk &chunk = getOrCreateChunk(chunkPos);
        Block *block = new Block;
        block->type = id;
        block->setPos(pos);
        chunk.addBlock(pos, block);
    }
    bool canPlaceBlock(Vec3 blockPosition)
    {
        physx::PxBoxGeometry boxGeom(0.49f, 0.49f, 0.49f);
        physx::PxTransform blockTransform(blockPosition);

        physx::PxOverlapBuffer hit;
        physx::PxQueryFilterData filterData;
        filterData.flags = physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eANY_HIT;

        bool hasOverlap = m_scene->overlap(boxGeom, blockTransform, hit, filterData);

        return !hasOverlap;
    }
    void addBlock(Vec3 pos)
    {
        if (canPlaceBlock(pos))
        {
            Vec2 chunkPos = getChunkPos(pos);
            Chunk &chunk = getOrCreateChunk(chunkPos);

            Block *block = new Block;
            block->setPos(pos);
            chunk.addBlock(pos, block);

            auto meshIt = chunkMeshes.find(chunkPos);
            if (meshIt != chunkMeshes.end())
            {
                meshIt->second->addBlock(pos, chunk);
            }
        }
    }
    void removeBlock(Vec3 pos)
    {
        Vec2 chunkPos = getChunkPos(pos);
        auto chunkIt = chunks.find(chunkPos);
        if (chunkIt != chunks.end())
        {
            chunkIt->second.removeBlock(pos);
        }
        auto meshIt = chunkMeshes.find(chunkPos);
        if (meshIt != chunkMeshes.end())
        {
            meshIt->second->removeBlock(pos, chunkIt->second);
        }
    }
    void render(const Vec3 &playerPosition)
    {
        Vec2 playerChunkPos = getChunkPos(playerPosition);
        for (int x = -1; x <= 1; ++x)
        {
            for (int z = -1; z <= 1; ++z)
            {
                Vec2 chunkPos = playerChunkPos + Vec2(x, z);
                auto meshIt = chunkMeshes.find(chunkPos);
                if (meshIt != chunkMeshes.end())
                {
                    pipeline->draw(meshIt->second);
                }
            }
        }
    }

    bool isBlockAt(const Vec3 &pos) const
    {
        Vec3 blockCenter(std::round(pos.x), std::round(pos.y), std::round(pos.z));

        if (std::abs(pos.x - blockCenter.x) <= 0.5f &&
            std::abs(pos.y - blockCenter.y) <= 0.5f &&
            std::abs(pos.z - blockCenter.z) <= 0.5f)
        {
            Vec2 chunkPos = getChunkPos(blockCenter);
            auto chunkIt = chunks.find(chunkPos);
            if (chunkIt != chunks.end())
            {
                return chunkIt->second.isBlockAt(blockCenter);
            }
        }
        return false;
    }
    Block *getBlock(Vec3 pos)
    {
        Vec2 chunkPos = getChunkPos(pos);
        auto chunkIt = chunks.find(chunkPos);
        if (chunkIt != chunks.end())
        {
            return chunkIt->second.getBlock(pos);
        }
        return nullptr;
    }
    void updata()
    {
        for (auto &pair : chunkMeshes)
        {
            pair.second->updata();
        }
    }
    void render(Pipeline *pipeline, const Vec3 &playerPosition)
    {
        Block::texture->bind();
        Vec2 playerChunkPos = getChunkPos(playerPosition);
        for (int x = -1; x <= 1; ++x)
        {
            for (int z = -1; z <= 1; ++z)
            {
                Vec2 chunkPos = playerChunkPos + Vec2(x, z);
                auto chunkIt = chunkMeshes.find(chunkPos);
                if (chunkIt != chunkMeshes.end())
                {
                    pipeline->draw(chunkIt->second);
                }
            }
        }
    }
    void selectBlock(bool& isLPress, bool& isRPress);
    void addFace(Vec3 pos, BlockFace face,unsigned id = 0, Vec4 color = Vec4(1, 1, 1, 1));
    void clearFace(Vec3 vec, BlockFace face);
    void lightenFace(const Vec3 &pos, BlockFace face, float light);
    void updataMeshRenderSource(const Vec3 &pos);
    void selectBlock(const Vec3& pos, Vec3 normal);
    void unselect();
    void destroyBlock(const Vec3& pos);

    void placeBlock(const Vec3& pos);

    std::pair<bool, Vec3> raycast(const Vec3 &start, const Vec3 &direction, float maxDistance)
    {
        Vec3 step = direction * 0.1f;
        Vec3 current = start;
        float distance = 0.0f;

        while (distance < maxDistance)
        {
            Vec3 checkPos = Vec3(std::round(current.x), std::round(current.y), std::round(current.z));
            if (isBlockAt(checkPos))
            {
                return {true, checkPos};
            }
            current += step;
            distance += 0.1f;
        }

        return {false, current};
    }

private:
    std::unordered_map<Vec2, Chunk> chunks;

    void generateChunkMeshes()
    {
        for (const auto &chunkPair : chunks)
        {
            updateChunkMesh(chunkPair.first);
        }
    }

    Vec2 getChunkPos(const Vec3 &worldPos) const
    {
        return Vec2(
            int(worldPos.x / Chunk::CHUNK_SIZE),
            int(worldPos.z / Chunk::CHUNK_SIZE));
    }

    Chunk &getOrCreateChunk(const Vec2 &chunkPos)
    {
        auto it = chunks.find(chunkPos);
        if (it == chunks.end())
        {
            auto result = chunks.emplace(std::piecewise_construct,
                                         std::forward_as_tuple(chunkPos),
                                         std::forward_as_tuple(chunkPos));
            return result.first->second;
        }
        return it->second;
    }
};