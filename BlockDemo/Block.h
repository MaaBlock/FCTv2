#pragma once
#include "../FCT/headers.h"
using namespace FCT;
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
    Box* box;
    static Context* ctx;
    static VertexFactory* factory;
    static ImageLoader* il;
    static Texture* texture;
    Vec3 pos;
    static void Init(Context* ctx, VertexFactory* factory, ImageLoader* il)
    {
        Block::ctx = ctx;
        Block::factory = factory;
        Block::il = il;
        texture = ctx->createTexture();
        texture->setSlot(3);
        if (texture->loadFromFile("MaBlock.png", il))
        {
            std::cout << "Texture loaded successfully" << std::endl;
        }
    }
    Block()
    {
        box = new Box(ctx, factory);
        box->texture(texture);
        box->size(FCT::Vec3(1, 1, 1));
        box->setTextureCoordinates();
        box->color(Vec4(1, 1, 1, 1));
        box->create();
    }
    ~Block()
    {
        box->release();
    }
    void setPos(Vec3 pos)
    {
        this->pos = pos;
        box->setPosition(pos);
    }
    void render(Pipeline* pipeline)
    {
        //pipeline->draw(box);
    }
};
