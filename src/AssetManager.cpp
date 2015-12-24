#include "AssetManager.h"


AssetManager::AssetManager(const std::string& assetPath)
{
	this->assetPath = assetPath;
}

AssetManager::~AssetManager()
{
	for (auto i = meshes.begin(); i != meshes.end(); ++i)
	{
		delete i->second;
	}
}

Mesh* AssetManager::LoadAsset(const std::string& name)
{
	return new Mesh(assetPath + name);
}

Mesh* AssetManager::GetAsset(const std::string& name)
{
	Mesh* returnMesh = nullptr;
	try
	{
		returnMesh = meshes.at(name);
	}
	catch (const std::out_of_range& e)
	{
		returnMesh = LoadAsset(name);
		meshes[name] = returnMesh;
	}
	return returnMesh;
}
