#pragma once

#include <fstream>

namespace AssetManager
{
	class Asset
	{
	public:
		Asset();
		virtual ~Asset();

		virtual void Load(std::istream* buffer) = 0;
	};
}
