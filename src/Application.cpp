#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"
#include "systems/PointRender.h"

#include <glm/gtc/random.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("data/models/"),
	shaderManager("data/shaders/"),
	textureManager("data/textures/"),
	configManager("data/config/"),
	particleManager("data/particleSets/"),
	showGui(false),
	showInfoBox(true),
	clearColor(0.2f, 0.2f, 0.2f)
{

	ImGui_ImplSdlGL3_Init(window);
	//Set the window alpha
	ImGui::GetStyle().WindowFillAlphaDefault = 0.9f;

	//Camera entity
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

	//Floor with wood texture
	kult::add<Component::Position>(cube) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(10, 10, .01)
	};
	kult::add<Component::Render>(cube) = {
		meshManager.Get("cube.obj"),
		textureManager.Get("white.raw"),
		true
	};

	//Particle cloud
	kult::add<Component::Position>(particleCloud) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(10, 10, 10)
	};
	kult::add<Component::PointRender>(particleCloud) =
	{
		true,
		particleManager.Get("1.bin")
	};
	AssetManager::ParticleList* particleList = particleManager.Get("1.bin");

	vertexGrid = VertexGrid(particleList->GetMin(), particleList->GetMax(), 0.038f);

	std::vector<AssetManager::ParticleList::Particle> particles;
	std::vector<VertexGrid::Vertex>* vertices = vertexGrid.GetVertices();

	for (int i = 0; i < vertices->size(); i++)
	{
		particles.push_back({ vertices->at(i).position, glm::f32(2.0f), glm::f32(0.1f) });
	}
	vertexGridParticles = new AssetManager::ParticleList(&particles);

	kult::add<Component::Position>(vertexParticlesEntity) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(10, 10, 10)
	};
	kult::add<Component::PointRender>(vertexParticlesEntity) =
	{
		true,
		vertexGridParticles
	};
}
Application::~Application()
{
	ImGui_ImplSdlGL3_Shutdown();
	camera.purge();
	cube.purge();
	particleCloud.purge();
	vertexParticlesEntity.purge();
	delete vertexGridParticles;
}

void Application::HandleEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYUP:
	{
		if (event.key.keysym.sym == SDLK_F1)
		{
			showGui = !showGui;
		}
		break;
	}
	default:
		break;
	}
	if (!showGui)
	{
		Systems::HandleFreelookEvent(event);
	}
}

void Application::Update(float deltaTime)
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(!showGui));
	if (!showGui)
	{
		Systems::UpdateFreemove();
	}
	Systems::Physics(deltaTime);
}

void Application::Render()
{
	glm::f32 fov = configManager.Get("camera/fFov")->Get<glm::f32>();
	glm::f32 near = configManager.Get("camera/fNear")->Get<glm::f32>();
	glm::f32 far = configManager.Get("camera/fFar")->Get<glm::f32>();
	glm::uvec2 screenSize = GetScreenSize();

	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Systems::Render(shaderManager.Get("default.shader"), camera, screenSize, fov, near, far);
	Systems::PointRender(shaderManager.Get("pointRender.shader"), camera, screenSize, fov, near, far);

	RenderGUI();
}

void Application::RenderGUI()
{
	ImGui_ImplSdlGL3_NewFrame(window);
	if (showGui)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 660));
		ImGui::SetNextWindowPos(ImVec2(10, 50));
		ImGui::Begin("Settings", &showGui, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
			ImGui::ColorEdit3("clear color", glm::value_ptr(clearColor));
			ImGui::Checkbox("Particle cloud:", &kult::get<Component::PointRender>(particleCloud).renderThis);
			ImGui::Checkbox("Vertex grid particles:", &kult::get<Component::PointRender>(vertexParticlesEntity).renderThis);
		ImGui::End();
	}

	if (showInfoBox)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 20));
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::Begin("Info", &showInfoBox, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	ImGui::Render();
}
