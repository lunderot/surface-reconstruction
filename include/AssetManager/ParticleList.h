#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

namespace AssetManager
{
	class ParticleList
	{
	private:
		std::vector<glm::vec3> particles;
		glm::vec3 min;
		glm::vec3 max;
	public:
		ParticleList(std::istream* buffer, const std::string& filename);
		~ParticleList();

		void Load(std::istream* buffer, const std::string& filename);

		const std::vector<glm::vec3>* GetParticles() const;
	};
}
