#pragma once
#include <kult.hpp>

#include "../Mesh.h"
#include "../Texture.h"

namespace Component
{
	struct RenderData
	{
		Mesh* mesh;
		Texture* texture;
		bool scaleUv;
		template<typename T> friend T&operator<<(T &os, const RenderData &self) {
			return os << "mesh", os;
		}
	};

	using Render = kult::component<'rend', RenderData>;
}
