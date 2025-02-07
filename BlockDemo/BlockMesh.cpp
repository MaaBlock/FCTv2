#include "BlockMesh.h"
#include "World.h"
void BlockMesh::addFace(Vec3 pos, BlockFace face, Vec4 color)

{
    Vec3 halfNormal = getHalfNomal(face);
    Vec3 halfTangent = getHalfTangent(face);
    Vec3 halfBitangent = getHalfBitangent(face);
    Vec3 vertices[4];
    vertices[0] = pos + halfNormal + halfTangent + halfBitangent;
    vertices[1] = pos + halfNormal + halfTangent - halfBitangent;
    vertices[2] = pos + halfNormal - halfTangent - halfBitangent;
    vertices[3] = pos + halfNormal - halfTangent + halfBitangent;
    int startVertex;
    if (m_memroy.empty()) {
        startVertex = m_vertexArray->getVertexCount();
        m_vertexArray->addVertex(6);
    }
    else {
        auto front = m_memroy.front();
        startVertex = front.beginVertex;
        m_memroy.pop();
    }

    m_vertexArray->setAttribute(startVertex, m_positionOffset, vertices[0]);
    m_vertexArray->setAttribute(startVertex, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex, m_texcoordOffset, Vec2(1, 1));

    m_vertexArray->setAttribute(startVertex + 1, m_positionOffset, vertices[1]);
    m_vertexArray->setAttribute(startVertex + 1, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 1, m_texcoordOffset, Vec2(1, 0));

    m_vertexArray->setAttribute(startVertex + 2, m_positionOffset, vertices[2]);
    m_vertexArray->setAttribute(startVertex + 2, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 2, m_texcoordOffset, Vec2(0, 0));

    m_vertexArray->setAttribute(startVertex + 3, m_positionOffset, vertices[0]);
    m_vertexArray->setAttribute(startVertex + 3, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 3, m_texcoordOffset, Vec2(1, 1));

    m_vertexArray->setAttribute(startVertex + 4, m_positionOffset, vertices[2]);
    m_vertexArray->setAttribute(startVertex + 4, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 4, m_texcoordOffset, Vec2(0, 0));

    m_vertexArray->setAttribute(startVertex + 5, m_positionOffset, vertices[3]);
    m_vertexArray->setAttribute(startVertex + 5, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 5, m_texcoordOffset, Vec2(0, 1));

    m_blockVertices[pos].beginVertex[face] = startVertex;
    m_blockVertices[pos].endVertex[face] = startVertex + 5;
}
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
                m_world->clearFace(nearVec, back);
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
                m_world->addFace(nearVec, back);
            }
        }
        updateResource();
    }
}
