#include "pch.h"
#include "Debug.h"
namespace bm98::core
{
sf::Text Debug::text_display;
bool Debug::active = true;
SortingLayer Debug::layer = SortingLayer::UI;
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

}