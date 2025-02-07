#pragma once
#include <unordered_map>
#include "Chunk.h"
#include "player.h"
#include "BlockMesh.h"
class World
{
public:
    Block *selectedBlock = nullptr;
    Vec3 selectedPos;
    Pipeline *pipeline;
    std::unordered_map<Vec2, BlockMesh *> chunkMeshes;
    Texture* m_texture;
    PhysicsSystem* m_phySys;
    physx::PxScene* m_scene;
    World(Pipeline *pl, PhysicsSystem* phySys, physx::PxScene* scene) : pipeline(pl) , m_phySys(phySys),
        m_scene(scene)
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
    void setBlock(Vec3 pos) {

        Vec2 chunkPos = getChunkPos(pos);
        //Vec3 localPos = worldToLocalPos(pos);
        Chunk& chunk = getOrCreateChunk(chunkPos);

        Block* block = new Block;
        block->setPos(pos);
        chunk.addBlock(pos, block);
    }
    void addBlock(Vec3 pos)
    {
        Vec2 chunkPos = getChunkPos(pos);
        //Vec3 localPos = worldToLocalPos(pos);
        Chunk &chunk = getOrCreateChunk(chunkPos);

        Block *block = new Block;
        block->setPos(pos);
        chunk.addBlock(pos, block);

        auto meshIt = chunkMeshes.find(chunkPos);
        if (meshIt != chunkMeshes.end()) {
            meshIt->second->addBlock(pos, chunk);
        }
        //updateChunkMesh(chunkPos);
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
        if (meshIt != chunkMeshes.end()) {
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
    void updata() {
        for (auto& pair : chunkMeshes) {
            pair.second->updata();
        }
    }
    void render(Pipeline *pipeline, const Vec3 &playerPosition)
    {
        
        Vec2 playerChunkPos = getChunkPos(playerPosition);
        /*for (int x = -1; x <= 1; ++x)
        {
            for (int z = -1; z <= 1; ++z)
            {
                Vec2 chunkPos = playerChunkPos + Vec2(x, z);
                auto chunkIt = chunks.find(chunkPos);
                if (chunkIt != chunks.end())
                {
                    chunkIt->second.render(pipeline);
                }
            }
        }
        */
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
    void addFace(Vec3 pos, BlockFace face, Vec4 color = Vec4(1, 1, 1, 1));
    void clearFace(Vec3 vec, BlockFace face);
    void selectBlock(const Vec3 &pos)
    {
        Block *block = getBlock(pos);
        if (block)
        {
            if (selectedBlock)
            {
                selectedBlock->box->color(Vec4(1, 1, 1, 1));
                selectedBlock->box->updata();
            }
            selectedBlock = block;
            selectedBlock->box->color(Vec4(1.2, 1.2, 1.2, 1));
            selectedBlock->box->updata();
            selectedPos = pos;
        }
    }

    void destroyBlock(const Vec3 &pos)
    {
        removeBlock(pos);
        if (selectedBlock && pos == selectedPos)
        {
            selectedBlock = nullptr;
        }
    }

    void placeBlock(const Vec3 &pos, const Vec3 &normal)
    {
        Vec3 placePos = pos + normal;
        if (!isBlockAt(placePos))
        {
            addBlock(placePos);
        }
    }

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

    Vec3 getNormal(const Vec3 &hitPos, const Vec3 &rayStart)
    {
        Vec3 diff = hitPos - rayStart;
        float absX = std::abs(diff.x);
        float absY = std::abs(diff.y);
        float absZ = std::abs(diff.z);

        if (absX > absY && absX > absZ)
        {
            return Vec3(diff.x > 0 ? -1 : 1, 0, 0);
        }
        else if (absY > absZ)
        {
            return Vec3(0, diff.y > 0 ? -1 : 1, 0);
        }
        else
        {
            return Vec3(0, 0, diff.z > 0 ? -1 : 1);
        }
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

/*
namespace std
{
    template <>
    struct hash<Vec2>
    {
        size_t operator()(const Vec2 &v) const
        {
            return ((v.x + v.y) * (v.x + v.y + 1) / 2) + v.y;
        }
    };
}
using namespace std;
class World
{
public:
    World(Player *player);
    void render()
    {
        Vec2 playerChunk = player->getChunk();
        for (int i = -2; i <= 2; i++)
        {
            for (int j = -2; j <= 2; j++)
            {
                Vec2 chunkPos = playerChunk + Vec2(i, j);
                getOrCreateChunk(chunkPos).render();
            }
        }
    }
    Chunk& getOrCreateChunk(const Vec2 &pos)
    {
        auto it = map.find(pos);
        if (it == map.end())
        {
            auto result = map.emplace(std::piecewise_construct,
                                      std::forward_as_tuple(pos),
                                      std::forward_as_tuple(pos));
            return result.first->second;
        }
        return it->second;
    }

private:
    Player *player;
    std::unordered_map<Vec2, Chunk> map;
};
*/