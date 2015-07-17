#include "Application.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]) : System(screenSize, title, argc, argv)
{
	float vertices[] = {		
		+0.5f, +0.5f, 1.0f,
		-0.5f, +0.5f, 0.0f,
		+0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 1.0f
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	shader = new Shader("data/shaders/default");
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
		std::cout << "Mouse button down" << std::endl;
		break;
	default:
		break;
	}
}

void Application::Update(float deltaTime)
{
	
}

void Application::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->Use();

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
}