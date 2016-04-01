#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "AssetManager/ParticleList.h"

class VertexGrid
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::int32 scalarValue;
		std::vector<AssetManager::ParticleList::Particle*> particles;
	};

private:
	glm::vec3 lowerBound;
	glm::vec3 upperBound;
	glm::f32 vertexBoundingBoxSize;
	glm::ivec3 gridSize;

	std::vector<Vertex> vertices;

public:
	VertexGrid();
	VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 vertexBoundingBoxSize);
	~VertexGrid();
	void CalculateScalarValues();
	void AddParticleToGrid(AssetManager::ParticleList::Particle*);
	std::vector<Vertex>* GetVertices();
};