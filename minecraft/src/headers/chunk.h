#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include<vector>
#include <GL/glew.h>
#include <bitset>
#include <chrono>
#include<queue>
#include <FastNoise/FastNoise.h>

#include"constants.h"
#include"functions.h"
#include "transform.h"
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
	Chunk(std::vector<uint8_t>& data, glm::vec3 pos, glm::ivec2 indexPos);

	Chunk();
	~Chunk();
	void Delete();
	void sendData();
	void findNeighboursZ(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, std::vector<uint8_t>& data);
	void findNeighboursX(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, std::vector<uint8_t>& data);
	void findNeighboursY(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, std::vector<uint8_t>& data);
	void meshFromData(std::vector<uint8_t>& data);

};