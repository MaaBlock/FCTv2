#pragma once
#include "Block.h"
#include "../FCT/headers.h"
using namespace FCT;

namespace std
{
    template <>
    struct hash<Vec2>
    {
        size_t operator()(const Vec2& v) const
        {
            return ((v.x + v.y) * (v.x + v.y + 1) / 2) + v.y;
        }
    };
    template <>
    struct hash<FCT::Vec3>
    {
        size_t operator()(const FCT::Vec3& v) const
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
    Vec2 chunkPos;
    std::unordered_map<Vec3, Block*> blocks;

    Chunk(Vec2 pos) : chunkPos(pos) {
        Vec3 offset = Vec3(pos.x * 16, 0, pos.y * 16);
        for (int i = 0; i < 16; i++) {

			for (int j = 0; j < 16; j++) {
                for (int y = 0; y < 3; ++y)
                {
                    newBlock(offset + Vec3(i, y, j), 0);
                }
                for (int y = 3; y < 5; ++y)
                {
                    newBlock(offset + Vec3(i, y, j), 1);
                }
                for (int y = 5; y < 6; ++y)
                {
                    newBlock(offset + Vec3(i, y, j), 2);
                }
            }
        }
    }

    ~Chunk()
    {
        for (auto& pair : blocks)
        {
            delete pair.second;
        }
    }

    unsigned getBlockType(Vec3 pos) {
		return (isBlockAt(pos)) ? blocks[pos]->type : 0;
    }
    Vec3 worldPosToChunkPos(Vec3 worldPos) const
    {
        return Vec3(
            static_cast<int>(worldPos.x) % Chunk::CHUNK_SIZE,
            static_cast<int>(worldPos.y),
            static_cast<int>(worldPos.z) % Chunk::CHUNK_SIZE);
    }

    void addBlock(Vec3 pos, Block* block)
    {
        blocks[pos] = block;
    }
    void newBlock(Vec3 pos, unsigned id) {
        Block* block = new Block;
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

    Block* getBlock(Vec3 pos)
    {
        auto it = blocks.find(pos);
        return (it != blocks.end()) ? it->second : nullptr;
    }

    bool isBlockAt(Vec3 pos) const
    {
        return blocks.find(pos) != blocks.end();
    }

    void render(Pipeline* pipeline)
    {
        //for (const auto& pair : blocks)
        //{
            //pair.second->render(pipeline);
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