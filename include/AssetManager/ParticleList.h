#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

namespace AssetManager
{
	class ParticleList
	{
	public:
		struct Particle
		{
			glm::vec3 position;
			glm::f32 radius;
			glm::f32 scalar;
		};

	private:
		GLuint vbo;
		GLuint vao;
		unsigned int vertexCount;
		std::vector<Particle> particles;
		glm::vec3 min;
		glm::vec3 max;

		void GenerateBuffers();

	public:
		ParticleList(std::vector<Particle>* particles);
		ParticleList(std::istream* buffer, const std::string& filename);
		~ParticleList();

		void Load(std::istream* buffer, const std::string& filename);

		std::vector<Particle>* GetParticles();

		glm::vec3 GetMin() const;
		glm::vec3 GetMax() const;
		GLuint GetVBO() const;
		GLuint GetVAO() const;
		glm::u32 GetVertexCount() const;
	};
}
