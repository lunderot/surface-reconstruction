#include "Mesh.h"

Mesh::Mesh(const std::string& filepath)
{
	std::vector<glm::u32> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::ifstream file(filepath);
	if (!file)
	{
		throw std::runtime_error("Failed to load file: " + filepath);
	}

	while (!file.eof())
	{
		std::string command;
		file >> command;
		if (command == "v")
		{
			glm::vec3 vertex;
			file >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (command == "vt")
		{
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			uvs.push_back(uv);
		}
		else if (command == "vn")
		{
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (command == "f")
		{
			glm::u32 vertexIndex[3], uvIndex[3], normalIndex[3];
			char padding;
			for (int i = 0; i < 3; i++)
			{
				file >> vertexIndex[i] >> padding >> uvIndex[i] >> padding >> normalIndex[i];

				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
		}
		else //Unknown command
		{
			std::getline(file, command);
		}
	}

	std::vector<glm::vec3> out;

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = vertices[vertexIndex - 1];
		out.push_back(vertex);
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, out.size() * sizeof(glm::vec3), out.data(), GL_STATIC_DRAW);

	this->vertexCount = out.size() * sizeof(glm::vec3);
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
}

GLuint Mesh::GetVBO() const
{
	return vbo;
}

glm::u32 Mesh::GetVertexCount() const
{
	return vertexCount;
}
