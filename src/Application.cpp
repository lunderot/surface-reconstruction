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
	particleManager("data/particleSets/")
{

	ImGui_ImplSdlGL3_Init(window);

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
		particleManager.Get("1.bin")
	};

	AssetManager::ParticleList* particleList = particleManager.Get("1.bin");

	vertexGrid = VertexGrid(particleList->GetMin(), particleList->GetMax(), particleList->GetParticles()->at(0).radius);

	std::vector<AssetManager::ParticleList::Particle> particles;
	std::vector<VertexGrid::Vertex>* vertices = vertexGrid.GetVertices();

	for (int i = 0; i < vertices->size(); i++)
	{
		particles.push_back({ vertices->at(i).position, 0.038f});
	}

	kult::add<Component::Position>(vertexParticles) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(10, 10, 10)
	};
	kult::add<Component::PointRender>(vertexParticles) =
	{
		new AssetManager::ParticleList(&particles)
	};

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

Application::~Application()
{
	ImGui_ImplSdlGL3_Shutdown();
	camera.purge();
	cube.purge();
	particleCloud.purge();
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
	glm::f32 fov = configManager.Get("camera/fFov")->Get<glm::f32>();
	glm::f32 near = configManager.Get("camera/fNear")->Get<glm::f32>();
	glm::f32 far = configManager.Get("camera/fFar")->Get<glm::f32>();
	glm::uvec2 screenSize = GetScreenSize();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shaderManager.Get("default.shader"), camera, screenSize, fov, near, far);
	Systems::PointRender(shaderManager.Get("pointRender.shader"), camera, screenSize, fov, near, far);

	//DrawGUI();
}

void Application::DrawGUI()
{
	ImGui_ImplSdlGL3_NewFrame(window);
	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
	ImGui::Render();
}
