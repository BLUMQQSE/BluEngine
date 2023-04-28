#include "../pch.h"
#include "Debug.h"
namespace bm98::core
{
/*
sf::Text Debug::text_display;
bool Debug::active = true;
Sorting::Layer Debug::layer = Sorting::Layer::UI;
char Debug::z_order = 100;

void Debug::init()
{
	Renderer::Instance()->add(Renderer::RenderObject(&text_display, active, layer, z_order));
}

void Debug::Log(std::string message)
{
	std::cout << message << "\n";
}

void Debug::LogError(std::string message)
{
	std::cout << message << "\n";
}


*/


void Debug::core_log(std::string message, LogLevel level)
{
	switch (level)
	{
	case bm98::core::Debug::LogLevel::BASIC:
		core_logger->trace(message);
		break;
	case bm98::core::Debug::LogLevel::INFO:
		core_logger->info(message);
		break;
	case bm98::core::Debug::LogLevel::WARNING:
		core_logger->warn(message);
		break;
	case bm98::core::Debug::LogLevel::FAILURE:
		core_logger->critical(message);
		break;
	}
}

void Debug::log(std::string message, LogLevel level)
{
	switch (level)
	{
	case bm98::core::Debug::LogLevel::BASIC:
		game_logger->trace(message);
		break;
	case bm98::core::Debug::LogLevel::INFO:
		game_logger->info(message);
		break;
	case bm98::core::Debug::LogLevel::WARNING:
		game_logger->warn(message);
		break;
	case bm98::core::Debug::LogLevel::FAILURE:
		game_logger->critical(message);
		break;
	}
}

void Debug::mouse_position_display(sf::Font& font, sf::View* view)
{
	text_display.setPosition(Input::Instance()->mouse_position().x, Input::Instance()->mouse_position().y - 50);
	text_display.setFont(font);
	text_display.setCharacterSize(18);
	std::stringstream ss;
	ss << "(" << Input::Instance()->mouse_position(view).x << ", " << Input::Instance()->mouse_position(view).y << ")";
	text_display.setString(ss.str());

}
void Debug::mouse_position_display(sf::Font& font, sf::View* view, sf::Vector2f offset)
{
	text_display.setPosition(Input::Instance()->mouse_position().x,
		Input::Instance()->mouse_position().y - 50);
	text_display.setFont(font);
	text_display.setCharacterSize(18);
	std::stringstream ss;
	ss << "(" << Input::Instance()->mouse_position(view).x - offset.x << ", " <<
		Input::Instance()->mouse_position(view).y - offset.y << ")";
	text_display.setString(ss.str());
}

Debug::Debug()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	
	core_logger = spdlog::stdout_color_mt("CORE");
	core_logger->set_level(spdlog::level::trace);

	game_logger = spdlog::stdout_color_mt("GAME");
	game_logger->set_level(spdlog::level::trace);


}

}