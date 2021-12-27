#pragma once

class Camera:public Transform {
	class Mouse {
	public:
		float sensitivity;
		double x;
		double y;
		Mouse();
		void update(double x, double y);
	};
public:
	float speed;
	float zoom;
	Mouse mouse;
	glm::mat4 projection;
	Camera(glm::vec3 pos, float speed, float sensitivity, float zoom, float fovy, float aspect, float near, float far);
	void keyboardInput(const char* direction, float dt);
	void mouseInput(float xoffset, float yoffset, bool constrainPitch = true);
};