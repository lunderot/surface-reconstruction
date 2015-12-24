#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

class Mesh
{
private:
	GLuint vbo;
	glm::u32 vertexCount;
public:
	Mesh(const std::string& filepath);
	~Mesh();
	GLuint GetVBO() const;
	glm::u32 GetVertexCount() const;
};

