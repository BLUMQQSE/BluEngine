#include "pch.h"
#include "SettingsState.h"
#include "GraphicsSettings.h"
#include "Input.h"
#include "Time.h"
#include "Debug.h"
#include "Gui.h"
#include "Physics.h"
#include "Game.h"
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
	Debug::init();
	Renderer::add(Renderer::RenderObject(&options_text, _render, options_layer, z_order));
	Renderer::add(Renderer::RenderObject(&circle, _render, options_layer, z_order));
	Renderer::add(Renderer::RenderObject(&circle2, _render, options_layer, z_order));

	Renderer::add(Renderer::RenderObject(&concave, _render, options_layer, z_order));

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

	for (auto& it3 : volume_sliders)
		delete it3.second;

	for (auto v : volume_displays)
	{
		delete v.second.first;
		delete v.second.second;
	}

}

void SettingsState::init_variables()
{
	modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::update_input()
{
	float delta = Time::delta_time();
	if (Input::get_action("W"))
		circle.move(0, -100 * delta);
	if (Input::get_action("A"))
		circle.move(-100 * delta, 0);
	if (Input::get_action("D"))
		circle.move(100 * delta, 0);
	if (Input::get_action("S"))
		circle.move(0, 100 * delta);

	if (Input::get_mouse_down(Input::Mouse::RIGHT))
		concave.set_rotation(30);

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
	for (auto& it3 : volume_sliders)
		it3.second->update();

	Game::get_game_settings().audio_settings.master_volume = volume_sliders.at("MASTER")->get_value() / 100.f;
	Game::get_game_settings().audio_settings.music_volume = volume_sliders.at("MUSIC")->get_value() / 100.f;
	Game::get_game_settings().audio_settings.sound_volume = volume_sliders.at("SOUND")->get_value() / 100.f;
	Game::get_game_settings().audio_settings.ambient_volume = volume_sliders.at("AMBIENT")->get_value() / 100.f;

	for (auto& v : volume_displays)
		v.second.second->set_text(std::to_string(static_cast<int>(std::round(volume_sliders.at(v.first)->get_value()))) + "%");
}

void SettingsState::late_update()
{
	
}

void SettingsState::fixed_update()
{
	Vector2f dif = FloatConvex::intersection(circle, concave);
	
	if (dif != Vector2f::infinity())
	{
		circle.move(dif.x, dif.y);

		std::cout << "collision\n";
	}
}

void SettingsState::render()
{
}

void SettingsState::init_gui()
{
	init_buttons();
	init_drop_downs();
	init_volume_display();
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

void SettingsState::init_volume_display()
{
	volume_sliders["MASTER"] = new GUI::Slider(700, 400, 400, 0, 100, 100, true);
	volume_sliders["MUSIC"] = new GUI::Slider(700, 500, 400, 0, 100, 100, true);
	volume_sliders["SOUND"] = new GUI::Slider(700, 600, 400, 0, 100, 100, true);
	volume_sliders["AMBIENT"] = new GUI::Slider(700, 700, 400, 0, 100, 100, true);

	volume_displays["MASTER"] = std::pair<GUI::Label*, GUI::Label*>(new GUI::Label(650, 400, 22, font, "MASTER", sf::Color::White),
		new GUI::Label(700 + volume_sliders.at("MASTER")->get_width() + 10, 400, 22, font, "100%", sf::Color::White));
	volume_displays["MUSIC"] = std::pair<GUI::Label*, GUI::Label*>(new GUI::Label(650, 500, 22, font, "MUSIC", sf::Color::White),
		new GUI::Label(700 + volume_sliders.at("MUSIC")->get_width() + 10, 500, 22, font, "100%", sf::Color::White));
	volume_displays["SOUND"] = std::pair<GUI::Label*, GUI::Label*>(new GUI::Label(650, 600, 22, font, "SOUND", sf::Color::White),
		new GUI::Label(700 + volume_sliders.at("SOUND")->get_width() + 10, 600, 22, font, "100%", sf::Color::White));
	volume_displays["AMBIENT"] = std::pair<GUI::Label*, GUI::Label*>(new GUI::Label(650, 700, 22, font, "AMBIENT", sf::Color::White),
		new GUI::Label(700 + volume_sliders.at("AMBIENT")->get_width() + 10, 700, 22, font, "100%", sf::Color::White));

}

void SettingsState::init_drop_downs()
{
	std::vector<std::string> modes_str;
	for (auto& i : modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	drop_downs["RESOLUTION"] = new GUI::DropDownList(900, 100, 150, 30, font,
		modes_str);

	//drop_downs["FULLSCREEN"] = new GUI::DropDownList(800, 175, 150, 30, font, screen_list, 2);
}
}