#include "../pch.h"
#include "Debug.h"
#include "EventSystem.h"
#include "ResourceManager.h"
#include "FileManager.h"
#include "../data_assets/DataAssets.h"
#include "../TilemapComponent.h"
namespace bm98::core 
{

void ResourceManager::load_resources()
{
	iterate_textures_directory("Resources/Images/");
	iterate_fonts_directory("Fonts/");
	iterate_audio_directory("Resources/Audio/Sounds/");
	iterate_prefab_directory("Data/Prefabs/");
	
	iterate_data_asset_directory("Data/DataAssets/");
	
	unserialize_data_assets();

	TilemapComponent::LoadTileSheets();
}

void ResourceManager::reload()
{
	asset_data.clear();
	iterate_data_asset_directory("Data/DataAssets/");
	unserialize_data_assets();
	EventSystem::Instance()->push_event(EventID::_SYSTEM_RESOURCES_RELOAD_FLAG_);
}

Json::Value ResourceManager::get_prefab_data(std::string prefab_file_name)
{
	return prefab_data.at(prefab_file_name);
}

bool ResourceManager::has_prefab_data(std::string prefab_file_name)
{
	if (prefab_data.find(prefab_file_name) == prefab_data.end())
	{
		Debug::Instance()->core_log("ResourceManager::PREFAB " + prefab_file_name + " DOES NOT EXIST", Debug::LogLevel::WARNING);
		return false;
	}
	return true;
}
/*
DataAsset* ResourceManager::get_data_asset(std::string asset_file_name)
{
	return asset_data.at(asset_file_name);
}
*/



bool ResourceManager::has_data_asset(std::string asset_file_name)
{
	if (asset_data.find(asset_file_name) == asset_data.end())
	{
		Debug::Instance()->core_log("ResourceManager::ASSET DATA " + asset_file_name + " DOES NOT EXIST", Debug::LogLevel::WARNING);
		return false;
	}
	return true;
}

sf::SoundBuffer& ResourceManager::get_sound_buffer(std::string audio_file_name)
{
	return sound_buffers.at(audio_file_name);
}

bool ResourceManager::has_sound_buffer(std::string audio_file_name)
{
	if (sound_buffers.find(audio_file_name) == sound_buffers.end())
		return false;

	return true;
}

Json::Value ResourceManager::get_sound_data(std::string audio_data_file_name)
{
	return sound_data.at(audio_data_file_name);
}

bool ResourceManager::has_sound_data(std::string audio_data_file_name)
{
	if (sound_data.find(audio_data_file_name) == sound_data.end())
	{

		Debug::Instance()->core_log("ResourceManager::AUDIO DATA " + audio_data_file_name + " DOES NOT EXIST", Debug::LogLevel::WARNING);
		return false;
	}
	return true;
}

sf::Texture& ResourceManager::get_texture(std::string texture_file_name)
{
	return textures.at(texture_file_name);
}

bool ResourceManager::has_texture(std::string texture_file_name)
{
	if (textures.find(texture_file_name) == textures.end())
	{

		Debug::Instance()->core_log("ResourceManager::TEXTURE " + texture_file_name + " DOES NOT EXIST", Debug::LogLevel::WARNING);
		return false;
	}
	return true;
}

sf::Font& ResourceManager::get_font(std::string font_file_name)
{
	return fonts.at(font_file_name);
}

bool ResourceManager::has_font(std::string font_file_name)
{
	if (fonts.find(font_file_name) == fonts.end())
	{
		Debug::Instance()->core_log("ResourceManager::FONT " + font_file_name+ " DOES NOT EXIST", Debug::LogLevel::WARNING);
		return false;
	}
	return true;
}

void ResourceManager::shutdown()
{

}

ResourceManager::ResourceManager()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_RESOURCEMANAGER_LOAD_RESOURCES_, this);
}

void ResourceManager::iterate_prefab_directory(std::string dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			prefab_data[file_name] = FileManager::Instance()->load_from_file(dir_path + file_name);
			
			continue;
		}
	
		iterate_prefab_directory(dir_path + file_name + "/");
		
	}
}

void ResourceManager::iterate_data_asset_directory(std::string dir_path)
{
	int x = 1;
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			if (entry.path().string().find("ItemData") != std::string::npos)
			{
				if (entry.path().string().find("WeaponData") != std::string::npos)
					asset_data[file_name] = std::make_shared<WeaponData>(file_name);
				else
					asset_data[file_name] = std::make_shared<ItemData>(file_name);
			}
			else if (entry.path().string().find("EnemyData") != std::string::npos)
				asset_data[file_name] = std::make_shared<EnemyData>(file_name);
			else if (entry.path().string().find("DamagerData") != std::string::npos)
				asset_data[file_name] = std::make_shared<DamagerData>(file_name);
			else if (entry.path().string().find("EffectData") != std::string::npos)
				asset_data[file_name] = std::make_shared<EffectData>(file_name);
			else
			{
				asset_data[file_name] = std::make_shared<DataAsset>(file_name);
			}

			asset_data.at(file_name)->unserialize_json(FileManager::Instance()->load_from_file(dir_path + file_name));
			continue;
		}
		
		iterate_data_asset_directory(dir_path + file_name + "/");
		
	}
	int y= 2;
}

void ResourceManager::iterate_audio_directory(std::string dir_path)
{
	for (const auto& entry : std::filesystem::directory_iterator(dir_path))
	{
		std::string file_name = entry.path().string().substr(dir_path.size(), entry.path().string().size() - 1);
		if (!entry.is_directory())
		{
			if (file_name.substr(file_name.find_last_of(".") + 1) == "json")
			{
				sound_data[file_name] = FileManager::Instance()->load_from_file(dir_path + file_name);

				continue;
			}

			sf::SoundBuffer sound_to_add;
			if (!sound_to_add.loadFromFile(dir_path + file_name))
			{
				Debug::Instance()->core_log("RESOURCEMANAGER FAILED TO LOAD FILE " + file_name, Debug::LogLevel::FAILURE);
			}
			sound_buffers[file_name] = sound_to_add;
			continue;
		}
		
		iterate_audio_directory(dir_path + file_name + "/");
		
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
			if (!texture_to_add.loadFromFile(dir_path + file_name))
			{
				Debug::Instance()->core_log("RESOURCEMANAGER FAILED TO LOAD FILE " + file_name, Debug::LogLevel::FAILURE);
			}
			textures[file_name] = texture_to_add;
			continue;
		}
		
		iterate_textures_directory(dir_path + file_name + "/");
		
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
			if (!font_to_add.loadFromFile(dir_path + file_name))
			{
				Debug::Instance()->core_log("RESOURCEMANAGER FAILED TO LOAD FILE " + file_name, Debug::LogLevel::FAILURE);
			}
			fonts[file_name] = font_to_add;

			continue;
		}
		
		iterate_fonts_directory(dir_path + file_name + "/");
		
	}
}

void ResourceManager::unserialize_data_assets()
{

}

void ResourceManager::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::_SYSTEM_RESOURCEMANAGER_LOAD_RESOURCES_:
		load_resources();
		break;
	}
}


}