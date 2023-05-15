#include "pch.h"
#include "GraphicsSettings.h"
#include "core/FileManager.h"
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

	Json::Value obj = core::FileManager::Instance()->load_from_file(file_path);

	game_title = obj["game-title"].asString();
	resolution.width = obj["width"].asUInt64();
	resolution.height = obj["height"].asUInt64();

	full_screen = obj["fullscreen"].asBool();
	framerate_limit = obj["frame-rate-limit"].asUInt64();
	vertical_sync = obj["vertical-sync"].asBool();
	context_settings.antialiasingLevel = obj["context-settings"]["antialiasing-level"].asUInt64();

}

void GraphicsSettings::save_to_file(const std::string file_path)
{

	Json::Value obj;
	obj["game-title"] = game_title;
	obj["width"] = resolution.width;
	obj["height"] = resolution.height;

	obj["fullscreen"] = full_screen;
	obj["frame-rate-limit"] = framerate_limit;
	obj["vertical-sync"] = vertical_sync;
	obj["context-settings"]["antialiasing-level"] = context_settings.antialiasingLevel;

	core::FileManager::Instance()->save_to_file_styled(obj, file_path);

}

}