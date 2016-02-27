#pragma once
#include <kult.hpp>

#include "components/Position.h"
#include "components/Physics.h"
#include "components/Freelook.h"

#include <SDL.h>

namespace Systems
{
	void HandleFreelookEvent(SDL_Event& event);
}
