#include "pch.h"
#include "SettingsState.h"
#include "GraphicsSettings.h"
#include "Input.h"
#include "Debug.h"
#include "Gui.h"
#include "Physics.h"
namespace bm98
{
using namespace core;
SettingsState::SettingsState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	: State(window, states, graphics_settings)
{
	Renderer::clear();
	state_name = "Settings_State";
	init_variables();
	init_fonts();
	init_text();
	init_gui();

}

SettingsState::~SettingsState()
{
	Renderer::clear();
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != buttons.end(); ++it)
	{
		delete it->second;
	}

	auto it2 = this->drop_downs.begin();
	for (it2 = this->drop_downs.begin(); it2 != drop_downs.end(); ++it2)
	{
		delete it2->second;
	}
}

void SettingsState::init_variables()
{
	modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::update_input()
{
	if (buttons["BACK"]->is_pressed())
	{
		isRequestingQuit = true;
	}
	if (buttons["APPLY"]->is_pressed())
	{
		graphics_settings->resolution = modes[drop_downs["RESOLUTION"]->get_selected_index()];
		window->create(graphics_settings->resolution, graphics_settings->game_title, sf::Style::Default);
	}
}

void SettingsState::on_end_state()
{
	Debug::Log("Will now clean up settings state on exit");
}

void SettingsState::update()
{
	State::update();
	update_input();
	Debug::mouse_position_display(font);
	for (auto& it : buttons)
	{
		it.second->update();

	}
	for (auto& it2 : drop_downs)
	{
		it2.second->update();

	}
}

void SettingsState::late_update()
{
}

void SettingsState::fixed_update()
{
}

void SettingsState::render()
{
	for (auto& it : buttons)
	{
		it.second->add_to_buffer();
	}
	for (auto& it2 : drop_downs)
	{
		it2.second->add_to_buffer();
	}
	Renderer::add(Renderer::RenderObject(&options_text, _render, options_layer, z_order));

}

void SettingsState::init_gui()
{
	init_buttons();
	init_drop_downs();
}

void SettingsState::init_text()
{
	options_text.setFont(font);
	options_text.setPosition(sf::Vector2f(100, 100));
	options_text.setCharacterSize(30);
	options_text.setFillColor(sf::Color(255, 255, 255, 200));

	options_text.setString(
		"Resolution \n\nFullscreen \n\nVsync \n\nAntialiasing \n"
	);
}

void SettingsState::init_buttons()
{
	buttons["BACK"] = new GUI::Button(100.f, 540.f, 150.f, 60.f, &font, "BACK", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
	buttons["APPLY"] = new GUI::Button(850.f, 540.f, 150.f, 60.f, &font, "APPLY", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
}

void SettingsState::init_drop_downs()
{
	std::vector<std::string> modes_str;
	for (auto& i : modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	drop_downs["RESOLUTION"] = new GUI::DropDownList(600, 100, 150, 30, font,
		modes_str);

	//drop_downs["FULLSCREEN"] = new GUI::DropDownList(800, 175, 150, 30, font, screen_list, 2);
}
}