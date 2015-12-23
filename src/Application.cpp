#include "Application.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]) : System(screenSize, title, argc, argv)
{
	float vertices[] = {		
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		   1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		  1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		   1.0f, -1.0f, -1.0f,
		   1.0f, 1.0f, -1.0f,
		   1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		   1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		   1.0f, -1.0f, 1.0f,
		   1.0f, 1.0f, 1.0f,
		   1.0f, -1.0f, -1.0f,
		   1.0f, 1.0f, -1.0f,
		   1.0f, -1.0f, -1.0f,
		   1.0f, 1.0f, 1.0f,
		   1.0f, -1.0f, 1.0f,
		   1.0f, 1.0f, 1.0f,
		   1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		   1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		   1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		    1.0f, -1.0f, 1.0f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	shader = new Shader("data/shaders/default");
	camera = new Camera(screenSize, 59.0f, 0.01f, 1000.0f);

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Application::~Application()
{
	delete shader;
	glDeleteBuffers(1, &vbo);
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
	camera->HandleEvent(event);
}

void Application::Update(float deltaTime)
{
	glm::mat4 m = glm::mat4();
	m = glm::rotate(m, glm::radians(SDL_GetTicks()*360/10000.0f), glm::vec3(0, 0, 1));

	shader->SetUniform("model", m);
	shader->SetUniform("projview", camera->GetProjectionView());
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->Use();

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 12*3);

	glDisableVertexAttribArray(0);
}