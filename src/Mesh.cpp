#include "Mesh.h"

Mesh::Mesh(const std::string& filepath)
{
	std::vector<Vertex> out;
	LoadMesh(filepath, out);

	//Generate vertex buffer and vertex array object
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	//Bind the buffers
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Load the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, out.size() * sizeof(Vertex), out.data(), GL_STATIC_DRAW);

	//Enable the shader attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
	this->vertexCount = out.size();
}


Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::LoadMesh(const std::string& filepath, std::vector<Vertex>& out)
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

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		Vertex vertex;
		vertex.position = vertices[vertexIndices[i] - 1];
		vertex.normal = normals[normalIndices[i] - 1];
		vertex.uv = uvs[uvIndices[i] - 1];
		out.push_back(vertex);
	}
}

GLuint Mesh::GetVBO() const
{
	return vbo;
}

GLuint Mesh::GetVAO() const
{
	return vao;
}

glm::u32 Mesh::GetVertexCount() const
{
	return vertexCount;
}
