#pragma once

struct Block {
	unsigned int data = 0;
	
	Block(BlockType type, uint8_t x, uint8_t y, uint8_t z, bool tx, bool ty, uint8_t n);
	void write(uint8_t& x, uint8_t& y, uint8_t& z, bool& tx, bool& ty, uint8_t& n, uint8_t texID);
	glm::vec3 getPos();
	unsigned int getType();
};