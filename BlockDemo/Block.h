#pragma once
#include "../FCT/headers.h"
using namespace FCT;
extern std::string BlockIdMap[];
extern const int BlockTypeSize;
/*
enum class BlockType {
	grass
};
struct Block : public RefCount
{
	BlockType type;
};
Block* CreateBlock(BlockType type);
*/

class Block
{
public:
    static Context* ctx;
    static VertexFactory* factory;
    static ImageLoader* il;
    static TextureArray* texture;
    unsigned type;
    Vec3 pos;
    static std::vector<std::string> getPaths() {
        std::vector<std::string> ret;
        for (int i = 0; i < BlockTypeSize; i++) {
            ret.push_back("res/" + BlockIdMap[i] + "_Side.png");
        }
        for (int i = 0; i < BlockTypeSize; i++) {
            ret.push_back("res/" + BlockIdMap[i] + "_Top.png");
        }
        for (int i = 0; i < BlockTypeSize; i++) {
            ret.push_back("res/" + BlockIdMap[i] + "_Bottom.png");
        }
        return ret;
    }
    static void Init(Context* ctx, VertexFactory* factory, ImageLoader* il)
    {
        Block::ctx = ctx;
        Block::factory = factory;
        Block::il = il;
        texture = ctx->createTextureArray();
        texture->create(64, 64, BlockTypeSize * 3, TextureArray::Format::RGBA8);
        texture->setSlot(3);
        texture->loadFromFile(getPaths(), il);

    }
    Block()
    {
    }
    ~Block()
    {
    }
    void setPos(Vec3 pos)
    {
        this->pos = pos;
    }
    void render(Pipeline* pipeline)
    {
        //pipeline->draw(box);
    }
};
