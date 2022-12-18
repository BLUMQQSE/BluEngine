#pragma once
namespace bm98::core
{

class ResourceManager
{

public:
	/// <summary>
	/// Loads DataAssets, Prefabs, and Textures.
	/// </summary>
	static void load_resources();

	/// <summary>
	/// Gets Json::Value data stored for prefab at prefab_file_name.
	/// </summary>
	/// <param name="prefab_file_name">Name of JSON file loading in.</param>
	static Json::Value get_prefab_data(std::string prefab_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of prefab file looking for.</param>
	static bool has_prefab_data(std::string prefab_file_name);
	/// <summary>
	/// Gets Json::Value data stored for DataAsset at asset_file_name.
	/// </summary>
	/// <param name="prefab_file_name">Name of JSON file loading in.</param>
	static Json::Value get_data_asset(std::string asset_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of DataAsset file looking for.</param>
	static bool has_data_asset(std::string asset_file_name);
	/// <summary>
	/// Gets a texture reference loaded from texture_file_name.
	/// </summary>
	/// <param name="texture_file_name">Name of texture file loading in.</param>
	static sf::Texture& get_texture(std::string texture_file_name);
	/// <summary>
	/// Verifies provided key is valid. Should only be needed by editor and
	/// components altered by editor.
	/// </summary>
	/// <param name="texture_file_name">Name of texture file looking for.</param>
	static bool has_texture(std::string texture_file_name);
	/// <summary>
	/// Gets a font reference loaded from font_file_name.
	/// </summary>
	/// <param name="font_file_name">Name of texture file loading in.</param>
	static sf::Font& get_font(std::string font_file_name);
	/// <summary>
	/// Verifies provided key is valid.
	/// </summary>
	/// <param name="font_file_name">Name of font file looking for.</param>
	static bool has_font(std::string font_file_name);

private:
	static std::unordered_map<std::string, Json::Value> prefab_data;
	static std::unordered_map<std::string, Json::Value> asset_data;
	static std::unordered_map<std::string, sf::Texture> textures;
	static std::unordered_map<std::string, sf::Font> fonts;

	static void iterate_prefab_directory(std::string dir_path);
	static void iterate_data_asset_directory(std::string dir_path);
	static void iterate_textures_directory(std::string dir_path);
	static void iterate_fonts_directory(std::string dir_path);
};

}