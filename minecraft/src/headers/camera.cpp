#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.h"
#include"camera.h"


Camera::Camera(glm::vec3 pos, float speed, float sensitivity, float zoom, float fovy, float aspect, float near, float far)
	:Transform(pos, { 0,0,-1 }, { 0,1,0 }, { 1,0,0 }, 0.0f, -90.0f)
{
	this->speed = speed;
	this->zoom = zoom;
	this->projection = glm::perspective(glm::radians(fovy), aspect, near, far);
	mouse.sensitivity = sensitivity;
}
void Camera::keyboardInput(const char* direction, float dt) {
	float velocity = speed * dt;
	if (direction == "f")
		pos += glm::normalize(glm::vec3(forward.x,0,forward.z)) * velocity;
	if (direction == "b")
		pos -= glm::normalize(glm::vec3(forward.x, 0, forward.z)) * velocity;
	if (direction == "l")
		pos -= glm::normalize(glm::vec3(right.x, 0, right.z)) * velocity;
	if (direction == "r")
		pos += glm::normalize(glm::vec3(right.x, 0, right.z)) * velocity;
	if (direction == "u")
		pos += glm::vec3(0, 1, 0) * velocity;
	if (direction == "d")
		pos -= glm::vec3(0, 1, 0) * velocity;
}
void Camera::mouseInput(float newx, float newy, bool constrainPitch) {

	float xoffset = newx - mouse.x;
	float yoffset = newy - mouse.y;

	xoffset *= mouse.sensitivity;
	yoffset *= mouse.sensitivity;

	yaw += xoffset;
	pitch -= yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	mouse.update(newx, newy);

	// update Front, Right and Up Vectors using the updated Euler angles
	updateDirections();
}


Camera::Mouse::Mouse() {}
void Camera::Mouse::update(double x, double y) {
	this->x = x;
	this->y = y;
}