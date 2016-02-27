#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("data/models/"),
	shaderManager("data/shaders/"),
	textureManager("data/textures/"),
	shader(shaderManager.GetAsset("default"))
{
	kult::add<Component::Position>(camera) = {
		glm::vec3(0, 0, 2)
	};
	kult::add<Component::Freelook>(camera) = {
		0.002f
	};

	kult::add<Component::Position>(cube) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(30, 30, 1)
	};
	kult::add<Component::Render>(cube) = {
		meshManager.GetAsset("cube.obj"),
		textureManager.GetAsset("wood.raw"),
		true
	};

	kult::add<Component::Position>(tree) = {
		glm::vec3(10, 0, 0),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::Render>(tree) = {
		meshManager.GetAsset("tree.obj"),
		textureManager.GetAsset("missing.raw"),
		false
	};
	kult::add<Component::Physics>(tree) = {
		glm::vec3(0, 0, 0),
		glm::zero<glm::vec3>(),
		glm::vec3(0, 0, 1)
	};

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Application::~Application()
{
	cube.purge();
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
	Systems::HandleFreelookEvent(event);
}

void Application::Update(float deltaTime)
{
	Systems::Physics(deltaTime);
}

void Application::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
}