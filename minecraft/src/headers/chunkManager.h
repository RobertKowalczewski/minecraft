#pragma once
#include<thread>
#include <FastNoise/FastNoise.h>
#include <perlinNoise.h>
#include"stack.h"
#include"camera.h"
#include"functions.h"


class ChunkManager {
	const unsigned int seed = 1u;
	const siv::BasicPerlinNoise<float> perlin{ seed };
	glm::ivec2 lastCamPos;
	//FastNoise::SmartNode<FastNoise::Perlin> noise = FastNoise::New<FastNoise::Perlin>();
	Stack<Chunk> threadedChunks;
public:
	FastNoise::SmartNode<> noise;
	Camera* camera;
	std::vector<std::thread> threads;
	Stack<glm::ivec4> chunksPos;
	unsigned int modelLoc;
	std::vector<Chunk> visibleChunks;
	std::queue<Chunk*> chunksAwaitingGpu;
	ChunkManager(unsigned int modelLoc, unsigned int shaderID, Camera* camera);
	void createChunksAsync();
	void noiseSettings();
	void createChunk(glm::ivec4 pos);
	void createChunkHeightMap(glm::ivec4 pos);
	void renderChunks(Shader& shader);
	void updateVisibleChunks(glm::ivec2 dPos, glm::ivec2 camPos);
private:
	void fillVisibleChunks();
	void requestNewChunks();
};