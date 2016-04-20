#include "VertexGrid.h"

VertexGrid::VertexGrid()
{}

VertexGrid::VertexGrid(glm::vec3 lowerBound, glm::vec3 upperBound, glm::f32 granularity, glm::f32 particleRadius, glm::f32 tHigh, glm::f32 tLow, glm::f32 vertexBoundingBoxFactor)
{
	this->particleRadius = particleRadius;
	this->granularity = granularity;
	this->upperBound = upperBound;
	this->lowerBound = lowerBound;
	this->tHigh = tHigh;
	this->tLow = tLow;

	//should not be hardcoded
	vertexBoundingBoxSize = vertexBoundingBoxFactor * particleRadius;

	glm::vec3 boundingBoxSize = glm::abs(upperBound - lowerBound);
	gridSize = glm::ivec3(boundingBoxSize / granularity);
	gridSize += 3; //Offset to avoid losing particles at the edges.

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
		Vertex* vert = &vertices[i];
		glm::vec3 numerator;
		glm::vec3 denominator;
		int it = 0;
		
		// x = ∑j xj k(|x − xj| / R)
		//		∑j k(|x − xj| / R)
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
			//glm::mat3 gradient; //=
			//glm::f32 EVmax = CalculateMaximumEigenvalue(gradient);
			//glm::f32 f = CalculateSpuriousBlobFactor(EVmax);

			glm::vec3 averageParticlePos = numerator / denominator;

			//φ(x) = |x − ¯x| − ¯r * f
			vert->scalarValue = glm::length(vert->position - averageParticlePos) - particleRadius;// *f;

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

glm::f32 VertexGrid::CalculateMaximumEigenvalue(glm::mat3 gradient)
{
	vmml::mat3f vmmGradient(
		gradient[0].x, gradient[0].y, gradient[0].z,
		gradient[1].x, gradient[1].y, gradient[1].z,
		gradient[2].x, gradient[2].y, gradient[2].z
		);
	vmml::vec3f eigenvalues;
	vmml::mat3f eigenvectors;
	size_t rotationCount;
	vmml::solveJacobi3x3(vmmGradient, eigenvalues, eigenvectors, rotationCount);

	return eigenvalues.getMaxComponent();
}

glm::f32 VertexGrid::CalculateSpuriousBlobFactor(glm::f32 EVmax)
{
	if (EVmax < tLow)
	{
		return 1.0;
	}
	else
	{
		//γ = tHigh − EVmax
		//		tHigh − tLow
		glm::f32 gamma = (tHigh - EVmax) / (tHigh - tLow);

		//f = γ^3 − 3γ^2 + 3γ;
		glm::f32 gammaPowTwo = gamma * gamma;
		return gamma * gammaPowTwo - 3 * gammaPowTwo + 3 * gamma;
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