#include "systems/Physics.h"

namespace Systems
{
	using namespace kult;

	void Physics(float dt)
	{
		for (auto &id : join<Component::Position, Component::Physics>()) {
			auto& positionData = get<Component::Position>(id);
			auto& physicsData = get<Component::Physics>(id);

			physicsData.velocity += physicsData.acceleration * dt;
			positionData.pos += physicsData.velocity * dt;
			physicsData.rotationVelocity += physicsData.rotationAcceleration * dt;
			positionData.rot = glm::rotate(positionData.rot, physicsData.rotationVelocity.x * dt, glm::vec3(1, 0, 0));
			positionData.rot = glm::rotate(positionData.rot, physicsData.rotationVelocity.y * dt, glm::vec3(0, 1, 0));
			positionData.rot = glm::rotate(positionData.rot, physicsData.rotationVelocity.z * dt, glm::vec3(0, 0, 1));
		}
	}
}
