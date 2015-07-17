#include "Application.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]) : System(screenSize, title, argc, argv)
{
	
}


Application::~Application()
{
	
}

void Application::HandleEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		std::cout << "Mouse button down" << std::endl;
		break;
	default:
		break;
	}
}

void Application::Update(float deltaTime)
{
	
}

void Application::Render()
{
	
}