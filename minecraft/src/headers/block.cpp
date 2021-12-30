#include"constants.h"
#include <stdint.h>
#include <glm/glm.hpp>
#include<iostream>

#include"block.h"

Block::Block(BlockType type, uint8_t x, uint8_t y, uint8_t z, bool tx, bool ty, uint8_t n)
{
	/*
		texID/BlockType:7
		n:3
		ty:1
		tx:1
		z:6
		y:8
		x:6
	*/
	write(x, y, z, tx, ty, n, (uint8_t)type);
}

void Block::write(uint8_t& x, uint8_t& y, uint8_t& z, bool& tx, bool& ty, uint8_t& n, uint8_t texID){
		data = n + (ty << 3u) + (tx << 4u) + (z << 5u) + (y << 11u) + (x << 19u) + (texID << 25u);
}

glm::vec3 Block::getPos()
{
	return { float((data & 0x1F80000u) >> 19u),
		float((data & 0x7F800u) >> 11u),
		float((data & 0x7E0u) >> 5u) };
}

unsigned int Block::getType()
{
	return data >> 25u;
}
