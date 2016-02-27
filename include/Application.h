#pragma once

#include "System.h"
#include "Shader.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <kult.hpp>

class Application: public System
{
private:
	AssetManager<Mesh> meshManager;
	AssetManager<Shader> shaderManager;
	AssetManager<Texture> textureManager;

	Shader* shader;

	kult::entity camera;
	kult::entity cube;
	kult::entity tree;
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();
};

