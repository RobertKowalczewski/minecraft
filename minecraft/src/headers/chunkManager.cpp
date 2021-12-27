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
	/*int border = renderDistance / 2;

	std::queue<glm::ivec2> positions;
	glm::ivec2 startChunkPos = glm::ivec2((int)cameraPos.x % Constants::CHUNK_SIZE_X, (int)cameraPos.z % Constants::CHUNK_SIZE_Z);
	while (!positions.empty()) {
		glm::ivec2 t = positions.front();
		for (int x = -1; x <= 1; x += 2) {
			for (int z = -1; z <= 1; z += 2) {
				//TODO znaki rownoœci
				if (-border < t.x + x < border && -border < t.y + z < border) {
					positions.push(glm::ivec2(x, z));
				}
			}
		}
	}*/

	threads.push_back(std::thread(&ChunkManager::createChunksAsync, this));
	threads.push_back(std::thread(&ChunkManager::requestNewChunks, this));
}

void ChunkManager::createChunksAsync() {
	std::chrono::milliseconds time(20);
	while (true) {
		//if (chunksPos.size()) {
		//	std::for_each(
		//		std::execution::par_unseq,
		//		chunksPos.begin(),
		//		chunksPos.end(),
		//		[this](auto&& pos) {
		//			createChunkHeightMap(pos);
		//			//createChunk(pos);
		//		}
		//	);
		//	chunksPos.clear();
		//	while (!threadedChunks.empty_unsafe()) {
		//		Chunk temp = threadedChunks.top_unsafe();
		//		int index = temp.indexPos.x + temp.indexPos.y * Constants::RENDERDISTANCE;
		//		visibleChunks[index] = temp;
		//		chunksAwaiting.push(&visibleChunks[index]);
		//		threadedChunks.pop_unsafe();
		//	}
		//}

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
							createChunkHeightMap(pos);
							//createChunk(pos);
						}
					);
			/*for (auto& pos : temp) {
				createChunkHeightMap(pos);
			}*/
			while (!threadedChunks.empty()) {
				Chunk temp = threadedChunks.top();
				int index = temp.indexPos.x + temp.indexPos.y * Constants::RENDERDISTANCE;
				visibleChunks[index] = temp;
				chunksAwaitingGpu.push(&visibleChunks[index]);
				threadedChunks.pop();
			}
		}


		//real time chunk loading/unloading
		/*
		if ((int)camera->pos.x / Constants::CHUNK_SIZE_X - lastCamPos.x || (int)camera->pos.z / Constants::CHUNK_SIZE_Z - lastCamPos.y) {
			glm::ivec2 camPos = { (int)camera->pos.x / Constants::CHUNK_SIZE_X,(int)camera->pos.z / Constants::CHUNK_SIZE_Z };
			glm::ivec2 dPos = camPos - lastCamPos;
			if (dPos.x && dPos.y) {
				updateVisibleChunks(glm::ivec2(dPos.x,0), camPos);
				updateVisibleChunks(glm::ivec2(0, dPos.y), camPos);
			}
			else {
				updateVisibleChunks(dPos, camPos);
			}

			lastCamPos = camPos;
		}*/
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

void ChunkManager::fillVisibleChunks()
{
	/*glm::ivec2 camPos = { (int)camera->pos.x / Constants::CHUNK_SIZE_X,(int)camera->pos.z / Constants::CHUNK_SIZE_Z };
	int offset = Constants::RENDERDISTANCE / 2;
	for (int z = 0; z < Constants::RENDERDISTANCE; z++)
	{
		for (int x = 0; x < Constants::RENDERDISTANCE; x++) {
			visibleChunks[x + z * Constants::RENDERDISTANCE] = &loadedChunks[glm::ivec2(camPos.x + (x - offset), camPos.y + (z - offset))];
		}

	}*/
}


void ChunkManager::noiseSettings()
{
	noise = FastNoise::NewFromEncodedNodeTree("EwAAAIA/EwCPwnU+HAABGwAhACEAGwAWAB0BAAATAK5H4T4HAACPwvU9FgBc////EwAfhes/BwAAcT0KPxsAIQAWAKf///8TAD0KV0AIABYAhQAAABMA7FFIQQcAAAAAAD8APQrXPgAfhes+ARMACtcjPRYAGAEAAAcAAI/CdT0=");
	//noise = FastNoise::Perlin();
	//noise = FastNoise::NewFromEncodedNodeTree("FwAAAAAAAACAPwAAAAAAAIA/GwAhABwAARsAEwBvEoM6FgDnAAAACAABEwBvEoM6FgDPBAAACAAA7FG4PhMACtcjPBYAdzgAAAgAAM3MTD0AZmYGQA==");
	//noise = FastNoise::NewFromEncodedNodeTree("GwAXAArXoz7sUahAAAAAAM3MzD0bABMACtejvBYAKQAAAAgAARMAj8L1PRYAoQAAAP//AAABFwAAAIC/AACAPwAAAAAAAIA/BAAAAAAAAACAvwAAAAAAAAAAAAAAAAAAAAAAAAAACtcjPQ==");
}

