#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/Render.h"

#include "../Shader.h"
#include "../Camera.h"

namespace Systems
{
	void Render(Shader* shader, Camera* camera);
}
