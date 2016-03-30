#pragma once
#include <kult.hpp>

#include "../AssetManager/Mesh.h"
#include "../AssetManager/Texture.h"

namespace Component
{
	struct RenderData
	{
		AssetManager::Mesh* mesh;
		AssetManager::Texture* texture;
		bool scaleUv;
		template<typename T> friend T&operator<<(T &os, const RenderData &self) {
			return os << "mesh", os;
		}
	};

	using Render = kult::component<'rend', RenderData>;
}
