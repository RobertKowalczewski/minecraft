#pragma once


class Transform {
public:
	float pitch, yaw;
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;

	Transform(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f), glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f),
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), glm::vec3 right = glm::vec3(1.f, 0.f, 0.f), float pitch = 0.f, float yaw = 0.f);

	glm::mat4 getViewMat();
	glm::mat4 getLocalMat();
	void move(glm::vec3 d);
	void rotate(float dx, float dy);
	void updateDirections();
};