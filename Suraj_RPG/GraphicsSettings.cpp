#include "pch.h"
#include "GraphicsSettings.h"
namespace bm98
{
GraphicsSettings::GraphicsSettings()
{
	this->resolution = sf::VideoMode::getDesktopMode();
	this->full_screen = false;
	this->framerate_limit = 120;
	this->context_settings.antialiasingLevel = 0;
	this->video_modes = sf::VideoMode::getFullscreenModes();
}

void GraphicsSettings::load_from_file(const std::string file_path)
{
	std::ifstream ifs(file_path);
	game_title = "None";

	if (ifs.is_open())
	{
		std::getline(ifs, game_title);
		ifs >> resolution.width >> resolution.height;
		ifs >> full_screen;
		ifs >> framerate_limit;
		ifs >> vertical_sync;
		ifs >> context_settings.antialiasingLevel;
	}

	ifs.close();
}

void GraphicsSettings::save_to_file(const std::string file_path)
{
	std::ofstream ofs(file_path);
	game_title = "None";

	if (ofs.is_open())
	{
		ofs << game_title;
		ofs << resolution.width << " " << resolution.height;
		ofs << full_screen;
		ofs << framerate_limit;
		ofs << vertical_sync;
		ofs << context_settings.antialiasingLevel;
	}

	ofs.close();
}
}