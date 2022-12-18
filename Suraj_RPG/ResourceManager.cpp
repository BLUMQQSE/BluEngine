#include "pch.h"
#include "ResourceManager.h"
#include "FileManager.h"
namespace bm98::core 
{
std::unordered_map<std::string, Json::Value> ResourceManager::asset_data;
std::unordered_map<std::string, Json::Value> ResourceManager::prefab_data;
std::unordered_map<std::string, sf::Texture> ResourceManager::textures;
std::unordered_map<std::string, sf::Font> ResourceManager::fonts;

void ResourceManager::load_resources()
{
	iterate_data_asset_directory("Data/DataAssets/");
	iterate_prefab_directory("Data/Prefabs/");
	iterate_textures_directory("Resources/Images/");
	iterate_fonts_directory("Fonts/");
	//for (auto& c : textures)
		//std::cout << c.first << "\n";
}

Json::Value ResourceManager::get_prefab_data(std::string prefab_file_name)
{
	return prefab_data.at(prefab_file_name);
}

bool ResourceManager::has_prefab_data(std::string prefab_file_name)
{
	if (prefab_data.find(prefab_file_name) == prefab_data.end())
		return false;

	return true;
}

Json::Value ResourceManager::get_data_asset(std::string asset_file_name)
{
	return asset_data.at(asset_file_name);
}

bool ResourceManager::has_data_asset(std::string asset_file_name)
{
	if (asset_data.find(asset_file_name) == asset_data.end())
		return false;

	return true;
}

sf::Texture& ResourceManager::get_texture(std::string texture_file_name)
{
	return textures.at(texture_file_name);
}

bool ResourceManager::has_texture(std::string texture_file_name)
{
	if (textures.find(texture_file_name) == textures.end())
		return false;
		
	return true;
}

sf::Font& ResourceManager::get_font(std::string font_file_name)
{
	return fonts.at(font_file_name);
}

bool ResourceManager::has_font(std::string font_file_name)
{
	if (fonts.find(font_file_name) == fonts.end())
		return false;

	return true;
}

void ResourceManager::iterate_prefab_directory(std::string dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			prefab_data[file_name] = FileManager::load_from_file(dir_path + file_name);
		}
		else
		{
			iterate_prefab_directory(dir_path + file_name + "/");
		}
	}
}

void ResourceManager::iterate_data_asset_directory(std::string dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			asset_data[file_name] = FileManager::load_from_file(dir_path + file_name);
		}
		else
		{
			iterate_data_asset_directory(dir_path + file_name + "/");
		}
	}
}

void ResourceManager::iterate_textures_directory(std::string dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			sf::Texture texture_to_add;
			std::string address = dir_path + file_name;
			if (!texture_to_add.loadFromFile(address))
			{
				std::cout << "ERROR::RESOURCEMANAGER::FAILED LOADING FILE: " << address << "\n";
			}
			textures[file_name] = texture_to_add;
		}
		else
		{
			iterate_textures_directory(dir_path + file_name + "/");
		}
	}
}

void ResourceManager::iterate_fonts_directory(std::string dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			sf::Font font_to_add;
			std::string address = dir_path + file_name;
			if (!font_to_add.loadFromFile(address))
			{
				std::cout << "ERROR::RESOURCEMANAGER::FAILED LOADING FILE: " << address << "\n";
			}
			fonts[file_name] = font_to_add;
		}
		else
		{
			iterate_textures_directory(dir_path + file_name + "/");
		}
	}
}


}