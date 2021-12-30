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
#include "chunk.h"

using namespace std::chrono;
/*
const std::vector<int> cube_behind = {
	0,
	1,
	2,
	3,
	4,
	5
};
const std::vector<int> cube_down = {
	 6,
	 7,
	 8,
	 9,
	 10,
	 11
};
const std::vector<int> cube_right = {
	 12,
	 13,
	 14,
	 15,
	 16,
	 17
};
const std::vector<int> cube_up = {
	 18,
	 19,
	 20,
	 21,
	 22,
	 23
};
const std::vector<int> cube_left = {
	 24,
	 25,
	 26,
	 27,
	 28,
	 29
};
const std::vector<int> cube_forward = {
	30,
	31,
	32,
	33,
	34,
	35
};*/

/*
const std::vector<int> cube_behind = {
	0, 0, 0 , 0,0,0,0,-1,
	0, 1, 0 , 0,1,0,0,-1,
	1, 1, 0 , 1,1,0,0,-1,
	0, 0, 0 , 0,0,0,0,-1,
	1, 1, 0 , 1,1,0,0,-1,
	1, 0, 0 , 1,0, 0,0,-1
};
const std::vector<int> cube_down = {
	 1, 0, 1 , 0,0,	0,-1,0,
	 0, 0, 1 , 0,1,	0,-1,0,
	 0, 0, 0 , 1,1,	0,-1,0,
	 1, 0, 1 , 0,0,	0,-1,0,
	 0, 0, 0 , 1,1,	0,-1,0,
	 1, 0, 0 ,  1,0,0,-1,0
};
const std::vector<int> cube_right = {
	 1, 0, 0 ,0,0, 1,0,0,
	 1, 1, 0, 0,1, 1,0,0,
	 1, 1, 1 ,1,1, 1,0,0,
	1, 0, 0 , 0,0, 1,0,0,
	 1, 1, 1 ,1,1, 1,0,0,
	 1, 0, 1 , 1,0, 1,0,0
};
const std::vector<int> cube_up = {
	0, 1, 0 , 0,0, 0,1,0,
	 0, 1, 1 ,0,1, 0,1,0,
	1, 1, 1 , 1,1, 0,1,0,
	 0, 1, 0 ,0,0, 0,1,0,
	 1, 1, 1 ,1,1, 0,1,0,
	 1, 1, 0 , 1,0, 0,1,0
};
const std::vector<int> cube_left = {
	 0, 0, 1 , 0,0,-1,0,0,
	 0, 1, 1 , 0,1,-1,0,0,
	 0, 1, 0 , 1,1,-1,0,0,
	  0, 0, 1 ,0,0,-1,0,0,
	  0, 1, 0 ,1,1,-1,0,0,
	 0, 0, 0  , 1,0,-1,0,0
};
const std::vector<int> cube_forward = {
	1, 0, 1 , 0,0, 0,0,1,
	1, 1, 1 , 0,1, 0,0,1,
	0, 1, 1 , 1,1, 0,0,1,
	 1, 0, 1 ,0,0, 0,0,1,
	0, 1, 1 , 1,1, 0,0,1,
	 0, 0, 1,1,0,  0,0,1
};
*/
/*
const unsigned int cube_behind[] = {
	1u, 2057u, 526361u, 1u, 526361u, 524305u
};
const unsigned int cube_down[] = {
	524322u, 42u, 26u, 524322u, 26u, 524306u
};
const unsigned int cube_right[] = {
	524291u, 526347u, 526395u, 524291u, 526395u, 524339u
};
const unsigned int cube_up[] = {
	2052u, 2092u, 526396u, 2052u, 526396u, 526356u
};
const unsigned int cube_left[] = {
	37u, 2093u, 2077u, 37u, 2077u, 21u
};
const unsigned int cube_forward[] = {
	524326u, 526382u, 2110u, 524326u, 2110u, 54u
};
*/

unsigned int writeBin(unsigned int& x, unsigned int& y, unsigned int& z, bool& tx,bool& ty, unsigned int& n) {
	/*
	n:3
	ty:1
	tx:1
	z:6
	y:8
	z:6
	suma:25
	*/
	unsigned int result = n + (ty<<3u) + (tx<<4u) + (z<<5u) + (y <<11u) + (x<<19u);
	//std::cout << result <<","<< std::endl;
	return result;
}
unsigned int addPosBin(unsigned int& bin, unsigned int& dx, unsigned int& dy, unsigned int& dz) {
	return bin + (dz << 5u) + (dy << 11u) + (dx << 19u);
}
unsigned int getPosDelta(unsigned int dx, unsigned int dy, unsigned int dz) {
	return (dz << 5u) + (dy << 11u) + (dx << 19u);
}

Chunk::Chunk(){
	vbo = 0;
	vao = 0;
	indexPos = { 0,0 };
	localMat = getLocalMat();
}
Chunk::~Chunk()
{
}
void Chunk::Delete() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
Chunk::Chunk(uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y], glm::vec3 pos, glm::ivec2 indexPos)
	:Transform(pos)
{

	this->indexPos = indexPos;
	localMat = getLocalMat();
	//auto start = high_resolution_clock::now();
	//this->data.insert(this->data.end(), &data[0], &data[Constants::BLOCK_COUNT]);


	meshFromData(data);
	//std::cout << "mesh creation: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
}

