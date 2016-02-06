#include "Application.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	camera(screenSize, 59.0f, 0.01f, 1000.0f),
	meshManager("data/models/"),
	shaderManager("data/shaders/")
{
	tree = meshManager.GetAsset("tree.obj");
	shader = shaderManager.GetAsset("default");

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Application::~Application()
{
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
	glm::mat4 m = glm::mat4();
	m = glm::rotate(m, glm::radians(SDL_GetTicks()*360/10000.0f), glm::vec3(0, 0, 1));

	shader->SetUniform("model", m);
	shader->SetUniform("projview", camera.GetProjectionView());
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Use();
	
	glBindVertexArray(tree->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, tree->GetVertexCount());
}