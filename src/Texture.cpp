#include "Texture.h"

Texture::Texture(const std::string& filepath)
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	std::vector<char> out;
	unsigned int width, height;
	LoadTexture(filepath, out, width, height);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
	glDeleteTextures(1, &tex);
}

void Texture::LoadTexture(const std::string& filepath, std::vector<char>& out, unsigned int& width, unsigned int& height)
{
	std::ifstream file(filepath, std::ios_base::binary);
	if (file.good())
	{

		//Get the filesize in bytes
		file.seekg(0, file.end);
		unsigned int size = (unsigned int)file.tellg();
		file.seekg(0, file.beg);

		//Assume texture is always square and RGBA format
		width = height = (int)sqrt(size / 4);

		out.resize(size);
		//Read the whole file into the vector
		file.read((char*)&out[0], out.size());
	}
	else
	{
		throw std::runtime_error("Failed to load file: " + filepath);
	}
}

GLuint Texture::GetTexture() const
{
	return tex;
}
