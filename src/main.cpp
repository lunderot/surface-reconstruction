#include "Application.h"

int main(int argc, char* argv[])
{
	try
	{
		Application app(glm::uvec2(1280, 720), "OpenGLFPS", argc, argv);
		app.Run();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "SDL error: " << SDL_GetError() << std::endl;
		return 1;
	}
	return 0;
}
