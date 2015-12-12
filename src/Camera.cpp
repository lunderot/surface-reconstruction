#include "Camera.h"


Camera::Camera(const glm::uvec2& screenSize, glm::f32 fovy, glm::f32 near, glm::f32 far)
{
	position = glm::vec3(0, 1, 1);
	lookDirection = glm::vec3(0, -1, -1);
	mouseChange = glm::vec2(0.0f, 100.0f);
	view = glm::lookAt(position, position + lookDirection, glm::vec3(0.0f, 0.0f, 1.0f));
	projection = glm::perspective(glm::radians(fovy), (float)screenSize.x / screenSize.y, near, far);
	sensitivity = 0.002f;
}


Camera::~Camera()
{
}

glm::mat4 Camera::GetProjectionView() const
{
	return projection * view;
}

void Camera::HandleEvent(SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
	{
		
		mouseChange.x += e.motion.xrel * sensitivity;
		mouseChange.y += e.motion.yrel * sensitivity;

		if (mouseChange.y < glm::radians(90.0f))
		{
			mouseChange.y = glm::radians(90.0f);
		}

		if (mouseChange.y > glm::radians(270.0f))
		{
			mouseChange.y = glm::radians(270.0f);
		}

		lookDirection = glm::vec3(cos(mouseChange.y) * sin(mouseChange.x), cos(mouseChange.y) * cos(mouseChange.x), sin(mouseChange.y));

		glm::vec3 right = glm::vec3(sin(mouseChange.x - 3.14f / 2.0f), cos(mouseChange.x - 3.14f / 2.0f), 0);

		glm::vec3 up = glm::cross(right, lookDirection);

		view = glm::lookAt(position, position + lookDirection, up);
		break;
	}
	default:
		break;
	}
}
