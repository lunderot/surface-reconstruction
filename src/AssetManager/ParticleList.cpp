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
			particles.push_back(vertex);
		}
	}

	const std::vector<glm::vec3>* ParticleList::GetParticles() const
	{
		return &particles;
	}
}
