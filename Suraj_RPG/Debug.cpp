#include "pch.h"
#include "Debug.h"
namespace bm98::core
{
sf::Text Debug::text_display;
bool Debug::active = true;
SortingLayer Debug::layer = SortingLayer::UI;
unsigned char Debug::z_order = 100;

void Debug::Log(std::string message)
{
	std::cout << message << "\n";
}

void Debug::LogError(std::string message)
{
	std::cout << message << "\n";
}

void Debug::mouse_position_display(sf::Font& font, sf::View* view)
{
	text_display.setPosition(Input::mouse_position().x, Input::mouse_position().y - 50);
	text_display.setFont(font);
	text_display.setCharacterSize(18);
	std::stringstream ss;
	ss << "(" << Input::mouse_position(view).x << ", " << Input::mouse_position(view).y << ")";
	text_display.setString(ss.str());
	Renderer::add(Renderer::RenderObject(&text_display, active, layer, z_order));
}
void Debug::mouse_position_display(sf::Font& font, sf::View* view, sf::Vector2f offset)
{
	text_display.setPosition(Input::mouse_position().x, Input::mouse_position().y - 50);
	text_display.setFont(font);
	text_display.setCharacterSize(18);
	std::stringstream ss;
	ss << "(" << Input::mouse_position(view).x - offset.x << ", " << Input::mouse_position(view).y - offset.y << ")";
	text_display.setString(ss.str());
	Renderer::add(Renderer::RenderObject(&text_display, active, layer, z_order));
}

}