#pragma once
#include "globals.h"
#include "Input.h"
#include "Renderer.h"
namespace bm98::core
{
class Debug
{
public:

	static void init();
	/// <summary>
	/// Log provided string to the console.
	/// </summary>
	/// <param name="message"></param>
	static void Log(std::string message);
	static void LogError(std::string message);
	static void mouse_position_display(sf::Font& font, sf::View* view = nullptr);
	static void mouse_position_display(sf::Font& font, sf::View* view,
		sf::Vector2f offset);

private:
	static sf::Text text_display;
	static bool active;
	static SortingLayer layer;
	static char z_order;
};
}
