#pragma once

#include <string>
#include <istream>
#include <glm/glm.hpp>

namespace AssetManager
{
	class Config
	{
	private:
		union
		{
			char data[64];
			float floatValue;
			int integerValue;
			glm::vec3 vectorValue;
		};
	public:
		Config(std::istream* buffer, const std::string& filename);
		~Config();

		template<typename T>
		T Get()
		{
			return *static_cast<T*>(&data[0]);
		}

		template<>
		std::string Get<std::string>()
		{
			return std::string(data);
		}

		template<>
		float Get<float>()
		{
			return floatValue;
		}

		template<>
		int Get<int>()
		{
			return integerValue;
		}

		template<>
		glm::vec3 Get<glm::vec3>()
		{
			return vectorValue;
		}

		void Load(std::istream* buffer, const std::string& filename);
	};
}
