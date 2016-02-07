#pragma once
#include <kult.hpp>
#include <glm/glm.hpp>

namespace Component
{
	struct PositionData
	{
		glm::vec3 pos;
		glm::vec3 rot;

		template<typename T> friend T&operator<<(T &os, const PositionData &self) {
			return os << "(x:" << self.pos.x << ",y:" << self.pos.y << ",z:" << self.pos.z << ",rx:" << self.rot.x << ",ry : " << self.rot.y << ",rz : " << self.rot.z << ")", os;
		}
	};

	using Position = kult::component<'pos', PositionData>;
}
