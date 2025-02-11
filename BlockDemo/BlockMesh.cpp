#include "BlockMesh.h"
#include "World.h"
void BlockMesh::setWorld(World* world)
{
    m_world = world;
    m_phySys = m_world->m_phySys;
    m_pxMarerial = m_phySys->createMaterial(0.7, 0.5, 0);
    m_physxShape = nullptr;
    m_actor = m_phySys->createStaticRigidBody(Vec3(0, 0, 0));
    world->m_scene->addActor(*m_actor);
}
void BlockMesh::addFace(Vec3 pos, BlockFace face, unsigned id, Vec4 color)
{
    auto drawId = getFaceId(id, face);
    Vec3 halfNormal = getHalfNomal(face);
    Vec3 halfTangent = getHalfTangent(face);
    Vec3 halfBitangent = getHalfBitangent(face);
    Vec3 vertices[4];
    vertices[0] = pos + halfNormal + halfTangent + halfBitangent;
    vertices[1] = pos + halfNormal + halfTangent - halfBitangent;
    vertices[2] = pos + halfNormal - halfTangent - halfBitangent;
    vertices[3] = pos + halfNormal - halfTangent + halfBitangent;
	Vec2 texcoords[4];
	texcoords[0] = Vec2(0, 0);
	texcoords[1] = Vec2(0, 1);
	texcoords[2] = Vec2(1, 1);
    texcoords[3] = Vec2(1, 0);
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
    m_vertexArray->setAttribute(startVertex, m_texcoordOffset, texcoords[0]);
    m_vertexArray->setAttribute(startVertex, m_typeIdOffset, drawId);

    m_vertexArray->setAttribute(startVertex + 1, m_positionOffset, vertices[1]);
    m_vertexArray->setAttribute(startVertex + 1, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 1, m_texcoordOffset, texcoords[1]);
    m_vertexArray->setAttribute(startVertex + 1, m_typeIdOffset, drawId);

    m_vertexArray->setAttribute(startVertex + 2, m_positionOffset, vertices[2]);
    m_vertexArray->setAttribute(startVertex + 2, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 2, m_texcoordOffset, texcoords[2]);
    m_vertexArray->setAttribute(startVertex + 2, m_typeIdOffset, drawId);

    m_vertexArray->setAttribute(startVertex + 3, m_positionOffset, vertices[0]);
    m_vertexArray->setAttribute(startVertex + 3, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 3, m_texcoordOffset, texcoords[0]);
    m_vertexArray->setAttribute(startVertex + 3, m_typeIdOffset, drawId);

    m_vertexArray->setAttribute(startVertex + 4, m_positionOffset, vertices[2]);
    m_vertexArray->setAttribute(startVertex + 4, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 4, m_texcoordOffset, texcoords[2]);
    m_vertexArray->setAttribute(startVertex + 4, m_typeIdOffset, drawId);

    m_vertexArray->setAttribute(startVertex + 5, m_positionOffset, vertices[3]);
    m_vertexArray->setAttribute(startVertex + 5, m_colorOffset, color);
    m_vertexArray->setAttribute(startVertex + 5, m_texcoordOffset, texcoords[3]);
    m_vertexArray->setAttribute(startVertex + 5, m_typeIdOffset, drawId);

    m_blockVertices[pos].beginVertex[face] = startVertex;
    m_blockVertices[pos].endVertex[face] = startVertex + 5;
}
void BlockMesh::updata()
{
    if (m_needUpdataResource) {
        updataRenderResource();
        updataPhysxResource();
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
                                        addFace(block.second->pos, BlockFace(i), block.second->type);
                                    }
                                }
                            }
                            m_needUpdataResource = true;
                            return; 
                            });
        thread.detach();
    }
}

void BlockMesh::addBlock(const Vec3& position, Chunk* chunk)
{
    if (m_isReady) {
        for (int i = BlockFace::BlockFace_Begin + 1; i < BlockFace::BlockFace_End; i++) {
            auto back = getBackFace(BlockFace(i));
            auto nearVec = position + getNomal(BlockFace(i));
            if (m_world->isBlockAt(nearVec)) {
                m_world->clearFace(nearVec, back);
            }
            else {
                m_world->addFace(position, BlockFace(i));
            }
        }
        updataRenderResource();
        updataPhysxResource();
    }
}
void BlockMesh::removeBlock(const Vec3& pos, Chunk* chunk)
{
    if (m_isReady) {
        for (int i = BlockFace::BlockFace_Begin + 1; i < BlockFace::BlockFace_End; i++) {
            auto back = getBackFace(BlockFace(i));
            auto nearVec = pos + getNomal(BlockFace(i));
            if (m_blockVertices[pos].endVertex[i]) {
                m_world->clearFace(pos, BlockFace(i));
            }
            if (m_world->isBlockAt(nearVec)) {
                m_world->addFace(nearVec, back, m_world->getBlockType(nearVec));
            }
        }
        updataRenderResource();
        updataPhysxResource();
    }
}

void BlockMesh::lightenFace(const Vec3& pos, BlockFace face, float light) {
    Vec4 color = Vec4(light, light, light, 1.0);
	if (m_blockVertices[pos].endVertex[face]) {
        int startVertex = m_blockVertices[pos].beginVertex[face];
        m_vertexArray->setAttribute(startVertex + 0, m_colorOffset, color);
        m_vertexArray->setAttribute(startVertex + 1, m_colorOffset, color);
        m_vertexArray->setAttribute(startVertex + 2, m_colorOffset, color);
        m_vertexArray->setAttribute(startVertex + 3, m_colorOffset, color);
        m_vertexArray->setAttribute(startVertex + 4, m_colorOffset, color);
        m_vertexArray->setAttribute(startVertex + 5, m_colorOffset, color);
    }
}
