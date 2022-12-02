#pragma once
#include "Gui.h"
namespace bm98
{
class PauseMenu
{
public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~PauseMenu();

	std::map<std::string, GUI::Button*>& get_buttons();

	void add_button(const std::string key, float x, float y,
		const std::string text);
	const bool is_button_pressed(const std::string key);
	void update();

	void add_to_buffer();


private:

	sf::Font& font;
	sf::Text menu_text;
	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, GUI::Button*> buttons;

};

}