#pragma once
#include "EventSystem.h"
namespace bm98
{
class DataAsset;
class ItemAsset;
}
namespace bm98::core
{

class ResourceManager: public Listener
{

public:
	// Returns instance of the ResourceManager
	static ResourceManager* Instance()
	{
		static ResourceManager instance;
		return &instance;
	}

	/// <summary>
	/// Loads DataAssets, Prefabs, and Textures.
	/// </summary>
	void load_resources();

	/// <summary>
	/// NOT IMPLEMENTED YET
	/// Reloads all resources. Can be used after editing data to apply to game
	/// while running. Does NOT ensure already instantiated gameobjects will
	/// refresh to use altered data.
	/// </summary>
	void reload();

	/// <summary>
	/// Gets Json::Value data stored for prefab at prefab_file_name.
	/// </summary>
	/// <param name="prefab_file_name">Name of JSON file loading in.</param>
	Json::Value get_prefab_data(std::string prefab_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of prefab file looking for.</param>
	bool has_prefab_data(std::string prefab_file_name);

	/// <summary>
	/// Gets DataAsset data stored for DataAsset at asset_file_name.
	/// </summary>
	/// <param name="prefab_file_name">Name of JSON file loading in.</param>
	DataAsset* get_data_asset(std::string asset_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of DataAsset file looking for.</param>
	bool has_data_asset(std::string asset_file_name);
	
	/// <summary>
	/// Gets sf::SoundBuffer data stored for audio clip at audio_file_name.
	/// </summary>
	/// <param name="prefab_file_name">Name of SoundBuffer file loading in.</param>
	sf::SoundBuffer& get_sound_buffer(std::string audio_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of SoundBuffer file looking for.</param>
	bool has_sound_buffer(std::string audio_file_name);
	

	/// <summary>
	/// Gets Json::Value data stored for audio clip at audio_file_name.
	/// </summary>
	/// <param name="prefab_file_name">Name of Json::Value file loading in.</param>
	Json::Value get_sound_data(std::string audio_data_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of Json::Value file looking for.</param>
	bool has_sound_data(std::string audio_data_file_name);

	/// <summary>
	/// Gets a texture reference loaded from texture_file_name.
	/// </summary>
	/// <param name="texture_file_name">Name of texture file loading in.</param>
	sf::Texture& get_texture(std::string texture_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of texture file looking for.</param>
	bool has_texture(std::string texture_file_name);
	/// <summary>
	/// Gets a font reference loaded from font_file_name.
	/// </summary>
	/// <param name="font_file_name">Name of texture file loading in.</param>
	sf::Font& get_font(std::string font_file_name);
	/// <summary>
	/// Verifies provided key is valid.
	/// </summary>
	/// <param name="font_file_name">Name of font file looking for.</param>
	bool has_font(std::string font_file_name);

private:
	ResourceManager();
	~ResourceManager() {  }
	ResourceManager(const ResourceManager& rhs)
	{

	}
	ResourceManager& operator=(const ResourceManager& rhs) {}



	std::unordered_map<std::string, Json::Value> prefab_data;
	std::unordered_map<std::string, DataAsset*> asset_data; 
	std::unordered_map<std::string, sf::SoundBuffer> sound_buffers;
	std::unordered_map<std::string, Json::Value> sound_data;


	std::unordered_map<std::string, sf::Texture> textures;
	std::unordered_map<std::string, sf::Font> fonts;

	void iterate_prefab_directory(std::string dir_path);
	void iterate_data_asset_directory(std::string dir_path);
	void iterate_audio_directory(std::string dir_path);
	void iterate_textures_directory(std::string dir_path);
	void iterate_fonts_directory(std::string dir_path);

	// Inherited via Listener
	virtual void handle_event(Event* event) override;
};

}