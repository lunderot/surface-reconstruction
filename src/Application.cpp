#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"
#include "systems/DebugRender.h"

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
	showVertexRelation(false),
	clearColor(0.2f, 0.2f, 0.2f),
	selectedVertex(0, 0, 0)
{

	ImGui_ImplSdlGL3_Init(window);
	//Set the window alpha
	ImGui::GetStyle().WindowFillAlphaDefault = 0.9f;

	//Camera entity
	kult::add<Component::Position>(camera) = {
		configManager.Get("camera/vPosition")->Get<glm::vec3>()
	};

	kult::add<Component::Freelook>(camera) = {
		configManager.Get("camera/fSensitivity")->Get<glm::f32>()
	};
	kult::add<Component::Physics>(camera);
	kult::add<Component::Freemove>(camera) = {
		configManager.Get("camera/fSpeed")->Get<glm::f32>()
	};

	//Particle cloud
	kult::add<Component::Position>(particleCloud) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::DebugRender>(particleCloud) =
	{
		true,
		false,
		particleManager.Get("1.bin"),
		glm::vec3(1, 0, 0)
	};
	AssetManager::ParticleList* particleList = particleManager.Get("1.bin");

	vertexGrid = VertexGrid(particleList->GetMin(), particleList->GetMax(), 0.04f, 0.038f);
	for (auto& particle : *particleList->GetParticles())
	{
		vertexGrid.AddParticleToGrid(&particle);
	}

	std::vector<AssetManager::ParticleList::Particle> particles;
	std::vector<VertexGrid::Vertex>* vertices = vertexGrid.GetVertices();

	for (auto i: *vertices)
	{
		particles.push_back({ i.position, glm::f32(2.0f), glm::f32(0.1f) });
	}
	vertexGridParticles = new AssetManager::ParticleList(&particles);

	kult::add<Component::Position>(vertexParticlesEntity) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::DebugRender>(vertexParticlesEntity) =
	{
		true,
		false,
		vertexGridParticles,
		glm::vec3(0, 0, 0)
	};

	VertexGrid::Vertex*  vertex = vertexGrid.GetVertex(selectedVertex);
	vertexRelationLines = new AssetManager::ParticleList(vertex->position, &vertex->particles);

	kult::add<Component::Position>(vertexRelationEntity) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::DebugRender>(vertexRelationEntity) = {
		true,
		true,
		vertexRelationLines,
		glm::vec3(0, 0, 1)
	};	

	glm::f32 scale = vertexGrid.GetVertexBoundingBoxSize();
	kult::add<Component::Position>(cubeEntity) = {
		vertexGrid.GetVertex({0,0,0})->position,
		glm::quat(),
		glm::vec3(scale, scale, scale)
	};
	kult::add<Component::Render>(cubeEntity) = {
		meshManager.Get("cube.obj"),
		nullptr,
		false,
		false,
		false,
		true,
		glm::vec3(0, 1, 0)
	};

}
Application::~Application()
{
	ImGui_ImplSdlGL3_Shutdown();
	camera.purge();
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
	Systems::DebugRender(shaderManager.Get("pointRender.shader"), camera, screenSize, fov, near, far);
	Systems::Render(shaderManager.Get("default.shader"), camera, screenSize, fov, near, far);

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
			ImGui::ColorEdit3("Clear color", glm::value_ptr(clearColor));
			ImGui::Checkbox("Show FPS", &showInfoBox);
			ImGui::Separator();
			ImGui::Checkbox("Particle cloud", &kult::get<Component::DebugRender>(particleCloud).renderThis);
			ImGui::Checkbox("Vertex grid particles", &kult::get<Component::DebugRender>(vertexParticlesEntity).renderThis);
			if (ImGui::Button("Reset camera position"))
			{
				kult::get<Component::Position>(camera).pos = glm::vec3(0, 0, 0);
			}
			ImGui::SliderFloat("Camera speed", &kult::get<Component::Freemove>(camera).speed, 0.01f, 10.0f);

			if (ImGui::Checkbox("Draw vertex relation", &showVertexRelation))
			{
				kult::get<Component::Render>(cubeEntity).visible = showVertexRelation;
			}

			if (showVertexRelation)
			{
				glm::uvec3 gs = vertexGrid.GetGridSize();
				glm::f32 sensitivity = 0.1f; //TODO: This should be config value
				
				bool changed = false;

				changed = ImGui::DragInt("X", &selectedVertex.x, sensitivity, 0, gs.x - 1);
				changed = ImGui::DragInt("Y", &selectedVertex.y, sensitivity, 0, gs.y - 1) || changed;
				changed = ImGui::DragInt("Z", &selectedVertex.z, sensitivity, 0, gs.z - 1) || changed;
				if (changed)
				{
					VertexGrid::Vertex* vertex = vertexGrid.GetVertex(selectedVertex);

					kult::get<Component::Position>(cubeEntity).pos = vertex->position;

					auto& debugRenderData = kult::get<Component::DebugRender>(vertexRelationEntity);
					delete debugRenderData.mesh;
					debugRenderData.mesh = new AssetManager::ParticleList(vertex->position, &vertex->particles);
				}
			}

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
