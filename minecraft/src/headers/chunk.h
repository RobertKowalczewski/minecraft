#pragma once
#include <FastNoise/FastNoise.h>
#include"vertex.h"


class Chunk: public Transform {
public:
	glm::uvec2 indexPos;
	glm::mat4 localMat;
	unsigned int vao;
	unsigned int vbo;
	bool empty=true;
	std::vector<unsigned char> data;
	std::vector<Vertex> vertices;
	Chunk(unsigned char data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y], glm::vec3 pos, glm::ivec2 indexPos);
	Chunk();
	~Chunk();
	void Delete();
	void sendData();
	void findNeighboursZ(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]);
	void findNeighboursX(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]);
	void findNeighboursY(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]);

	void meshFromData(unsigned char data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]);

};