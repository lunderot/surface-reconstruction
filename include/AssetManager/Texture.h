#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Asset.h"

namespace AssetManager
{
	class Texture : public Asset
	{
	private:
		GLuint tex;
	public:
		Texture();
		~Texture();

		void Load(std::istream* buffer, const std::string& filename);

		GLuint GetTexture() const;
	};
}
