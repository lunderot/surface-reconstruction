#pragma once
#include <kult.hpp>

#include "../AssetManager/ParticleList.h"

namespace Component
{
	struct PointRenderData
	{
		AssetManager::ParticleList* mesh;
		template<typename T> friend T&operator<<(T &os, const PointRenderData &self) {
			return os << "pren", os;
		}
	};

	using PointRender = kult::component<'pren', PointRenderData>;
}