void ChunkManager::createChunk(glm::ivec4 pos)
{

	//z -> y -> x
	/*std::vector<float> noiseOutput(Constants::BLOCK_COUNT);
	auto start = high_resolution_clock::now();
	noise->GenUniformGrid3D(noiseOutput.data(), pos.z, 0, pos.w, Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z, 0.2f, 10);
	std::cout << "noise: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << std::endl;*/

	bool safeSlices[Constants::CHUNK_SIZE_Y];
	std::fill_n(safeSlices, Constants::CHUNK_SIZE_Y, 1);
	auto start = high_resolution_clock::now();

	unsigned char data[Constants::BLOCK_COUNT];

	float frequency = 0.01;

	for (int x = 0; x < Constants::CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < Constants::CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < Constants::CHUNK_SIZE_Z; z++)
			{
				if (octave_noise_3d(4,.2, 1,(x + pos.z + 999999) * frequency, y * frequency, (Constants::CHUNK_SIZE_Z - 1 - z + pos.w + 999999) * frequency) > 0) {
					data[getIndex3D(x,y,z)] = 1;
				}
				else {
					data[getIndex3D(x, y, z)] = 0;
				}
			}
		}
	}
	std::cout << "noise: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << std::endl;


	/*for (int i = 0; i < Constants::BLOCK_COUNT;i++) {
		if (noise >= .5) {
			data[i] = 1;
		}
		else {
			data[i] = 0;
		}
	}*/

	/*for (int i = 0; i < data.size(); i++)
	{
		data[i] = 1;
	}*/

	//std::vector<int> data(Constants::BLOCK_COUNT);

	//for (float z = 0; z < Constants::CHUNK_SIZE_Z; z++) {
	//	for (float y = 0; y < Constants::CHUNK_SIZE_Y; y++) {
	//		for (float x = 0; x < Constants::CHUNK_SIZE_X; x++) {
	//			//std::cout << perlin.normalizedOctave3D_01((x + chunkPosX) * 0.01, y, (z + chunkPosZ) * 0.01, 1) << std::endl;
	//			if (perlin.normalizedOctave3D_01((x + chunkPosX) * 0.04, y * 0.04, ((Constants::CHUNK_SIZE_Z - 1 -z) + chunkPosZ) * 0.04, 1) > .7) {
	//				data[getIndex3D((int)x, (int)y, (int)z)] = 1;
	//			}
	//			else {
	//				data[getIndex3D((int)x, (int)y, (int)z)] = 0;
	//			}

	//		}
	//	}
	//}
	/*chunks.push_back(Chunk(data, glm::vec3(chunkPosX, 0, chunkPosZ)));
	chunksAwaiting.push(&chunks.back());*/



	threadedChunks.push(Chunk(data, glm::vec3(pos.z, 0, pos.w), glm::vec2(pos.x, pos.y), noise, seed, safeSlices));


}

void ChunkManager::createChunkHeightMap(glm::ivec4 pos) {
	auto start = high_resolution_clock::now();

	float noiseOutput[Constants::CHUNK_SIZE_X * Constants::CHUNK_SIZE_Z];
	noise->GenUniformGrid2D(noiseOutput, pos.z, pos.w, Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Z, 0.04f, 10);

	bool safeSlices[Constants::CHUNK_SIZE_Y];
	std::fill_n(safeSlices, Constants::CHUNK_SIZE_Y, 1);
	
	unsigned char data[Constants::BLOCK_COUNT];
	int borderY;
	for (int z = 0; z < Constants::CHUNK_SIZE_Z; z++)
	{
		for (int x = 0; x < Constants::CHUNK_SIZE_X; x++)
		{
			borderY = noiseOutput[x + (Constants::CHUNK_SIZE_Z - 1 - z) * Constants::CHUNK_SIZE_X] * Constants::CHUNK_SIZE_Y;
			for (int y = 0; y < Constants::CHUNK_SIZE_Y; y++) {
				if (y < borderY) {
					data[getIndex3D(x,y,z)] = 1;
				}
				else {
					data[getIndex3D(x, y, z)] = 0;
					safeSlices[y] = 0;
				}
			}
		}
	}
	std::cout << "noise: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << std::endl;


	threadedChunks.push(Chunk(data, glm::vec3(pos.z, 0, pos.w), glm::vec2(pos.x,pos.y), noise, seed, safeSlices));
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
//TODO
/*void ChunkManager::generateMesh()
{
	//TODO one big mesh
	mesh.clear();
	for (Chunk& chunk : chunks) {
		mesh.insert(mesh.end(), chunk.vertices.begin(), chunk.vertices.end());
	}
}*/
