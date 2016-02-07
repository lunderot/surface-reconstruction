#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

class Texture
{
private:
	GLuint tex;

	void LoadTexture(const std::string& filepath, std::vector<char>& out, unsigned int& width, unsigned int& height);
public:
	Texture(const std::string& filepath);
	~Texture();

	GLuint GetTexture() const;
};

