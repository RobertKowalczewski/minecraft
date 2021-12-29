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
const std::vector<unsigned int> cube_behind = {
	5,
	262157,
	17039389,
	5,
	17039389,
	16777237,
};
const std::vector<unsigned int> cube_down = {
	16778244,
	1036,
	28,
	16778244,
	28,
	16777236,
};
const std::vector<unsigned int> cube_right = {
	16777216,
	17039368,
	17040408,
	16777216,
	17040408,
	16778256,
};
const std::vector<unsigned int> cube_up = {
	262145,
	263177,
	17040409,
	262145,
	17040409,
	17039377,
};
const std::vector<unsigned int> cube_left = {
	1027,
	263179,
	262171,
	1027,
	262171,
	19,
};
const std::vector<unsigned int> cube_forward = {
	16778242,
	17040394,
	263194,
	16778242,
	263194,
	1042,
};
*/
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

/*
const std::vector<unsigned char> cube_behind = {
	0 ,
	2 ,
	6 ,
	0 ,
	6 ,
	4
};
const std::vector<unsigned char> cube_down = {
	5 ,
	1 ,
	0 ,
	5 ,
	0 ,
	4
};
const std::vector<unsigned char> cube_right = {
	4 ,
	6,
	7 ,
	4 ,
	7 ,
	5
};
const std::vector<unsigned char> cube_up = {
	2 ,
	3 ,
	7 ,
	2 ,
	7 ,
	6
};
const std::vector<unsigned char> cube_left = {
	1 ,
	3 ,
	2 ,
	1 ,
	2 ,
	0
};
const std::vector<unsigned char> cube_forward = {
	5 ,
	7 ,
	3 ,
	5 ,
	3 ,
	1
};
*/
/*
const std::vector<unsigned char> cube_behind = {
	0, 0, 0 ,
	0, 1, 0 ,
	1, 1, 0 ,
	0, 0, 0 ,
	1, 1, 0 ,
	1, 0, 0 
};
const std::vector<unsigned char> cube_down = {
	 1, 0, 1 ,
	 0, 0, 1 ,
	 0, 0, 0 ,
	 1, 0, 1 ,
	 0, 0, 0 ,
	 1, 0, 0 
};
const std::vector<unsigned char> cube_right = {
	 1, 0, 0 ,
	 1, 1, 0,
	 1, 1, 1 ,
	1, 0, 0 ,
	 1, 1, 1 ,
	 1, 0, 1 
};
const std::vector<unsigned char> cube_up = {
	0, 1, 0 ,
	 0, 1, 1 ,
	1, 1, 1 ,
	 0, 1, 0 ,
	 1, 1, 1 ,
	 1, 1, 0 
};
const std::vector<unsigned char> cube_left = {
	 0, 0, 1 ,
	 0, 1, 1 ,
	 0, 1, 0 ,
	  0, 0, 1 ,
	  0, 1, 0 ,
	 0, 0, 0 
};
const std::vector<unsigned char> cube_forward = {
	1, 0, 1 ,
	1, 1, 1 ,
	0, 1, 1 ,
	 1, 0, 1 ,
	0, 1, 1 ,
	 0, 0, 1 
};
*/
Chunk::Chunk(){
	vbo = 0;
	vao = 0;
	seed = 0;
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
Chunk::Chunk(unsigned char data[Constants::BLOCK_COUNT], glm::vec3 pos, glm::ivec2 indexPos, FastNoise::SmartNode<>& noise, unsigned int seed, bool safeSlices[Constants::CHUNK_SIZE_Y])
	:Transform(pos)
{
	/*unsigned int dataBin = writeBin(10, 70, 30, 1, 1, 3);

	unsigned int normalIndex = dataBin & 0x7;
	glm::vec3 aNormal = glm::vec3(normals[normalIndex * 3], normals[normalIndex * 3+ 1], normals[normalIndex * 3 + 2]);

	glm::vec2 aTexCoord = glm::vec2(float((dataBin & 0x10) >> 4), float((dataBin & 0x8) >> 3));

	glm::vec4 aPos = glm::vec4(float((dataBin & 0x1F80000) >> 19),
		float((dataBin & 0x7F800) >> 11),
		float((dataBin & 0x7E0) >> 5),
		1.0);

	std::cout << std::bitset<32>(dataBin) << std::endl;
	std::cout << aPos.x <<" "<<aPos.y<<" "<<aPos.z << std::endl;*/
	/*std::cout << "cube_behind" << std::endl;
	for (int i = 0; i < cube_behind.size();i+=8) {
		unsigned int bin = writeBin(cube_behind[i], cube_behind[i + 1], cube_behind[i + 2], cube_behind[i + 3], cube_behind[i + 4], 1);
		std::cout << bin << ", ";
	}
	std::cout<<std::endl;
	std::cout << "cube_down" << std::endl;
	for (int i = 0; i < cube_down.size(); i += 8) {
		unsigned int bin = writeBin(cube_down[i], cube_down[i + 1], cube_down[i + 2], cube_down[i + 3], cube_down[i + 4], 2);
		std::cout << bin << ", ";
	}
	std::cout << std::endl;
	std::cout << "cube_right" << std::endl;
	for (int i = 0; i < cube_right.size(); i += 8) {
		unsigned int bin = writeBin(cube_right[i], cube_right[i + 1], cube_right[i + 2], cube_right[i + 3], cube_right[i + 4], 3);
		std::cout << bin << ", ";
	}
	std::cout << std::endl;

	std::cout << "cube_up" << std::endl;
	for (int i = 0; i < cube_up.size(); i += 8) {
		unsigned int bin = writeBin(cube_up[i], cube_up[i + 1], cube_up[i + 2], cube_up[i + 3], cube_up[i + 4], 4);
		std::cout << bin << ", ";
	}
	std::cout << std::endl;

	std::cout << "cube_left" << std::endl;
	for (int i = 0; i < cube_left.size(); i += 8) {
		unsigned int bin = writeBin(cube_left[i], cube_left[i + 1], cube_left[i + 2], cube_left[i + 3], cube_left[i + 4], 5);
		std::cout << bin << ", ";
	}
	std::cout << std::endl;
	std::cout << "cube_forward" << std::endl;
	for (int i = 0; i < cube_forward.size(); i += 8) {
		unsigned int bin = writeBin(cube_forward[i], cube_forward[i + 1], cube_forward[i + 2], cube_forward[i + 3], cube_forward[i + 4], 6);
		std::cout << bin << ", ";
	}
	*/

	//vertices.reserve(36 * Constants::BLOCK_COUNT / 2);

	this->seed = seed;
	this->indexPos = indexPos;
	localMat = getLocalMat();

	auto start = high_resolution_clock::now();
	//this->data.insert(this->data.end(), &data[0], &data[Constants::BLOCK_COUNT]);


	meshFromData(safeSlices, data);
	//meshFromData2(noise, data);

	std::cout << "mesh creation: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
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

void Chunk::findNeighboursZ(unsigned int& x, unsigned int& y, unsigned int& z, unsigned char data[Constants::BLOCK_COUNT]) {
	if (z == Constants::CHUNK_SIZE_Z - 1 || !data[getIndex3D(x,y,z+1)]) {
		unsigned int posDelta = getPosDelta(x, y, z);
		for (unsigned int n : cube_forward) {
			vertices.push_back(n + posDelta);
		}
	}
	if (!z || !data[getIndex3D(x, y, z - 1)]) {
		unsigned int posDelta = getPosDelta(x, y, z);
		for (unsigned int n : cube_behind) {
			vertices.push_back(n + posDelta);
		}
	}
}
void Chunk::findNeighboursY(unsigned int& x, unsigned int& y, unsigned int& z, unsigned char data[Constants::BLOCK_COUNT]) {
	if (y == Constants::CHUNK_SIZE_Y - 1 || !data[getIndex3D(x, y+1, z)]) {
		unsigned int posDelta = getPosDelta(x, y, z);
		for (unsigned int n : cube_up) {
			vertices.push_back(n + posDelta);
		}
	}
	if (!y || !data[getIndex3D(x, y-1, z)]) {
		unsigned int posDelta = getPosDelta(x, y, z);
		for (unsigned int n : cube_down) {
			vertices.push_back(n + posDelta);
		}
	}
}
void Chunk::findNeighboursX(unsigned int& x, unsigned int& y, unsigned int& z, unsigned char data[Constants::BLOCK_COUNT]) {
	if (x == Constants::CHUNK_SIZE_X - 1 || !data[getIndex3D(x+1, y, z)]) {
		unsigned int posDelta = getPosDelta(x, y, z);
		for (unsigned int n : cube_right) {
			vertices.push_back(n + posDelta);
		}
	}
	if (!x || !data[getIndex3D(x-1, y, z)]) {
		unsigned int posDelta = getPosDelta(x, y, z);
		for (unsigned int n : cube_left) {
			vertices.push_back(n + posDelta);
		}
	}
}

void Chunk::meshFromData(bool safeSlices[Constants::CHUNK_SIZE_Y], unsigned char data[Constants::BLOCK_COUNT]) {
	int index = 0;

	for (unsigned int z = 0u; z < Constants::CHUNK_SIZE_Z; z++)
	{
		for (unsigned int y = 0u; y < Constants::CHUNK_SIZE_Y; y++)
		{
			for (unsigned int x = 0u; x < Constants::CHUNK_SIZE_X; x++)
			{
				if (data[index]) {
					findNeighboursX(x, y, z, data);
					findNeighboursY(x, y, z, data);
					findNeighboursZ(x, y, z, data);
				}
				index++;
			}
		}
	}
}
void Chunk::meshFromData2(FastNoise::SmartNode<>& noise, unsigned char data[Constants::BLOCK_COUNT]) {
	int last = 0;
	int val = 0;
	unsigned int posDelta = 0;

	for (unsigned int x = 0; x < Constants::CHUNK_SIZE_X; x++)
	{

		for (unsigned int y = 0; y < Constants::CHUNK_SIZE_Y; y++)
		{
			last = 0;
			for (unsigned int z = 0; z < Constants::CHUNK_SIZE_Z; z++)
			{
				val = data[getIndex3D(x,y,z)];
				if (val != last) {
					posDelta = getPosDelta(x, y, z);
					if (last) {
						posDelta = getPosDelta(x, y, z - 1);
						for (unsigned int v : cube_forward) {
							vertices.push_back(v + posDelta);
						}
					}
					else {
						for (unsigned int v : cube_behind) {
							vertices.push_back(v + posDelta);
						}
					}
				}
				last = val;
			}
			if (val) {
				posDelta = getPosDelta(x, y, Constants::CHUNK_SIZE_Z-1);
				for (unsigned int v : cube_forward) {
					/*if (noise->GenSingle3D(x + pos.x, y + pos.y, Constants::CHUNK_SIZE_Z + pos.z,seed)) {
						vertices.push_back(v + posDelta);
					}*/
					vertices.push_back(v + posDelta);

				}
			}
		}
	}

	for (unsigned int x = 0; x < Constants::CHUNK_SIZE_X; x++)
	{
		for (unsigned int z = 0; z < Constants::CHUNK_SIZE_Z; z++)
		{
			last = 0;
			for (unsigned int y = 0; y < Constants::CHUNK_SIZE_Y; y++)
			{
				val = data[getIndex3D(x, y, z)];
				if (val != last) {
					if (last) {
						posDelta = getPosDelta(x, y - 1, z);
						for (unsigned int v : cube_up) {
							vertices.push_back(v + posDelta);
						}
					}
					else {
						posDelta = getPosDelta(x, y, z);
						for (unsigned int v : cube_down) {
							vertices.push_back(v + posDelta);
						}
					}
				}
				last = val;
			}
			if (val) {
				posDelta = getPosDelta(x, Constants::CHUNK_SIZE_Y - 1, z);
				for (unsigned int v : cube_up) {
					vertices.push_back(v + posDelta);
				}
			}
		}
	}

	for (unsigned int y = 0; y < Constants::CHUNK_SIZE_Y; y++)
	{
		for (unsigned int z = 0; z < Constants::CHUNK_SIZE_Z; z++)
		{
			last = 0;
			for (unsigned int x = 0; x < Constants::CHUNK_SIZE_X; x++)
			{
				val = data[getIndex3D(x, y, z)];
				if (val != last) {
					if (last) {
						posDelta = getPosDelta(x - 1, y, z);
						for (unsigned int v : cube_right) {
							vertices.push_back(v + posDelta);
						}
					}
					else {
						posDelta = getPosDelta(x, y, z);
						for (unsigned int v : cube_left) {
							vertices.push_back(v + posDelta);
						}
					}
				}
				last = val;
			}
			if (val) {
				posDelta = getPosDelta(Constants::CHUNK_SIZE_X - 1, y, z);
				for (unsigned int v : cube_right) {
					vertices.push_back(v + posDelta);
				}
			}
		}
	}
}