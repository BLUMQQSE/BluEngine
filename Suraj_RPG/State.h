#pragma once
#include "Gui.h"
namespace bm98
{
class GraphicsSettings;

class State
{

public:
	State(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings);
	virtual ~State();

	/// <summary>
	/// Handles cleaning up state just prior to destroying the state from
	/// the game stack;
	/// </summary>
	virtual void on_end_state();

	virtual void pause_state();
	virtual void unpause_state();

	const bool& get_quit() const;

	virtual void init_state();
	virtual void init_variables();

	virtual void update_input() = 0;

	virtual void update_sfml(sf::Event sfEvent);
	virtual void update() = 0;
	virtual void fixed_update();
	virtual void late_update();

	virtual void render() = 0;
	std::string get_state_name();

protected:
	std::string state_name;
	std::stack<State*>* states;
	sf::RenderWindow* window;
	GraphicsSettings* graphics_settings;
	bool isRequestingQuit;
	float grid_size;

	bool paused;

	sf::Font font;
	std::map<std::string, sf::Texture> textures;

	/// <summary>
	/// Assigns keybinds map to sf::Keyboard::Key(s) from file_path.
	/// </summary>
	/// <param name="file_path">Path to file containing keybinds.</param>
	void init_fonts_from_file(std::string file_path);
	virtual void init_fonts();
	virtual void init_buttons();


};

}