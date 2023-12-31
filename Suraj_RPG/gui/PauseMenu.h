#pragma once
#include "Gui.h"
#include "../IRenderable.h"
namespace bm98
{
class PauseMenu : public IRenderable
{
public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~PauseMenu();

	std::map<std::string, std::weak_ptr<GUI::Button>> get_buttons();

	void add_button(const std::string key, float x, float y,
		const std::string text);
	const bool is_button_pressed(const std::string key);
	void update();

	void add_to_buffer();
	void close();
	void open();


private:

	sf::Font& font;
	sf::Text menu_text;
	sf::RectangleShape background;
	sf::RectangleShape container;

	float previous_time_scale;

	std::map<std::string, std::shared_ptr<GUI::Button>> buttons;

};

}