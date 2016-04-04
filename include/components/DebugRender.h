#pragma once
#include <kult.hpp>

#include "../AssetManager/ParticleList.h"

namespace Component
{
	struct DebugRenderData
	{
		bool renderThis;
		bool renderLines;
		AssetManager::ParticleList* mesh;
		template<typename T> friend T&operator<<(T &os, const DebugRenderData &self) {
			return os << "pren", os;
		}
	};

	using DebugRender = kult::component<'pren', DebugRenderData>;
}
