#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	camera(screenSize, 59.0f, 0.01f, 1000.0f),
	meshManager("data/models/"),
	shaderManager("data/shaders/"),
	textureManager("data/textures/")
{
	shader = shaderManager.GetAsset("default");

	kult::add<Component::Position>(player) = { glm::vec3(4, 0, 0), glm::vec3(0, 0, 0) };
	kult::add<Component::Render>(player) = { meshManager.GetAsset("capsule.obj"), textureManager.GetAsset("capsule.raw") };
	

	kult::add<Component::Position>(tree) = { glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) };
	kult::add<Component::Render>(tree) = { meshManager.GetAsset("tree.obj"), textureManager.GetAsset("missing2.raw") };
	kult::add<Component::Physics>(tree) = { glm::vec3(-10, 0, 0), glm::vec3(10, 0, 0) };


	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Application::~Application()
{
	player.purge();
	tree.purge();
}

void Application::HandleEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		std::cout << "Mouse button down" << std::endl;
		break;
	}
	default:
		break;
	}
	camera.HandleEvent(event);
}

void Application::Update(float deltaTime)
{
	Systems::Physics(deltaTime);
	
}

void Application::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Systems::Render(shader, &camera);
}