#pragma once

#include "System.h"
#include "Shader.h"

class Application: public System
{
private:
	Shader* shader;
	GLuint vbo;
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();
};

