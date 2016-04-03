#include "AssetManager/Config.h"

#include <string>

namespace AssetManager
{
	Config::Config(std::istream* buffer, const std::string& filename)
	{
		Load(buffer, filename);
	}

	Config::~Config()
	{
	}

	void Config::Load(std::istream* buffer, const std::string& filename)
	{
		
		switch (filename[0])
		{
		case 'i':
		{
			*buffer >> integerValue;
		}
			break;
		case 's':
		{
			std::string str((std::istreambuf_iterator<char>(*buffer)), std::istreambuf_iterator<char>());
			std::memset(data, '\0', 64);
			std::memcpy(data, str.data(), 64);
		}
			break;
		case 'f':
		{
			*buffer >> floatValue;
		}
			break;
		case 'v':
		{
			*buffer >> vectorValue.x >> vectorValue.y >> vectorValue.z;
		}
			break;
		default:
		{
			throw std::runtime_error("Invalid config variable type");
		}
			break;
		}
	}
}
