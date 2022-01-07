#pragma once
#include <GL/glew.h>
#include"shader.h"
#include "constants.h"
#include<vector>
#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<chunk.h>
#include<camera.h>

class Debugger {
public:
	bool DEBUG = false;
	bool chunkOutlines = false;

	Debugger(bool debug, bool chunkOutlines, glm::mat4 projection, unsigned int externalShaderID, Camera* camera);
	~Debugger();
	void doStuff(std::vector<Chunk>& chunks);
private:
	Camera* camera;
	unsigned int externalShaderID;
	unsigned int modelLoc;
	unsigned int viewLoc;
	unsigned int vao = 0;
	Shader shaderD = Shader("res/debugShader/vertexD.glsl","res/debugShader/fragmentD.glsl");

	std::vector<unsigned int> outlineTris = {
		////forward
		//Constants::CHUNK_SIZE_X, 0, Constants::CHUNK_SIZE_Z,
		//Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y,Constants::CHUNK_SIZE_Z,
		//0, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z,
		//Constants::CHUNK_SIZE_X, 0, Constants::CHUNK_SIZE_Z,
		//0, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z,
		//0, 0, Constants::CHUNK_SIZE_Z,
		////behind
		//0, 0, 0 ,
		//0, Constants::CHUNK_SIZE_Y, 0 ,
		//Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y, 0 ,
		//0, 0, 0 ,
		//Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y, 0 ,
		//Constants::CHUNK_SIZE_X, 0, 0 ,
		////left
		//0, 0, 1,
		//0, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z,
		//0, Constants::CHUNK_SIZE_Y, 0,
		//0, 0, Constants::CHUNK_SIZE_Z,
		//0, Constants::CHUNK_SIZE_Y, 0,
		//0, 0, 0,
		////right
		//Constants::CHUNK_SIZE_X, 0, 0,
		//Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y, 0,
		//Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z,
		//Constants::CHUNK_SIZE_X, 0, 0 ,
		//Constants::CHUNK_SIZE_X, Constants::CHUNK_SIZE_Y, Constants::CHUNK_SIZE_Z,
		//Constants::CHUNK_SIZE_X, 0, 1
		0,0,Constants::CHUNK_SIZE_Z,
		Constants::CHUNK_SIZE_X,0,Constants::CHUNK_SIZE_Z,
		/*1,1,1,
		0,1,1,
		0,0,1*/
	};

	void drawChunkOutlines(std::vector<Chunk>& chunks);
};