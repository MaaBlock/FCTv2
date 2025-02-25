#pragma once

#include "Chunk.h"
#include <queue>

enum BlockFace : unsigned int
{
    BlockFace_Begin = 0,
    Right_Xpositive,
    Left_Xnegative,
    Top_Ypositive,
    Bottom_Ynegative,
    Front_Zpositive,
    Back_Znegative,
    BlockFace_End,
};
inline BlockFace getBackFace(BlockFace face) {
    switch (face)
    {
    case BlockFace::Right_Xpositive:
        return BlockFace::Left_Xnegative;
    case BlockFace::Left_Xnegative:
        return BlockFace::Right_Xpositive;
    case BlockFace::Top_Ypositive:
        return BlockFace::Bottom_Ynegative;
    case BlockFace::Bottom_Ynegative:
        return BlockFace::Top_Ypositive;
    case BlockFace::Front_Zpositive:
        return BlockFace::Back_Znegative;
    case BlockFace::Back_Znegative:
        return BlockFace::Front_Zpositive;
    default:
        return BlockFace::BlockFace_End;
    }
}
inline BlockFace getFaceFromNormal(const Vec3& normal)
{
    const float epsilon = 0.001f;

    if (std::abs(normal.x - 1.0f) < epsilon) {
        return BlockFace::Right_Xpositive;
    }
    if (std::abs(normal.x + 1.0f) < epsilon) {
        return BlockFace::Left_Xnegative;
    }

    if (std::abs(normal.y - 1.0f) < epsilon) {
        return BlockFace::Top_Ypositive;
    }
    if (std::abs(normal.y + 1.0f) < epsilon) {
        return BlockFace::Bottom_Ynegative;
    }

    if (std::abs(normal.z - 1.0f) < epsilon) {
        return BlockFace::Front_Zpositive;
    }
    if (std::abs(normal.z + 1.0f) < epsilon) {
        return BlockFace::Back_Znegative;
    }

    return BlockFace::BlockFace_End;
}

inline Vec3 getHalfNomal(BlockFace face)
{
    switch (face)
    {
    case BlockFace::Right_Xpositive:
        return Vec3(0.5f, 0, 0);
    case BlockFace::Left_Xnegative:
        return Vec3(-0.5f, 0, 0);
    case BlockFace::Top_Ypositive:
        return Vec3(0, 0.5f, 0);
    case BlockFace::Bottom_Ynegative:
        return Vec3(0, -0.5f, 0);
    case BlockFace::Front_Zpositive:
        return Vec3(0, 0, 0.5f);
    case BlockFace::Back_Znegative:
        return Vec3(0, 0, -0.5f);
    default:
        return Vec3(0, 0, 0);
    }
}
inline Vec3 getNomal(BlockFace face){
    return getHalfNomal(face) * 2;
}
inline Vec3 getHalfTangent(BlockFace face)
{
    switch (face)
    {
    case BlockFace::Right_Xpositive:
        return Vec3(0, 0, 0.5f);
    case BlockFace::Left_Xnegative:
        return Vec3(0, 0, -0.5f);
    case BlockFace::Top_Ypositive:
        return Vec3(0.5f, 0, 0);
    case BlockFace::Bottom_Ynegative:
        return Vec3(0.5f, 0, 0);
    case BlockFace::Front_Zpositive:
        return Vec3(-0.5f, 0, 0);
    case BlockFace::Back_Znegative:
        return Vec3(0.5f, 0, 0);
    default:
        return Vec3(0, 0, 0);
    }
}

inline Vec3 getHalfBitangent(BlockFace face)
{
    switch (face)
    {
    case BlockFace::Right_Xpositive:
        return Vec3(0, 0.5f, 0);
    case BlockFace::Left_Xnegative:
        return Vec3(0, 0.5f, 0);
    case BlockFace::Top_Ypositive:
        return Vec3(0, 0, 0.5f);
    case BlockFace::Bottom_Ynegative:
        return Vec3(0, 0, -0.5f);
    case BlockFace::Front_Zpositive:
        return Vec3(0, 0.5f, 0);
    case BlockFace::Back_Znegative:
        return Vec3(0, 0.5f, 0);
    default:
        return Vec3(0, 0, 0);
    }
}
inline float getFaceId(size_t blockId, BlockFace face){
    switch (face)
    {
    case BlockFace_Begin:
        break;
    case Right_Xpositive:
    case Left_Xnegative:
    case Front_Zpositive:
    case Back_Znegative:
		return blockId;
        break;
    case Top_Ypositive:
        return blockId + BlockTypeSize;
        break;
    case Bottom_Ynegative:
        return blockId + 2 * BlockTypeSize;
        break;
    case BlockFace_End:
        break;
    default:
        break;
    }
}
class World;
class BlockMesh : public Object
{
private:
    Pipeline *m_pl;
    size_t m_positionOffset;
    size_t m_colorOffset;
    size_t m_texcoordOffset;
    size_t m_typeIdOffset;
    VertexArray *m_vertexArray;
    PhysicsSystem* m_phySys;
    physx::PxTriangleMesh* m_physxMesh;
    physx::PxShape* m_physxShape;
    physx::PxMaterial* m_pxMarerial;
    physx::PxRigidStatic* m_actor;
    struct BlockVertex
    {
        BlockVertex() : beginVertex{}, endVertex{} {

        }
        int beginVertex[BlockFace::BlockFace_End];
        int endVertex[BlockFace::BlockFace_End];
    };
    struct ReuseBlockVertex {
        int beginVertex;
        int endVertex;
    };
    std::unordered_map<Vec3, BlockVertex> m_blockVertices;
    std::queue<ReuseBlockVertex> m_memroy;
    DrawCall *m_dc;
    VertexBuffer *m_buffer;
    InputLayout *m_inputlayout;
    TextureArray *m_texture;
    World* m_world;
    bool m_isReady;
    bool m_needUpdataResource = false;
    static const int CHUNK_SIZE = 16;
    static const int CHUNK_HEIGHT = 256;

