#include "Block.h"

Context* Block::ctx = nullptr;
VertexFactory* Block::factory = nullptr;
ImageLoader* Block::il = nullptr;
TextureArray* Block::texture = nullptr;
std::string BlockIdMap[] =
{
	"Stone",
	"Dirt",
	"Grass"
};
const int BlockTypeSize = 3;
/*
Block* CreateBlock(BlockType type)
{
	Block* ret;
	ret = new Block;
	ret->type = type;
	return ret;
}
*/