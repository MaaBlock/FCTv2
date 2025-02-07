#include "Block.h"

Context* Block::ctx = nullptr;
VertexFactory* Block::factory = nullptr;
ImageLoader* Block::il = nullptr;
Texture* Block::texture = nullptr;
/*
Block* CreateBlock(BlockType type)
{
	Block* ret;
	ret = new Block;
	ret->type = type;
	return ret;
}
*/