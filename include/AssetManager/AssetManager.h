#pragma once
#include <string>
#include <map>

#include "Asset.h"

namespace AssetManager
{

	template <typename T>
	class AssetManager
	{
	private:
		std::string assetPath;
		std::map<std::string, Asset*> assets;
	public:
		AssetManager(const std::string& assetPath);
		~AssetManager();

		T* Get(const std::string& name);
	};

	template <typename T>
	AssetManager<T>::AssetManager(const std::string& assetPath) :
		assetPath(assetPath)
	{
	}

	template <typename T>
	AssetManager<T>::~AssetManager()
	{
		for (auto i : assets)
		{
			delete i.second;
		}
	}

	template <typename T>
	T* AssetManager<T>::Get(const std::string& name)
	{
		T* asset = nullptr;
		try
		{
			//Return the asset if it is already loaded
			asset = static_cast<T*>(assets.at(name));
		}
		catch (const std::out_of_range&)
		{
			//Load and return the asset if it is not already loaded
			std::ifstream inputFile(assetPath + name, std::ios::binary);
			asset = new T();
			asset->Load(&inputFile, name);

			assets[name] = asset;
		}
		return asset;
	}
}