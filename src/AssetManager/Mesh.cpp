#include "AssetManager/Mesh.h"

#include <iostream>

namespace AssetManager
{
	Mesh::Mesh(VertexGrid* vertexGrid) //Generate a mesh from points using cube marching
	{
		glm::uvec3 gridSize = vertexGrid->GetGridSize();
		glm::f32 granularity = vertexGrid->GetGranularity();

		std::vector<Vertex> out;

		for (int z = 0; z < gridSize.z - 2; z++)
		{
			for (int y = 0; y < gridSize.y - 2; y++)
			{
				for (int x = 0; x < gridSize.x - 2; x++)
				{
					unsigned char data = 0;
					glm::f32 weights[8] =
					{
						vertexGrid->GetVertex({ x + 0, y + 0, z + 0 })->scalarValue,
						vertexGrid->GetVertex({ x + 0, y + 1, z + 0 })->scalarValue,
						vertexGrid->GetVertex({ x + 1, y + 1, z + 0 })->scalarValue,
						vertexGrid->GetVertex({ x + 1, y + 0, z + 0 })->scalarValue,

						vertexGrid->GetVertex({ x + 0, y + 0, z + 1 })->scalarValue,
						vertexGrid->GetVertex({ x + 0, y + 1, z + 1 })->scalarValue,
						vertexGrid->GetVertex({ x + 1, y + 1, z + 1 })->scalarValue,
						vertexGrid->GetVertex({ x + 1, y + 0, z + 1 })->scalarValue
					};

					//If the scalar value for a specific node is positive, add it to the marching cube data
					data |= weights[0] > 0.0f ? 1 : 0;
					data |= weights[1] > 0.0f ? 2 : 0;
					data |= weights[2] > 0.0f ? 4 : 0;
					data |= weights[3] > 0.0f ? 8 : 0;

					data |= weights[4] > 0.0f ? 16 : 0;
					data |= weights[5] > 0.0f ? 32 : 0;
					data |= weights[6] > 0.0f ? 64 : 0;
					data |= weights[7] > 0.0f ? 128 : 0;



					AddMarchingCubesTriangles(out, { x, y, z }, data, granularity, weights, vertexGrid);

				}
			}
		}
		for (size_t i = 0; i < out.size(); i += 3)
		{
			glm::vec3 normal = glm::normalize(glm::cross(out[i].position - out[i + 1].position, out[i].position - out[i + 2].position));
			out[i].normal = out[i + 1].normal = out[i + 2].normal = normal;
		}
		
		for (auto v: *vertexGrid->GetVertices())
		{
			for (int i = 0; i < 3; i++)
			{
				glm::vec3 normal;
				VertexGrid::Edge& e = v.edges[i];
				for (int j = 0; j <= e.index; j++)
				{
					normal += out[e.vertexRefrence[j]].normal;
				}
				normal = glm::normalize(normal);
				for (int j = 0; j <= e.index; j++)
				{
					out[e.vertexRefrence[j]].normal = normal;
				}
			}
		}

		CreateBuffers(out);
	}

	void Mesh::AddMarchingCubesTriangles(std::vector<Vertex>& out, glm::uvec3 vertexIndex, unsigned char data, glm::f32 granularity, glm::f32 weights[8], VertexGrid* vertexGrid)
	{
		if (data == 0 || data == 255) //No vertices active -> no triangles generated
		{
			return;
		}
		VertexGrid::Vertex* vertex = vertexGrid->GetVertex(vertexIndex);

		const int* triangleEdgeData = triTable[data];

		if (vertexIndex.x == 16 &&
			vertexIndex.y == 2 &&
			vertexIndex.z == 0)
		{
			std::cout << "jhdfk" << std::endl;
		}

		int i = 0;
		while (triangleEdgeData[i] != -1)
		{
			

			int currentEdge = triangleEdgeData[i];
			const int* vert = edgeToVert[currentEdge];
			glm::vec3 interpolatedVertex = (vertToCoord[vert[0]] + (-weights[vert[0]] / (weights[vert[1]] - weights[vert[0]])) * (vertToCoord[vert[1]] - vertToCoord[vert[0]]));

			out.push_back({  vertex->position + interpolatedVertex * granularity, glm::vec3(1, 0, 0), glm::vec2(0, 0) });

			glm::uvec3 currentVertexIndex = vertexIndex;
			int edgeId = 0;
			if (currentEdge == 3)
			{
				edgeId = 1;
			}
			else if (currentEdge == 8)
			{
				edgeId = 2;
			}
			//Lower
			if (currentEdge == 1)
			{
				currentVertexIndex.y++;
				edgeId = 1;
			}
			else if (currentEdge == 2)
			{
				currentVertexIndex.x++;
				edgeId = 0;
			}

			//Middle
			else if (currentEdge == 9)
			{
				currentVertexIndex.y++;
				edgeId = 2;
			}
			else if (currentEdge == 10)
			{
				currentVertexIndex.x++;
				currentVertexIndex.y++;
				edgeId = 2;
			}

			else if (currentEdge == 11)
			{
				currentVertexIndex.x++;
				edgeId = 2;
			}


			//Upper
			else if (currentEdge == 4)
			{
				currentVertexIndex.z++;
				edgeId = 0;
			}
			else if (currentEdge == 5)
			{
				currentVertexIndex.y++;
				currentVertexIndex.z++;
				edgeId = 1;
			}
			else if (currentEdge == 6)
			{
				currentVertexIndex.x++;
				currentVertexIndex.z++;
				edgeId = 0;
			}
			else if (currentEdge == 7)
			{
				currentVertexIndex.z++;
				edgeId = 2;
			}

			auto& e = vertexGrid->GetVertex(currentVertexIndex)->edges[edgeId]; 
			e.vertexRefrence[e.index++] = out.size() - 1;

			if (currentVertexIndex.x == 17 && 
				currentVertexIndex.y == 3  &&
				currentVertexIndex.z == 0 && edgeId == 2)
			{
				std::cout << "jhdfk" << std::endl;
			}

			i++;
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
