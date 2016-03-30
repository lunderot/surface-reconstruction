#pragma once
#include <string>
#include <map>

namespace AssetManager
{

	template <typename T>
	class AssetManager
	{
	private:
		std::string assetPath;
		std::map<std::string, T*> assets;
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
			if (inputFile.good())
			{
				std::size_t position = name.find_last_of('/');
				std::string filename;
				if (position == std::string::npos)
				{
					filename = name;
				}
				else
				{
					filename = name.substr(position + 1);
				}
				asset = assets[name] = new T(&inputFile, filename);
			}
			else
			{
				throw std::runtime_error("Failed to open asset: " + name);
			}
		}
		return asset;
	}
}