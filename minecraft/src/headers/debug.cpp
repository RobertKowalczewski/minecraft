#include "debug.h"

void checkGLError();

void Debugger::doStuff(std::vector<Chunk>& chunks)
{
	if (!DEBUG) return;

	if (chunkOutlines)
		drawChunkOutlines(chunks);
}
Debugger::~Debugger() {
}

void Debugger::drawChunkOutlines(std::vector<Chunk>& chunks)
{
	shaderD.use();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMat()));
	glBindVertexArray(vao);
	for (Chunk& chunk : chunks) {
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(chunk.localMat));
		glDrawArrays(GL_LINES, 0, outlineTris.size());
	}
	glUseProgram(externalShaderID);
}

Debugger::Debugger(bool debug, bool chunkOutlines, glm::mat4 projection, unsigned int externalShaderID, Camera* camera)
{
	this->camera = camera;
	this->externalShaderID = externalShaderID;
	this->DEBUG = debug;
	this->chunkOutlines = chunkOutlines;

	shaderD.use();
	glUniformMatrix4fv(glGetUniformLocation(shaderD.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	this->modelLoc = glGetUniformLocation(shaderD.ID, "model");
	this->viewLoc = glGetUniformLocation(shaderD.ID, "view");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, outlineTris.size() * sizeof(unsigned int), &outlineTris[0], GL_STATIC_DRAW);

	glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, sizeof(unsigned int) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(externalShaderID);
}
