#pragma once
namespace bm98
{
class GraphicsSettings
{
public:
	GraphicsSettings();

	sf::VideoMode resolution;
	bool full_screen;
	bool vertical_sync;
	unsigned framerate_limit;
	sf::ContextSettings context_settings;
	std::vector<sf::VideoMode> video_modes;
	std::string game_title;

	void load_from_file(const std::string file_path);
	void save_to_file(const std::string file_path);


};
}