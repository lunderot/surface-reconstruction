#pragma once
#include <string>
#include <map>

template <typename T>
class AssetManager
{
private:
	std::string assetPath;
	std::map<std::string, T*> assets;
public:
	AssetManager(const std::string& assetPath);
	~AssetManager();

	T* GetAsset(const std::string& name);
};



template <typename T>
AssetManager<T>::AssetManager(const std::string& assetPath)
{
	this->assetPath = assetPath;
}

template <typename T>
AssetManager<T>::~AssetManager()
{
	for (auto i = assets.begin(); i != assets.end(); ++i)
	{
		delete i->second;
	}
}

template <typename T>
T* AssetManager<T>::GetAsset(const std::string& name)
{
	T* returnAsset = nullptr;
	try
	{
		returnAsset = assets.at(name);
	}
	catch (const std::out_of_range&)
	{
		returnAsset = new T(assetPath + name);
		assets[name] = returnAsset;
	}
	return returnAsset;
}
