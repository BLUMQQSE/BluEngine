#include "pch.h"
#include "Debug.h"
namespace bm98::core
{
sf::Text Debug::text_display;

void Debug::Log(std::string message)
{
	std::cout << message << "\n";
}

void Debug::LogError(std::string message)
{
	std::cout << message << "\n";
}

void Debug::mouse_position_display(sf::Font& font)
{
	text_display.setPosition(Input::mouse_position().x, Input::mouse_position().y - 50);
	text_display.setFont(font);
	text_display.setCharacterSize(12);
	std::stringstream ss;
	ss << "(" << Input::mouse_position().x << ", " << Input::mouse_position().y << ")";
	text_display.setString(ss.str());
	//Renderer::add(RenderObject(&text_display, SortingLayer::UI, 1));
}
}