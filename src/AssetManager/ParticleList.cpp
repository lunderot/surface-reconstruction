#include "AssetManager/ParticleList.h"

namespace AssetManager
{
	ParticleList::ParticleList(std::istream* buffer, const std::string& filename)
	{
		min = max = glm::vec3(0, 0, 0);
		Load(buffer, filename);
	}

	ParticleList::~ParticleList()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void ParticleList::Load(std::istream* buffer, const std::string& filename)
	{
		while (!buffer->eof())
		{
			glm::vec3 vertex;
			*buffer >> vertex.x >> vertex.y >> vertex.z;
			if (vertex.x > max.x)
			{
				max.x = vertex.x;
			}
			else if(vertex.x < min.x)
			{
				min.x = vertex.x;
			}
			if (vertex.y > max.y)
			{
				max.y = vertex.y;
			}
			else if (vertex.y < min.y)
			{
				min.y = vertex.y;
			}
			if (vertex.z > max.z)
			{
				max.z = vertex.z;
			}
			else if (vertex.z < min.z)
			{
				min.z = vertex.z;
			}
			particles.push_back({ vertex, 1.0f });
		}

		//Generate vertex buffer and vertex array object
		glGenBuffers(1, &vbo);
		glGenVertexArrays(1, &vao);

		//Bind the buffers
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Load the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_STATIC_DRAW);

		//Enable the shader attributes
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(0);
		vertexCount = particles.size();
	}

	const std::vector<ParticleList::Particle>* ParticleList::GetParticles() const
	{
		return &particles;
	}

	GLuint ParticleList::GetVBO() const
	{
		return vbo;
	}

	GLuint ParticleList::GetVAO() const
	{
		return vao;
	}

	glm::u32 ParticleList::GetVertexCount() const
	{
		return vertexCount;
	}
}