    bool m_onlyDrawLines;
public:
    void updataPhysxResource() {
        if (m_physxMesh) {
            m_physxMesh->release();
        }
        if (m_physxShape) {
            m_actor->detachShape(*m_physxShape);
            m_physxShape->release();
        }
        m_physxMesh = m_phySys->createTriangleMeshNoIndices(m_vertexArray);
        m_physxShape = m_phySys->createTriangleMeshShape(m_physxMesh,m_pxMarerial);
        m_actor->attachShape(*m_physxShape);
    }
    void create()
    {
    }
    void onlyDrawLines(bool onlyDrawLines) {
        if (onlyDrawLines)
            m_dc->primitive(PrimitiveType::Lines);
        else 
            m_dc->primitive(PrimitiveType::Triangles);
    }
    void draw() const {
        if (!m_isReady)
            return;
        Object::draw();
        //std::cout << "sucess" << std::endl;
    }
    BlockMesh(Pipeline *pl) : m_pl(pl), Object(pl)
    {
        m_vertexArray = new VertexArray(m_pl->getVertexFactory(), 0);
        m_positionOffset = m_vertexArray->getAttributeOffset("position");
        m_colorOffset = m_vertexArray->getAttributeOffset("color");
        m_texcoordOffset = m_vertexArray->getAttributeOffset("TexCoord");
        m_typeIdOffset = m_vertexArray->getAttributeOffset("typeId");
        Context *ctx = m_pl->getContext();
        m_buffer = ctx->createVertexBuffer(m_vertexArray);
        m_buffer->create(ctx);
        m_inputlayout = ctx->createInputLayout(m_pl->getVertexFactory());
        m_inputlayout->create(ctx, m_buffer);
        m_dc = ctx->createDrawCall(PrimitiveType::Triangles, 0, 0);
        m_texture = nullptr;
        m_resources.push_back(m_texture);
        m_resources.push_back(m_inputlayout);
        m_resources.push_back(m_buffer);
        m_resources.push_back(m_dc);
        m_isReady = false;
        m_physxMesh = nullptr;
    }
    void setWorld(World* world);
    void addFace(Vec3 pos, BlockFace face,  unsigned id = 0, Vec4 color = Vec4(1, 1, 1, 1));
    void updata();
    void updataResource() {
        updataRenderResource();
        updataPhysxResource();
    }
    void updataRenderResource()
    {
        m_buffer->updata();
        m_dc->setCount(m_vertexArray->getVertexCount());
    }

    void texture(TextureArray*texture)
    {
        m_texture = texture;
        m_resources[0] = texture;
    }

    ~BlockMesh()
    {
        delete m_vertexArray;
    }
    bool isReady() const { return m_isReady; }
    void generateMesh(const Chunk &chunk);
    void clearFace(Vec3 vec, BlockFace face) {
        if (m_blockVertices[vec].endVertex[face]) {
            //std::cout << m_blockVertices[vec].beginVertex[face] << ":" << m_blockVertices[vec].endVertex[face] << std::endl;
            for (int i = m_blockVertices[vec].beginVertex[face]; i <= m_blockVertices[vec].endVertex[face]; i++) {
                m_vertexArray->setAttribute(i, m_colorOffset, Vec4(0, 0, 0, 0));
                m_vertexArray->setAttribute(i, m_positionOffset, Vec3(0, 0, 0));
            }
            m_memroy.push({ m_blockVertices[vec].beginVertex[face], m_blockVertices[vec].endVertex[face] });
            m_blockVertices[vec].endVertex[face] = 0;
            m_blockVertices[vec].beginVertex[face] = 0;
        }
    }
    void addBlock(const Vec3& position, Chunk* chunk);
    void removeBlock(const Vec3& pos, Chunk* chunk);
	void lightenFace(const Vec3& pos, BlockFace face,float light);
private:
};