#include "systems/Physics.h"

namespace Systems
{
	using namespace kult;

	void Physics(float dt)
	{
		for (auto &id : join<Component::Position, Component::Physics>()) {
			get<Component::Physics>(id).velocity += get<Component::Physics>(id).acceleration * dt;
			get<Component::Position>(id).pos += get<Component::Physics>(id).velocity * dt;
			get<Component::Physics>(id).rotationVelocity += get<Component::Physics>(id).rotationAcceleration * dt;
			get<Component::Position>(id).rot += get<Component::Physics>(id).rotationVelocity * dt;
		}
	}
}
