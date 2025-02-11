#pragma once
#include "Block.h"
#include "../FCT/headers.h"
#include <noise/noise.h>
using namespace FCT;
using namespace noise;
#pragma comment(lib,"libnoise.lib")

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
    template <>
    struct hash<FCT::Vec3>
    {
        size_t operator()(const FCT::Vec3 &v) const
        {
            size_t h1 = std::hash<float>{}(v.x);
            size_t h2 = std::hash<float>{}(v.y);
            size_t h3 = std::hash<float>{}(v.z);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

struct Chunk
{
public:
    static const int CHUNK_SIZE = 16;
    static module::Perlin baseNoise;
    static module::Billow detailNoise;
    static module::Add combinedNoise;
    bool m_isInit;
    static void Init()
    {
        Chunk::baseNoise.SetOctaveCount(6);
        Chunk::baseNoise.SetFrequency(0.01);
        Chunk::baseNoise.SetPersistence(0.5);

        Chunk::detailNoise.SetOctaveCount(8);
        Chunk::detailNoise.SetFrequency(0.03);
        Chunk::detailNoise.SetPersistence(0.5);

        Chunk::combinedNoise.SetSourceModule(0, Chunk::baseNoise);
        Chunk::combinedNoise.SetSourceModule(1, Chunk::detailNoise);
    }
    Vec2 chunkPos;
    Vec2 m_pos;
    bool m_isReady;
    std::unordered_map<Vec3, Block *> blocks;
    bool isReady() {
        return m_isReady;
    }
    void generate() {
        Vec3 offset = Vec3(m_pos.x * CHUNK_SIZE, 0, m_pos.y * CHUNK_SIZE);
        for (int i = 0; i < CHUNK_SIZE; i++)
        {
            for (int j = 0; j < CHUNK_SIZE; j++)
            {
                double worldX = offset.x + i;
                double worldZ = offset.z + j;

                double value = combinedNoise.GetValue(worldX * 0.01, worldZ * 0.01, 0);
                int height = static_cast<int>((value + 1) * 32) + 64;

                for (int y = 0; y < height; ++y)
                {
                    unsigned blockType;
                    if (y == height - 1)
                    {
                        blockType = 2;
                    }
                    else if (y > height - 4)
                    {
                        blockType = 1;
                    }
                    else
                    {
                        blockType = 0;
                    }
                    newBlock(offset + Vec3(i, y, j), blockType);
                }
            }
        }
        m_isReady = true;
    }
    Chunk(Vec2 pos) : chunkPos(pos), m_pos(pos)
    {
        m_isReady = false;
        m_isInit = false;
    }
    ~Chunk()
    {
        for (auto &pair : blocks)
        {
            delete pair.second;
        }
    }
    bool isInit() const {
        return m_isInit;
    }
    void isInit(bool inited) {
        m_isInit = inited;
    }
    unsigned getBlockType(Vec3 pos)
    {
        return (isBlockAt(pos)) ? blocks[pos]->type : 0;
    }
    Vec3 worldPosToChunkPos(Vec3 worldPos) const
    {
        return Vec3(
            static_cast<int>(worldPos.x) % Chunk::CHUNK_SIZE,
            static_cast<int>(worldPos.y),
            static_cast<int>(worldPos.z) % Chunk::CHUNK_SIZE);
    }

    void addBlock(Vec3 pos, Block *block)
    {
        blocks[pos] = block;
    }
    void newBlock(Vec3 pos, unsigned id)
    {
        Block *block = new Block;
        block->type = id;
        block->setPos(pos);
        addBlock(pos, block);
    }

    void removeBlock(Vec3 pos)
    {
        auto it = blocks.find(pos);
        if (it != blocks.end())
        {
            delete it->second;
            blocks.erase(it);
        }
    }

    Block *getBlock(Vec3 pos)
    {
        auto it = blocks.find(pos);
        return (it != blocks.end()) ? it->second : nullptr;
    }

    bool isBlockAt(Vec3 pos) const
    {
        return blocks.find(pos) != blocks.end();
    }

    void render(Pipeline *pipeline)
    {
        // for (const auto& pair : blocks)
        //{
        // pair.second->render(pipeline);
        //}
    }

    Vec3 getWorldPos(Vec3 localPos) const
    {
        return Vec3(
            chunkPos.x * CHUNK_SIZE + localPos.x,
            localPos.y,
            chunkPos.y * CHUNK_SIZE + localPos.z);
    }
};