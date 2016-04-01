#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"

#include <glm/gtc/random.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("data/models/"),
	shaderManager("data/shaders/"),
	textureManager("data/textures/"),
	configManager("data/config/"),
	particleManager("data/particleSets/"),
	shader(shaderManager.Get("default.shader"))
{
	kult::add<Component::Position>(camera) = {
		glm::vec3(0, 0, 2)
	};
	kult::add<Component::Freelook>(camera) = {
		configManager.Get("camera/fSensitivity")->Get<glm::f32>()
	};
	kult::add<Component::Physics>(camera);
	kult::add<Component::Freemove>(camera) = {
		configManager.Get("camera/fSpeed")->Get<glm::f32>()
	};

	kult::add<Component::Position>(cube) = {
		glm::vec3(0, 0, 2),
		glm::quat(),
		glm::vec3(30, 30, 1)
	};
	kult::add<Component::Render>(cube) = {
		meshManager.Get("cube.obj"),
		textureManager.Get("wood.raw"),
		true
	};

	auto pl = particleManager.Get("2.bin")->GetParticles();

	for (auto i: *pl)
	{
		kult::entity e;

		kult::add<Component::Position>(e) = {
			i * 10.0f,
			glm::quat(),
			glm::vec3(1, 1, 1) * 0.01f
		};
		kult::add<Component::Render>(e) = {
			meshManager.Get("cube.obj"),
			textureManager.Get("wood.raw"),
			false
		};
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Application::~Application()
{
	camera.purge();
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
	Systems::UpdateFreemove();
	Systems::Physics(deltaTime);
}

void Application::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
}