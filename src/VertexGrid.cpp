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
	gridSize += 2; //Offset to avoid losing particles at the upper edges.		

	vertices.resize(gridSize.x * gridSize.y * gridSize.z);
	
	for (int z = 0; z < gridSize.z; z++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			for (int x = 0; x < gridSize.x; x++)
			{
				GetVertex({x,y,z})->position = GridPosToWorldPos({x,y,z});
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

	for (int x = gridPosMin.x; x < gridPosMax.x; x++)
	{
		for (int y = gridPosMin.y; y < gridPosMax.y; y++)
		{
			for (int z = gridPosMin.z; z < gridPosMax.z; z++)
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

VertexGrid::Vertex* VertexGrid::GetVertex(glm::ivec3 gridPos)
{
	return &vertices[gridPos.x + gridSize.x * (gridPos.y + gridSize.y * gridPos.z)];
}

glm::vec3 VertexGrid::GridPosToWorldPos(glm::ivec3 gridPos)
{
	return lowerBound + granularity * glm::vec3(gridPos);
}
