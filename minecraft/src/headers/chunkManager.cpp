#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>
#include<unordered_map>
#include <FastNoise/FastNoise.h>
#include <perlinNoise.h>
#include<iostream>
#include<thread>
#include<queue>
#include<chrono>
#include<execution>
#include<mutex>
#include<algorithm>
#include <simplexnoise.h>

#include"block.h"
#include"stack.h"
#include"constants.h"
#include"functions.h"
#include"transform.h"
#include"chunk.h"
#include "shader.h"
#include "chunkManager.h"
#include"camera.h"

using namespace std::chrono;

ChunkManager::ChunkManager(unsigned int modelLoc, unsigned int shaderID, Camera* camera)
{
	this->camera = camera;
	this->lastCamPos = glm::ivec2{ (int)camera->pos.x / Constants::CHUNK_SIZE_X,(int)camera->pos.z / Constants::CHUNK_SIZE_Z };

	visibleChunks.resize(Constants::RENDERDISTANCE * Constants::RENDERDISTANCE);

	this->modelLoc = modelLoc;
	noiseSettings();

	for (int x = 0; x < Constants::RENDERDISTANCE; x++)
	{
		//it's technically z but whateva
		for (int y = 0; y < Constants::RENDERDISTANCE; y++)
		{
			chunksPos.push(glm::ivec4(x, y,
				(x - Constants::RENDERDISTANCE / 2) * Constants::CHUNK_SIZE_X, 
				(y - Constants::RENDERDISTANCE / 2) * Constants::CHUNK_SIZE_Z));

		}
	}

	threads.push_back(std::thread(&ChunkManager::createChunksAsync, this));

	//threads.push_back(std::thread(&ChunkManager::requestNewChunks, this));
}

void ChunkManager::createChunksAsync() {
	while (true) {
		if (!chunksPos.empty()) {
			std::vector<glm::ivec4> temp;
			while (!chunksPos.empty()) {
				temp.push_back(chunksPos.top());
				chunksPos.pop();
			}
			std::for_each(
						std::execution::par_unseq,
						temp.begin(),
						temp.end(),
						[this](auto&& pos) {
							//createChunkHeightMap(pos);
							createChunk(pos);
						}
					);
			while (!threadedChunks.empty()) {
				Chunk temp = threadedChunks.top();
				int index = temp.indexPos.x + temp.indexPos.y * Constants::RENDERDISTANCE;
				visibleChunks[index] = temp;
				chunksAwaitingGpu.push(&visibleChunks[index]);
				threadedChunks.pop();
			}
		}
	}
}

void ChunkManager::requestNewChunks()
{
	while (true) {
		if ((int)camera->pos.x / Constants::CHUNK_SIZE_X - lastCamPos.x || (int)camera->pos.z / Constants::CHUNK_SIZE_Z - lastCamPos.y) {
			glm::ivec2 camPos = { (int)camera->pos.x / Constants::CHUNK_SIZE_X,(int)camera->pos.z / Constants::CHUNK_SIZE_Z };
			glm::ivec2 dPos = camPos - lastCamPos;

			if (dPos.x && dPos.y) {
				updateVisibleChunks(glm::ivec2(dPos.x, 0), camPos);
				updateVisibleChunks(glm::ivec2(0, dPos.y), camPos);
			}
			else {
				updateVisibleChunks(dPos, camPos);
			}

			lastCamPos = camPos;
		}
	}
}

