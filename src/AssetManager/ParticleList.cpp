#include "AssetManager/ParticleList.h"

namespace AssetManager
{
	ParticleList::ParticleList(std::istream* buffer, const std::string& filename)
	{
		min = max = glm::vec3(0, 0, 0);
		Load(buffer, filename);
	}

	ParticleList::ParticleList(glm::vec3 centerPosition, std::vector<Particle*>* particles)
	{
		for (auto i: *particles)
		{
			this->particles.push_back({ centerPosition, 1.0f, 1.0f });
			this->particles.push_back(*i);
		}
		GenerateBuffers();
	}

	ParticleList::ParticleList(std::vector<Particle>* particles)
	{
		this->particles = std::vector<Particle>(particles->begin(), particles->end());
		GenerateBuffers();
	}

	ParticleList::~ParticleList()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}

	void ParticleList::Load(std::istream* buffer, const std::string& filename)
	{
		float fMax = std::numeric_limits<float>::max();
		float fMin = std::numeric_limits<float>::min();
		max = { fMin ,fMin ,fMin };
		min = { fMax ,fMax ,fMax };
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
			particles.push_back({ vertex, 2.0f, 1.0f }); //TODO: This value shouldn't be hardcoded to 0.038f
		}

		GenerateBuffers();
	}

	void ParticleList::GenerateBuffers()
	{
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
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));

		glBindVertexArray(0);
		vertexCount = particles.size();
	}

	std::vector<ParticleList::Particle>* ParticleList::GetParticles()
	{
		return &particles;
	}

	glm::vec3 ParticleList::GetMin() const
	{
		return min;
	}

	glm::vec3 ParticleList::GetMax() const
	{
		return max;
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
