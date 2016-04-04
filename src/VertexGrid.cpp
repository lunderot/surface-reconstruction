#include "VertexGrid.h"

VertexGrid::VertexGrid()
{}

VertexGrid::VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 granularity, glm::f32 particleRadius)
{
	this->particleRadius = particleRadius;
	this->granularity = granularity;
	this->upperBound = upperBound;
	this->lowerBound = lowerBound;

	//should not be hardcoded
	vertexBoundingBoxSize = 4.0f * particleRadius;

	glm::vec3 boundingBoxSize = glm::abs(upperBound - lowerBound);
	gridSize = glm::ivec3(boundingBoxSize / granularity);
	gridSize += 1; //Offset to avoid losing particles at the upper edges.

	vertices.resize(gridSize.x * gridSize.y * gridSize.z);
	
	int it = 0;

	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			for (int z = 0; z < gridSize.z; z++)
			{
				vertices.at(it++).position = lowerBound + granularity * glm::vec3(x, y, z);
			}
		}
	}
}

VertexGrid::~VertexGrid()
{

}

void VertexGrid::CalculateScalarValues()
{

}

void VertexGrid::AddParticleToGrid(AssetManager::ParticleList::Particle* particle)
{
	glm::vec3 localMin = particle->position - vertexBoundingBoxSize * 0.5f;
	glm::vec3 localMax = particle->position + vertexBoundingBoxSize * 0.5f;

	glm::ivec3 gridPosMin = glm::ceil(glm::max((localMin - lowerBound) / granularity, 0.0f));
	glm::ivec3 gridPosMax = glm::floor(glm::min((localMax - lowerBound) / granularity, glm::vec3(gridSize)));

	for (int x = gridPosMin.x; x <= gridPosMax.x; x++)
	{
		for (int y = gridPosMin.y; y <= gridPosMax.y; y++)
		{
			for (int z = gridPosMin.z; z <= gridPosMax.z; z++)
			{
				GetVertex({x,y,z})->particles.push_back(particle);
			}
		}
	}
}

std::vector<VertexGrid::Vertex>* VertexGrid::GetVertices()
{
	return &vertices;
}

glm::uvec3 VertexGrid::GetGridSize() const
{
	return gridSize;
}

VertexGrid::Vertex* VertexGrid::GetVertex(glm::ivec3 position)
{
	//return &vertices[position.x * (gridSize.y - 1) * (gridSize.z - 1) + position.y * (gridSize.z - 1) + position.z];
	return &vertices[position.x + (gridSize.y - 1) * (position.y + (gridSize.x - 1) * position.z)];
}