void ChunkManager::updateVisibleChunks(glm::ivec2 dPos, glm::ivec2 camPos) {

	std::vector<Chunk> shifted(Constants::RENDERDISTANCE * Constants::RENDERDISTANCE);
	glm::ivec2 originToCam = glm::ivec2(camPos.x * Constants::CHUNK_SIZE_X, camPos.y * Constants::CHUNK_SIZE_Z);


	if (dPos.x < Constants::RENDERDISTANCE && dPos.y < Constants::RENDERDISTANCE) {
		if (dPos.x > 0) {
			for (int y = 0; y < Constants::RENDERDISTANCE; y++)
			{
				for (int x = dPos.x; x < Constants::RENDERDISTANCE; x++)
				{
					shifted[(x - dPos.x) + y * Constants::RENDERDISTANCE] = visibleChunks[x + y * Constants::RENDERDISTANCE];
				}
			}
			for (int y = 0; y < Constants::RENDERDISTANCE; y++)
			{
				for (int x = Constants::RENDERDISTANCE - dPos.x; x < Constants::RENDERDISTANCE; x++) {
					chunksPos.push(glm::ivec4(
						x, y,
						(camPos.x + (x - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_X,
						(camPos.y + (y - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_Z
					));
				}
			}
			for (Chunk& c : visibleChunks) {
				c.Delete();
			}
		}
		else if (dPos.x < 0) {
			for (int y = 0; y < Constants::RENDERDISTANCE; y++)
			{
				for (int x = 0; x < Constants::RENDERDISTANCE + dPos.x; x++)
				{
					shifted[(x - dPos.x) + y * Constants::RENDERDISTANCE] = visibleChunks[x + y * Constants::RENDERDISTANCE];
				}
			}

			for (int y = 0; y < Constants::RENDERDISTANCE; y++)
			{
				for (int x = 0; x < -dPos.x; x++) {
					chunksPos.push(glm::ivec4(
						x, y,
						(camPos.x + (x - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_X,
						(camPos.y + (y - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_Z
					));
				}
			}
			for (Chunk& c : visibleChunks) {
				c.Delete();
			}
		}
		else if (dPos.y > 0) {
			for (int y = dPos.y; y < Constants::RENDERDISTANCE; y++)
			{
				for (int x = 0; x < Constants::RENDERDISTANCE; x++)
				{
					shifted[x + (y - dPos.y) * Constants::RENDERDISTANCE] = visibleChunks[x + y * Constants::RENDERDISTANCE];
				}
			}
			for (int y = Constants::RENDERDISTANCE - dPos.y; y < Constants::RENDERDISTANCE; y++)
			{
				for (int x = 0; x < Constants::RENDERDISTANCE; x++) {
					chunksPos.push(glm::ivec4(
						x, y,
						(camPos.x + (x - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_X,
						(camPos.y + (y - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_Z
					));
				}
			}
			for (Chunk& c : visibleChunks) {
				c.Delete();
			}
			visibleChunks = shifted;
			return;

		}
		else if (dPos.y < 0) {
			for (int y = 0; y < Constants::RENDERDISTANCE + dPos.y; y++)
			{
				for (int x = 0; x < Constants::RENDERDISTANCE; x++)
				{
					shifted[x + (y - dPos.y) * Constants::RENDERDISTANCE] = visibleChunks[x + y * Constants::RENDERDISTANCE];
				}
			}
			for (int y = 0; y < -dPos.y; y++)
			{
				for (int x = 0; x < Constants::RENDERDISTANCE; x++) {
					chunksPos.push(glm::ivec4(
						x, y,
						(camPos.x + (x - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_X,
						(camPos.y + (y - Constants::RENDERDISTANCE / 2)) * Constants::CHUNK_SIZE_Z
					));
				}
			}
			for (Chunk& c : visibleChunks) {
				c.Delete();
			}
		}
		visibleChunks = shifted;
	}
}


void ChunkManager::noiseSettings()
{
	//noise = FastNoise::NewFromEncodedNodeTree("EACamZk/HgAZAB0ABAAAAAAAKVyPPgAAAAAAAAAAAAAAAEjhqkAAAAAAAAAAAAEEAAAAAACPwnW+exSuPgAAAAAAAAAAhetxQAAAAAAAAAAAAR0AGABSuB6/AACAPxYAAgAAABMApHA9Pw0AAwAAAAAAIEAIAACamRk/AAAAgD8BEwAzMzM///8EAAEZAAQAAAAAAAAAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB//8IAADNzEw+");
	noise = FastNoise::NewFromEncodedNodeTree("HQAeAB4AGQAdAAQAAAAAAClcjz4AAAAAAAAAAAAAAAAzM9NAAAAAAAAAAAABBAAAAAAAj8J1vnsUrj4AAAAAAAAAAOF6DEEAAAAAAAAAAAEdABgAFK5HvwAAgD8WAAIAAAATAKRwPT8NAAMAAAAAACBACAAAZmamPwAAAIA/AAAAgD8BEAAUrkc/GQAEAAAAAAB7FC4/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARMA7FE4Pv//BAAApHA9PwAAAIC/AAAAgD8=");
}

void ChunkManager::createChunk(glm::ivec4 pos)
{
	std::vector<float> noiseOutput(Constants::CHUNK_SIZE_Y * Constants::NOISE_ARRAY_SIZE);
	noise->GenUniformGrid3D(noiseOutput.data(), pos.z - 1,-(int)Constants::CHUNK_SIZE_Y/2, pos.w - 1, Constants::CHUNK_SIZE_X + 2, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z + 2, 0.01f, 10);

	//float avg = (minMax.min + minMax.max) / 2;

	std::vector<uint8_t> data(Constants::CHUNK_SIZE_Y * Constants::NOISE_ARRAY_SIZE);
	for (int i = 0; i < Constants::CHUNK_SIZE_Y * Constants::NOISE_ARRAY_SIZE; i++){
		data[i] = noiseOutput[i] < 0 ? 1 : 0;
	}

	threadedChunks.push(Chunk(data, glm::vec3(pos.z, 0, pos.w), glm::vec2(pos.x, pos.y)));
}

void ChunkManager::createChunkHeightMap(glm::ivec4 pos) {
	float noiseOutput[Constants::NOISE_ARRAY_SIZE];
	noise->GenUniformGrid2D(noiseOutput, pos.z-1, pos.w-1, Constants::CHUNK_SIZE_X+2, Constants::CHUNK_SIZE_Z+2, 0.01f, 10);

	std::vector<uint8_t> data(Constants::CHUNK_SIZE_Y * Constants::NOISE_ARRAY_SIZE);
	int borderY;
	for (int z = 0; z < Constants::CHUNK_SIZE_Z+2; z++)
	{
		for (int x = 0; x < Constants::CHUNK_SIZE_X+2; x++)
		{
			borderY = noiseOutput[x + ((Constants::CHUNK_SIZE_Z + 1) - z) * (Constants::CHUNK_SIZE_X+2)] * Constants::CHUNK_SIZE_Y;
			for (int y = 0; y < Constants::CHUNK_SIZE_Y; y++) {
				if (y < borderY) {
					data[getIndex3DNoise(x, y, z)] = 1;
				}
				else {
					data[getIndex3DNoise(x, y, z)] = 0;
				}
			}
		}
	}


	threadedChunks.push(Chunk(data, glm::vec3(pos.z, 0, pos.w), glm::vec2(pos.x,pos.y)));
}

void ChunkManager::renderChunks(Shader& shader)
{
	for (Chunk& chunk : visibleChunks) {
		if (!chunk.empty) {
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(chunk.localMat));
			glBindVertexArray(chunk.vao);
			glDrawArrays(GL_TRIANGLES, 0, chunk.vertices.size());
		}
	}

}
