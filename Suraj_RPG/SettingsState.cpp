#include "pch.h"
#include "SettingsState.h"

#include "core/Input.h"
#include "core/Time.h"
#include "core/Debug.h"
#include "core/Physics.h"
#include "core/Game.h"
#include "core/ResourceManager.h"

#include "GraphicsSettings.h"
#include "Gui.h"
namespace bm98
{
using namespace core;
SettingsState::SettingsState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	: State(window, states, graphics_settings)
{
	state_name = "Settings_State";
	init_variables();
	init_fonts();
	init_text();
	init_gui();


	//Debug::init();
	Renderer::Instance()->add(Renderer::RenderObject(&options_text, _render, options_layer, z_order));
	Renderer::Instance()->add(Renderer::RenderObject(&circle, _render, options_layer, z_order));
	Renderer::Instance()->add(Renderer::RenderObject(&square, _render, options_layer, z_order));

	Renderer::Instance()->add(Renderer::RenderObject(&concave, _render, options_layer, z_order));

	pb = std::make_unique<GUI::ProgressBar>();
	pb->set_position(Vector2f(800, 300));
	pb->set_size(Vector2f(300, 200));
	pb->set_percentage(.75f);
	pb->set_color(sf::Color::Magenta);

}

SettingsState::~SettingsState()
{
}

void SettingsState::init_state()
{
	Debug::Instance()->core_log("[SettingsState] Initialized", Debug::LogLevel::INFO);
}

void SettingsState::init_variables()
{
	modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::update_input()
{
	if (Input::Instance()->get_action_down("L"))
		pb->set_percentage(.5f);

	float delta = Time::Instance()->delta_time();
	if (Input::Instance()->get_action("W"))
		square.move(0, -100 * delta);
	if (Input::Instance()->get_action("A"))
		square.move(-100 * delta, 0);
	if (Input::Instance()->get_action("D"))
		square.move(100 * delta, 0);
	if (Input::Instance()->get_action("S"))
		square.move(0, 100 * delta);

	if (Input::Instance()->get_mouse_down(Input::Mouse::RIGHT))
		concave.rotate(10);

	if (buttons["BACK"]->is_pressed())
	{
		isRequestingQuit = true;
	}
	if (buttons["APPLY"]->is_pressed())
	{
		graphics_settings->resolution = modes[drop_downs["RESOLUTION"]->get_selected_index()];
		window->create(graphics_settings->resolution, graphics_settings->game_title, sf::Style::Default);
	}

	if (Input::Instance()->get_mouse_down())
	{
		if(square.contains_point(Input::Instance()->mouse_position()))
		{
			holding = true;
			offset = square.get_position()-Input::Instance()->mouse_position();
		}
	}
	if (Input::Instance()->get_mouse_up())
	{
		holding = false;
	}

}

void SettingsState::on_end_state()
{
	Debug::Instance()->core_log("[SettingsState] Shutdown", Debug::LogLevel::INFO);
}

void SettingsState::update()
{
	State::update();
	update_input();
	Debug::Instance()->mouse_position_display(font);


	if (holding)
	{
		square.set_position(Input::Instance()->mouse_position() + offset);
	}


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

void SettingsState::update_sfml(sf::Event sfEvent)
{
}

void SettingsState::late_update()
{

}

void SettingsState::fixed_update()
{
	Vector2f dif;
	dif = FloatConvex::Intersection(square, concave);

	if (dif != Vector2f::Infinity())
	{
		square.move(dif.x, dif.y);
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
	buttons["BACK"] = std::make_unique<GUI::Button>(100.f, 540.f, 150.f, 60.f, &font, "BACK", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
	buttons["APPLY"] = std::make_unique<GUI::Button>(850.f, 540.f, 150.f, 60.f, &font, "APPLY", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
}

void SettingsState::init_volume_display()
{
	volume_sliders["MASTER"] = std::make_unique<GUI::Slider>(700, 400, 400, 0, 100, 100, true);
	volume_sliders["MUSIC"] = std::make_unique<GUI::Slider>(700, 500, 400, 0, 100, 100, true);
	volume_sliders["SOUND"] = std::make_unique<GUI::Slider>(700, 600, 400, 0, 100, 100, true);
	volume_sliders["AMBIENT"] = std::make_unique<GUI::Slider>(700, 700, 400, 0, 100, 100, true);

	volume_displays["MASTER"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 400, 22, font, "MASTER", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("MASTER")->get_width() + 10, 400, 22, font, "100%", sf::Color::White));

	volume_displays["MUSIC"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 500, 22, font, "MUSIC", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("MUSIC")->get_width() + 10, 500, 22, font, "100%", sf::Color::White));
	volume_displays["SOUND"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 600, 22, font, "SOUND", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("SOUND")->get_width() + 10, 600, 22, font, "100%", sf::Color::White));
	volume_displays["AMBIENT"] = std::pair<std::unique_ptr<GUI::Label>, std::unique_ptr<GUI::Label>>(std::make_unique<GUI::Label>(650, 700, 22, font, "AMBIENT", sf::Color::White),
		std::make_unique<GUI::Label>(700 + volume_sliders.at("AMBIENT")->get_width() + 10, 700, 22, font, "100%", sf::Color::White));

}

void SettingsState::init_drop_downs()
{
	std::vector<std::string> modes_str;
	for (auto& i : modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	drop_downs["RESOLUTION"] = std::make_unique<GUI::DropDownList>(100, 100, 150, 30, font,
		modes_str);

	//drop_downs["FULLSCREEN"] = new GUI::DropDownList(800, 175, 150, 30, font, screen_list, 2);
}
}