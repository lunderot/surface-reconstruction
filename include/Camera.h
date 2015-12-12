#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <SDL.h>

#include <iostream>

class Camera
{
private:
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 position;
	glm::vec3 lookDirection;
	glm::vec2 mouseChange;
	glm::f32 sensitivity;
public:
	Camera(const glm::uvec2& screenSize, glm::f32 fovy, glm::f32 near, glm::f32 far);
	~Camera();

	glm::mat4 GetProjectionView() const;
	void HandleEvent(SDL_Event& e);
};

