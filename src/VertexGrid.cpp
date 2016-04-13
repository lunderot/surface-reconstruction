#include "VertexGrid.h"

VertexGrid::VertexGrid()
{}

VertexGrid::VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 granularity, glm::f32 particleRadius, glm::f32 vertexBoundingBoxFactor)
{
	this->particleRadius = particleRadius;
	this->granularity = granularity;
	this->upperBound = upperBound;
	this->lowerBound = lowerBound;

	//should not be hardcoded
	vertexBoundingBoxSize = vertexBoundingBoxFactor * particleRadius;

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
	glm::f32 R = vertexBoundingBoxSize;

	glm::f32 scalarMax = std::numeric_limits<float>::min();
	glm::f32 scalarMin = std::numeric_limits<float>::max();

	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec3 numerator;
		glm::vec3 denominator;

		int it = 0;

		Vertex* vert = &vertices[i];
		for (AssetManager::ParticleList::Particle* p : vert->particles)
		{
			glm::f32 s = glm::length(vert->position - p->position) / R;
			glm::f32 k = glm::max(0.0f, glm::pow(1 - pow(s, 2), 3));
			numerator += p->position * k;
			denominator += k;
			it++;
		}

		if (it > 0)
		{
			glm::vec3 averageParticlePos = numerator / denominator;

			vert->scalarValue = glm::length(vert->position - averageParticlePos) - particleRadius;

			if (vert->scalarValue > scalarMax)
			{
				scalarMax = vert->scalarValue;
			}
			if (vert->scalarValue < scalarMin)
			{
				scalarMin = vert->scalarValue;
			}
		}
		else
		{
			vert->scalarValue = 1.0f;
		}
	}
}

void VertexGrid::AddParticleToGrid(AssetManager::ParticleList::Particle* particle)
{
	glm::vec3 localMin = particle->position - vertexBoundingBoxSize * 0.5f - lowerBound;
	glm::vec3 localMax = particle->position + vertexBoundingBoxSize * 0.5f - lowerBound;

	glm::ivec3 gridPosMin = glm::ceil(glm::max(localMin / granularity, 0.0f));
	glm::ivec3 gridPosMax = glm::floor(glm::min(localMax / granularity, glm::vec3(gridSize) -1.0f));

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

VertexGrid::Vertex* VertexGrid::GetVertex(glm::ivec3 gridPos)
{
	return &vertices[gridPos.x + gridSize.x * (gridPos.y + gridSize.y * gridPos.z)];
}

glm::vec3 VertexGrid::GridPosToWorldPos(glm::ivec3 gridPos)
{
	return lowerBound + granularity * glm::vec3(gridPos);
}

glm::f32 VertexGrid::GetVertexBoundingBoxSize() const
{
	return vertexBoundingBoxSize;
}

glm::f32 VertexGrid::GetGranularity() const
{
	return granularity;
}
