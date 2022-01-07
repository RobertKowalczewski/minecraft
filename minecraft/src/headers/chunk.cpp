#include "chunk.h"

using namespace std::chrono;

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
Chunk::Chunk(std::vector<uint8_t>& data, glm::vec3 pos, glm::ivec2 indexPos)
	:Transform(pos)
{

	this->indexPos = indexPos;
	localMat = getLocalMat();
	meshFromData(data);
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
void Chunk::findNeighboursZ(unsigned int& textureID,unsigned int& x, unsigned int& y, unsigned int& z, std::vector<uint8_t>& data) {
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
void Chunk::findNeighboursY(unsigned int& textureID, unsigned int& x, unsigned int& y, unsigned int& z, std::vector<uint8_t>& data) {
	if (y < Constants::CHUNK_SIZE_Y - 1 && !data[getIndex3DNoise(x, y+1, z)]) {
		vertices.push_back(Vertex{  x, 1 + y, z, 0, 0, 3, textureID});
		vertices.push_back(Vertex{	x, 1 + y, 1 + z, 0, 1, 3, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 1, 1, 3, textureID});

		vertices.push_back(Vertex{	x, 1 + y, z, 0, 0, 3, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, 1 + z, 1, 1, 3, textureID});
		vertices.push_back(Vertex{	1 + x, 1 + y, z, 1, 0, 3, textureID});
	}
	if (!y || !data[getIndex3DNoise(x, y-1, z)]) {
		vertices.push_back(Vertex{  1 + x, y, 1 + z, 0, 0, 1, textureID});
		vertices.push_back(Vertex{	x, y, 1 + z, 0, 1, 1, textureID});
		vertices.push_back(Vertex{	x, y, z, 1, 1, 1, textureID});

		vertices.push_back(Vertex{	1 + x, y, 1 + z, 0, 0, 1, textureID});
		vertices.push_back(Vertex{	x, y, z, 1, 1, 1, textureID});
		vertices.push_back(Vertex{	1 + x, y, z, 1, 0, 1, textureID});
	}
}
void Chunk::findNeighboursX(unsigned int& textureID, unsigned int& x, unsigned int& y, unsigned int& z, std::vector<uint8_t>& data) {
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

void Chunk::meshFromData(std::vector<uint8_t>& data) {
	unsigned int blockType;
	for (unsigned int z = 1u; z < Constants::CHUNK_SIZE_Z + 1u; z++)
	{
		for (unsigned int y = 0u; y < Constants::CHUNK_SIZE_Y; y++)
		{
			for (unsigned int x = 1u; x < Constants::CHUNK_SIZE_X + 1u; x++)
			{
				if (blockType = data[getIndex3DNoise(x,y,z)]) {
					findNeighboursX(blockType,x, y, z, data);
					findNeighboursY(blockType,x, y, z, data);
					findNeighboursZ(blockType,x, y, z, data);
				}
			}
		}
	}
}