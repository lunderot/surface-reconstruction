#pragma once

#include "System.h"
#include "AssetManager/Shader.h"
#include "AssetManager/AssetManager.h"
#include "AssetManager/Mesh.h"
#include "AssetManager/Texture.h"
#include "AssetManager/Config.h"
#include "AssetManager/ParticleList.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <kult.hpp>

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"

#include "VertexGrid.h"

class Application: public System
{
private:
	//Asset managers
	AssetManager::AssetManager<AssetManager::Mesh> meshManager;
	AssetManager::AssetManager<AssetManager::Shader> shaderManager;
	AssetManager::AssetManager<AssetManager::Texture> textureManager;
	AssetManager::AssetManager<AssetManager::Config> configManager;
	AssetManager::AssetManager<AssetManager::ParticleList> particleManager;

	//Entities
	kult::entity camera;
	kult::entity particleCloud;
	kult::entity vertexParticlesEntity;
	kult::entity vertexRelationEntity;
	kult::entity cubeEntity;

	VertexGrid vertexGrid;
	AssetManager::ParticleList* vertexGridParticles;
	AssetManager::ParticleList* vertexRelationLines;

	//GUI varaiables
	bool showGui;
	bool showInfoBox;
	bool showVertexRelation;
	glm::ivec3 selectedVertex;
	glm::vec3 clearColor;

private:
	void CreateParticleCloud(AssetManager::ParticleList* particleList);
	void CreateVertexGrid(AssetManager::ParticleList* particleList);
	void RenderGUI();

public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);

	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();
};

