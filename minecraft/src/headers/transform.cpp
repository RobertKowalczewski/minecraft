#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "transform.h"



Transform::Transform(glm::vec3 pos, glm::vec3 forward,
	glm::vec3 up, glm::vec3 right, float pitch, float yaw) {
	this->up = up;
	this->pos = pos;
	this->forward = forward;
	this->right = right;
	this->pitch = pitch;
	this->yaw = yaw;
	if (pitch!=0.0f || yaw!=0.0f) {
		updateDirections();
	}
}

//check
glm::mat4 Transform::getLocalMat() {
	return glm::mat4(glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(pos, 1.0f));
}
glm::mat4 Transform::getViewMat() {
	return glm::lookAt(pos, pos + forward, up);
}
void Transform::move(glm::vec3 d) {
	pos += d;
}
void Transform::rotate(float dx, float dy) {
	pitch += dx;
	yaw += dy;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateDirections();
}
void Transform::updateDirections() {
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, glm::vec3(0.f, 1.f, 0.f)));
	up = glm::normalize(glm::cross(right, forward));
}