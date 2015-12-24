#pragma once
#include <string>
#include <map>
#include <fstream>

#include "Mesh.h"

class AssetManager
{
private:
	std::string assetPath;
	std::map<std::string, Mesh*> meshes;

	Mesh* LoadAsset(const std::string& name);

public:
	AssetManager(const std::string& assetPath);
	~AssetManager();

	Mesh* GetAsset(const std::string& name);
};

