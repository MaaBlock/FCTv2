#include "BlockMesh.h"
#include "World.h"
void BlockMesh::updata()
{
    if (m_needUpdataResource) {
        updateResource();
        m_isReady = true;
        m_needUpdataResource = false;
    }
}
void BlockMesh::generateMesh(const Chunk &chunk)
{
    {
        std::thread thread([this, &chunk]
                           { 
                            for (auto block : chunk.blocks) {
                                for (int i = BlockFace::BlockFace_Begin + 1; i < BlockFace::BlockFace_End; i++) {
                                    if (!m_world->isBlockAt(block.second->pos + getNomal(BlockFace(i)))) {
                                        addFace(block.second->pos, BlockFace(i));
                                    }
                                }
                            }
                            m_needUpdataResource = true;
                            return; 
                            });
        thread.detach();
    }
}

void BlockMesh::addBlock(const Vec3& position, const Chunk& chunk)
{
    if (m_isReady) {
        for (int i = BlockFace::BlockFace_Begin + 1; i < BlockFace::BlockFace_End; i++) {
            auto back = getBackFace(BlockFace(i));
            auto nearVec = position + getNomal(BlockFace(i));
            if (m_world->isBlockAt(nearVec)) {
                clearFace(nearVec, back);
            }
            else {
                addFace(position, BlockFace(i));
            }
        }
        updateResource();
    }
}
void BlockMesh::removeBlock(const Vec3& pos, const Chunk& chunk)
{
    if (m_isReady) {
        for (int i = BlockFace::BlockFace_Begin + 1; i < BlockFace::BlockFace_End; i++) {
            auto back = getBackFace(BlockFace(i));
            auto nearVec = pos + getNomal(BlockFace(i));
            if (m_blockVertices[pos].endVertex[i]) {
                clearFace(pos, BlockFace(i));
            }
            if (m_world->isBlockAt(nearVec)) {
                addFace(nearVec, back);
            }
        }
        updateResource();
    }
}
