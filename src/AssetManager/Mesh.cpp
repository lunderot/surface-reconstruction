#include "AssetManager/Mesh.h"

#include <iostream>

namespace AssetManager
{
	Mesh::Mesh(VertexGrid* vertexGrid) //Generate a mesh from poits using cube marching
	{
		glm::uvec3 gridSize = vertexGrid->GetGridSize();
		glm::f32 granularity = vertexGrid->GetGranularity();

		std::vector<Vertex> out;

		for (int z = 0; z < gridSize.z - 1; z++)
		{
			for (int y = 0; y < gridSize.y - 1; y++)
			{
				for (int x = 0; x < gridSize.x - 1; x++)
				{
					unsigned char data = 0;
					glm::f32 threshold = 0.01f;

					//If the scalar value for a specific node is over threshold, add it to the marching cube data
					data |= vertexGrid->GetVertex({ x + 0, y + 0, z + 0 })->scalarValue > threshold	? 1   : 0;
					data |= vertexGrid->GetVertex({ x + 0, y + 1, z + 0 })->scalarValue > threshold ? 2   : 0;
					data |= vertexGrid->GetVertex({ x + 1, y + 1, z + 0 })->scalarValue > threshold ? 4	  : 0;
					data |= vertexGrid->GetVertex({ x + 1, y + 0, z + 0 })->scalarValue > threshold ? 8	  : 0;

					data |= vertexGrid->GetVertex({ x + 0, y + 0, z + 1 })->scalarValue > threshold ? 16  : 0;
					data |= vertexGrid->GetVertex({ x + 0, y + 1, z + 1 })->scalarValue > threshold ? 32  : 0;
					data |= vertexGrid->GetVertex({ x + 1, y + 1, z + 1 })->scalarValue > threshold ? 64  : 0;
					data |= vertexGrid->GetVertex({ x + 1, y + 0, z + 1 })->scalarValue > threshold ? 128 : 0;



					AddMarchingCubesTriangles(out, vertexGrid->GetVertex({ x, y, z })->position, data, granularity);

				}
			}
		}
		CreateBuffers(out);
	}

	void Mesh::AddMarchingCubesTriangles(std::vector<Vertex>& out, glm::vec3 vertexPosition, unsigned char data, glm::f32 granularity)
	{
		//unsigned char iFlagsIndex = data;
		
		if (data == 0) //No vertices active -> no triangles generated
		{
			return;
		}
		//int iEdgeFlags = edgeTable[iFlagsIndex]; //Get which edges are intersecting the surface

		const int* triangleEdgeData = triTable[data];

		for (int i = 0; i < 16; i+=3)
		{
			if (triangleEdgeData[i] == -1)
			{
				break;
			}
			out.push_back({ vertexPosition + edgeToCoord[triangleEdgeData[i+0]] * granularity, glm::vec3(1, 0, 0), glm::vec2(0, 0) });
			out.push_back({ vertexPosition + edgeToCoord[triangleEdgeData[i+1]] * granularity, glm::vec3(1, 0, 0), glm::vec2(0, 0) });
			out.push_back({ vertexPosition + edgeToCoord[triangleEdgeData[i+2]] * granularity, glm::vec3(1, 0, 0), glm::vec2(0, 0) });
		}
		
	}

	Mesh::Mesh(std::istream* buffer, const std::string& filename)
	{
		Load(buffer, filename);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void Mesh::LoadMesh(std::istream* buffer, std::vector<Vertex>& out)
	{
		std::vector<glm::u32> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		while (!buffer->eof())
		{
			std::string command;
			*buffer >> command;
			if (command == "v")
			{
				glm::vec3 vertex;
				*buffer >> vertex.x >> vertex.y >> vertex.z;
				vertices.push_back(vertex);
			}
			else if (command == "vt")
			{
				glm::vec2 uv;
				*buffer >> uv.x >> uv.y;
				uvs.push_back(uv);
			}
			else if (command == "vn")
			{
				glm::vec3 normal;
				*buffer >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (command == "f")
			{
				glm::u32 vertexIndex[3], uvIndex[3], normalIndex[3];
				char padding;
				for (int i = 0; i < 3; i++)
				{
					*buffer >> vertexIndex[i] >> padding >> uvIndex[i] >> padding >> normalIndex[i];

					vertexIndices.push_back(vertexIndex[i]);
					uvIndices.push_back(uvIndex[i]);
					normalIndices.push_back(normalIndex[i]);
				}
			}
			else //Unknown command
			{
				std::getline(*buffer, command);
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

	void Mesh::Load(std::istream* buffer, const std::string& filename)
	{
		std::vector<Vertex> out;
		LoadMesh(buffer, out);
		CreateBuffers(out);
	}

	void Mesh::CreateBuffers(std::vector<Vertex>& out)
	{
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
}
