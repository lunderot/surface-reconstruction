#pragma once

#include <iostream>
#include <exception>
#include <algorithm>

#include <gl\glew.h>
#include <SDL.h>
#include <glm\glm.hpp>

class System
{
protected:
	glm::uvec2 screenSize;
	bool running;

	SDL_Window* window;
	SDL_GLContext context;

	SDL_Event event;
	
public:
	System(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~System();

	void Run();
	glm::uvec2 GetScreenSize() const;

	virtual void HandleEvent(SDL_Event& event) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};