void Chunk::sendData() {
	if (vertices.size()) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(unsigned int), &vertices[0], GL_STATIC_DRAW);

		

		//glVertexAttribPointer converts to float

		glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(unsigned int), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		empty = false;
	}
}
void Chunk::findNeighboursZ(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]) {
	if (!data[getIndex3DNoise(x,y,z+1)]) {
		vertices.push_back(Vertex{  1 + x, y, 1 + z, 0, 0, 0, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 0, 1, 0, textureID});
		vertices.push_back(Vertex{	x, 1 + y, 1 + z, 1, 1, 0, textureID});

		vertices.push_back(Vertex{	1 + x, y, 1 + z, 0, 0, 0, textureID});
		vertices.push_back(Vertex{	x, 1 + y, 1 + z, 1, 1, 0, textureID});
		vertices.push_back(Vertex{	x, y, 1 + z, 1, 0, 0, textureID});
	}
	if (!data[getIndex3DNoise(x, y, z - 1)]) {
		vertices.push_back(Vertex{ x,y, z, 0, 0, 5, textureID });
		vertices.push_back(Vertex{ x, 1 + y,z, 0, 1, 5, textureID });
		vertices.push_back(Vertex{ 1 + x, 1 + y, z, 1, 1, 5, textureID });

		vertices.push_back(Vertex{ x, y, z, 0, 0, 5, textureID });
		vertices.push_back(Vertex{ 1 + x, 1 + y, z, 1, 1, 5, textureID });
		vertices.push_back(Vertex{ 1 + x, y, z, 1, 0, 5, textureID });
	}
}
void Chunk::findNeighboursY(unsigned int& textureID, unsigned int& x, unsigned int& y, unsigned int& z, uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]) {
	if (!data[getIndex3DNoise(x, y+1, z)]) {
		vertices.push_back(Vertex{  x, 1 + y, z, 0, 0, 3, textureID});
		vertices.push_back(Vertex{	x, 1 + y, 1 + z, 0, 1, 3, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 1, 1, 3, textureID});

		vertices.push_back(Vertex{	x, 1 + y, z, 0, 0, 3, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 1, 1, 3, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, z, 1, 0, 3, textureID});
	}
	if (!data[getIndex3DNoise(x, y-1, z)]) {
		vertices.push_back(Vertex{  1 + x, y, 1 + z, 0, 0, 1, textureID});
		vertices.push_back(Vertex{	x, y, 1 + z, 0, 1, 1, textureID});
		vertices.push_back(Vertex{	x, y, z, 1, 1, 1, textureID});

		vertices.push_back(Vertex{	1 + x, y, 1 + z, 0, 0, 1, textureID});
		vertices.push_back(Vertex{	x, y, z, 1, 1, 1, textureID});
		vertices.push_back(Vertex{	1 + x, y, z, 1, 0, 1, textureID});
	}
}
void Chunk::findNeighboursX(unsigned int& textureID, unsigned int& x, unsigned int& y, unsigned int& z, uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]) {
	if (!data[getIndex3DNoise(x+1, y, z)]) {
		vertices.push_back(Vertex{  1 + x, y, z, 0, 0,2, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, z, 0, 1,2, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 1, 1,2, textureID});

		vertices.push_back(Vertex{	1 + x, y, z, 0, 0,2, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 1, 1,2, textureID});
		vertices.push_back(Vertex{	1 + x, y, 1 + z, 1, 0,2, textureID});
	}
	if (!data[getIndex3DNoise(x-1, y, z)]) {
		vertices.push_back(Vertex{  x, y, 1 + z, 0, 0, 4, textureID});
		vertices.push_back(Vertex{	x, 1 + y, 1 + z, 0, 1, 4, textureID});
		vertices.push_back(Vertex{	x, 1 + y, z, 1, 1, 4, textureID});

		vertices.push_back(Vertex{	x, y, 1 + z, 0, 0, 4, textureID});
		vertices.push_back(Vertex{	x, 1 + y, z, 1, 1, 4, textureID});
		vertices.push_back(Vertex{	x, y, z, 1, 0, 4, textureID});
	}
}

void Chunk::meshFromData(uint8_t data[Constants::NOISE_ARRAY_SIZE * Constants::CHUNK_SIZE_Y]) {
	unsigned int blockType;
	for (unsigned int z = 1u; z < Constants::CHUNK_SIZE_Z + 1u; z++)
	{
		for (unsigned int y = 0u; y < Constants::CHUNK_SIZE_Y; y++)
		{
			for (unsigned int x = 1u; x < Constants::CHUNK_SIZE_X + 1u; x++)
			{
				if (blockType = data[getIndex3DNoise(x,y,z)]) {
					blockType = static_cast<unsigned int>(BlockType::WATER);
					findNeighboursX(blockType,x, y, z, data);
					findNeighboursY(blockType,x, y, z, data);
					findNeighboursZ(blockType,x, y, z, data);
				}
			}
		}
	}
}