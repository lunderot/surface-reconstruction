#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/PointRender.h"

#include "../AssetManager/Shader.h"

namespace Systems
{
	void PointRender(AssetManager::Shader* shader, kult::entity camera, glm::ivec2 screenSize, glm::f32 fov, glm::f32 near, glm::f32 far);
}
