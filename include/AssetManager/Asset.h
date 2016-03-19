#pragma once

#include <fstream>

namespace AssetManager
{
	class Asset
	{
	public:
		Asset();
		virtual ~Asset();

		virtual void Load(std::istream* buffer, const std::string& filename) = 0;
	};
}
