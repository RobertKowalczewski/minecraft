#pragma once
#include <FastNoise/FastNoise.h>


class Chunk: public Transform {
public:
	glm::uvec2 indexPos;
	glm::mat4 localMat;
	unsigned int vao;
	unsigned int vbo;
	bool empty=true;
	std::vector<unsigned char> data;
	std::vector<unsigned int> vertices;
	Chunk(unsigned char data[Constants::BLOCK_COUNT], glm::vec3 pos, glm::ivec2 indexPos);
	Chunk();
	~Chunk();
	void Delete();
	void sendData();
	void findNeighboursZ(unsigned int& x, unsigned int& y, unsigned int& z, unsigned char data[Constants::BLOCK_COUNT]);
	void findNeighboursX(unsigned int& x, unsigned int& y, unsigned int& z, unsigned char data[Constants::BLOCK_COUNT]);
	void findNeighboursY(unsigned int& x, unsigned int& y, unsigned int& z, unsigned char data[Constants::BLOCK_COUNT]);

	void meshFromData(unsigned char data[Constants::BLOCK_COUNT]);

};