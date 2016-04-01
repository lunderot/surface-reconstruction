#include "VertexGrid.h"

VertexGrid::VertexGrid()
{}

VertexGrid::VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 granularity)
{
	glm::vec3 boundingBoxSize = glm::abs(upperBound - lowerBound);
	boundingBoxSize /= granularity;
	gridSize = glm::ivec3(boundingBoxSize);
	gridSize += 1; //Offset to avoid losing particles at the upper edges.

	vertices.resize(gridSize.x * gridSize.y * gridSize.z);
	
	for (int x = 0; x < gridSize.x; x++)
	{
		for (int y = 0; y < gridSize.y; y++)
		{
			for (int z = 0; z < gridSize.z; z++)
			{
				vertices[x * (gridSize.y - 1) + y * (gridSize.z - 1) + z].position = lowerBound + granularity * glm::vec3(x,y,z);
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

void VertexGrid::AddParticleToGrid(AssetManager::ParticleList::Particle*)
{

}

std::vector<VertexGrid::Vertex>* VertexGrid::GetVertices()
{
	return &vertices;
}
