#pragma once

#include "System.h"
#include "Shader.h"
#include "Camera.h"
#include "AssetManager.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Application: public System
{
private:
	Shader* shader;
	Camera* camera;
	AssetManager<Mesh>* meshManager;
	AssetManager<Shader>* shaderManager;

	Mesh* tree;
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();
};

