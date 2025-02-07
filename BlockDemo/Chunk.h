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

/*
class Chunk
{
public:
	Chunk(Vec2 chunkPos) : m_chunkPos(chunkPos) {
		genrateChunk();
	}
	void genrateChunk() {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				block[i][0][j] = CreateBlock(BlockType::grass);
			}
		}
	}
	void render();
	static void Init(Pipeline* pl) {
		m_pl = pl;
	}
private:
    Vec2 m_chunkPos;
    static Pipeline* m_pl;
	Block* block[16][256][16] = {};
};

*/

struct Chunk
{
public:
    static const int CHUNK_SIZE = 16;
    Vec2 chunkPos;
    std::unordered_map<Vec3, Block*> blocks;

    Chunk(Vec2 pos) : chunkPos(pos) {}

    ~Chunk()
    {
        for (auto& pair : blocks)
        {
            delete pair.second;
        }
    }

    Vec3 worldPosToChunkPos(Vec3 worldPos) const
    {
        return Vec3(
            (static_cast<int>(worldPos.x) % CHUNK_SIZE) % CHUNK_SIZE,
            static_cast<int>(worldPos.y),
            (static_cast<int>(worldPos.z) % CHUNK_SIZE) % CHUNK_SIZE);
    }

    void addBlock(Vec3 localPos, Block* block)
    {
        blocks[localPos] = block;
    }

    void removeBlock(Vec3 localPos)
    {
        auto it = blocks.find(localPos);
        if (it != blocks.end())
        {
            delete it->second;
            blocks.erase(it);
        }
    }

    Block* getBlock(Vec3 localPos)
    {
        auto it = blocks.find(localPos);
        return (it != blocks.end()) ? it->second : nullptr;
    }

    bool isBlockAt(Vec3 localPos) const
    {
        return blocks.find(localPos) != blocks.end();
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