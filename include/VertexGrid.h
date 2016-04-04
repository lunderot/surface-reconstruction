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
	glm::f32 vertexBoundingBoxSize;
	glm::vec3 lowerBound;
	glm::vec3 upperBound;
	glm::f32 particleRadius;
	glm::f32 granularity;
	glm::uvec3 gridSize;
	std::vector<Vertex> vertices;

public:
	VertexGrid();
	VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 vertexBoundingBoxSize, glm::f32 particleRadius);
	~VertexGrid();
	void CalculateScalarValues();
	void AddParticleToGrid(AssetManager::ParticleList::Particle*);
	std::vector<Vertex>* GetVertices();
	glm::uvec3 GetGridSize() const;
	Vertex* GetVertex(glm::ivec3 position);
};