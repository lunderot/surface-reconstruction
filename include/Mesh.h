#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};
private:
	GLuint vbo;
	GLuint vao;
	glm::u32 vertexCount;

	void LoadMesh(const std::string& filepath, std::vector<Vertex>& out);

public:
	Mesh(const std::string& filepath);
	~Mesh();
	GLuint GetVBO() const;
	GLuint GetVAO() const;
	glm::u32 GetVertexCount() const;
};

