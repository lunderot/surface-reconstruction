#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <vmmlib/vector3.h>
#include <vmmlib/matrix3.h>
#include <vmmlib/jacobiSolver.h>
#include "AssetManager/ParticleList.h"

class VertexGrid
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::f32 scalarValue;
		std::vector<AssetManager::ParticleList::Particle*> particles;
	};

private:
	glm::f32 vertexBoundingBoxSize;
	glm::vec3 lowerBound;
	glm::vec3 upperBound;
	glm::f32 particleRadius;
	glm::f32 granularity;
	glm::uvec3 gridSize;
	glm::f32 tHigh;
	glm::f32 tLow;
	std::vector<Vertex> vertices;

	glm::f32 CalculateSpuriousBlobFactor(glm::f32 EVmax);
	glm::f32 CalculateMaximumEigenvalue(glm::mat3 gradient);

public:
	VertexGrid();
	VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 granularity, glm::f32 particleRadius, glm::f32 tHigh, glm::f32 tLow, glm::f32 vertexBoundingBoxFactor);
	~VertexGrid();
	void CalculateScalarValues();
	void AddParticleToGrid(AssetManager::ParticleList::Particle*);
	std::vector<Vertex>* GetVertices();
	glm::uvec3 GetGridSize() const;
	Vertex* GetVertex(glm::ivec3 gridPos);
	glm::vec3 GridPosToWorldPos(glm::ivec3 gridPos);
	glm::f32 GetVertexBoundingBoxSize() const;
	glm::f32 GetGranularity() const;
};