#include "pch.h"
#include "MainMenuState.h"
#include "GraphicsSettings.h"
#include "Input.h"
#include "Debug.h"
#include "Gui.h"
#include "Physics.h"
#include "GameState.h"
#include "EditorState.h"
#include "SettingsState.h"
namespace bm98
{
using namespace core;
#pragma region PUBLIC

MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	: State(window, states, graphics_settings)
{
	state_name = "Main_Menu_State";
	init_variables();
	init_background();
	init_fonts();
	init_buttons();

}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	delete_buttons();
}

void MainMenuState::init_state()
{
	init_background();
	init_buttons();
}

void MainMenuState::on_end_state()
{
	Debug::Log("Will now clean up main menu state on exit");
	delete_buttons();
}

void MainMenuState::update_input()
{
	if (buttons["NEW_GAME"]->is_pressed())
	{
		//TODO: Add a new game state which options for creating
		//a new game
		on_end_state();
		states->push(new GameState(window, states, graphics_settings));
		return;
	}
	if (buttons["LOAD_GAME"]->is_pressed())
	{
		//TODO: Add a load game state showing saved game files
		return;
	}
	if (buttons["EDITOR"]->is_pressed())
	{
		//TODO: Add an editor state for dev purposes
		states->push(new EditorState(window, states, graphics_settings));
		return;
	}
	if (buttons["OPTIONS"]->is_pressed())
	{
		//TODO: Add an options state which gives players options to change things
		states->push(new SettingsState(window, states, graphics_settings));
		return;
	}
	if (buttons["QUIT"]->is_pressed())
	{
		isRequestingQuit = true;
		return;
	}
}

void MainMenuState::update()
{
	State::update();

	update_input();
	for (auto& it : buttons)
	{
		it.second->update();

	}
}

void MainMenuState::render()
{
	for (auto& it : buttons)
	{
		it.second->add_to_buffer();
	}
	//Renderer::add(Renderer::RenderObject(&background, SortingLayer::BACKGROUND));
}

#pragma endregion

#pragma region PROTECTED

void MainMenuState::init_variables()
{
	background.setSize((sf::Vector2f)window->getSize());
	if (!background_texture.loadFromFile("Resources/Images/Backgrounds/mainmenu_bg.png"))
	{
		throw("ERROR::MAIN_MENU_STATE::COULD NOT LOAD BACKGROUND TEXTURE");
	}
	background.setTexture(&background_texture);
}

void MainMenuState::init_background()
{
	Renderer::add(Renderer::RenderObject(&background, _render, background_layer, z_order));
}

void MainMenuState::init_buttons()
{
	buttons["NEW_GAME"] = new GUI::Button(800.f, 380.f, 250.f, 100.f, &font, "New Game", 42,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	buttons["LOAD_GAME"] = new GUI::Button(800.f, 500.f, 250.f, 100.f, &font, "Load Game", 42,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	buttons["EDITOR"] = new GUI::Button(800.f, 620.f, 250.f, 100.f, &font, "Editor", 42,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	buttons["OPTIONS"] = new GUI::Button(800.f, 740.f, 250.f, 100.f, &font, "Options", 42,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

	buttons["QUIT"] = new GUI::Button(800.f, 860.f, 250.f, 100.f, &font, "QUIT", 42,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

}

void MainMenuState::delete_buttons()
{
	for (auto& b : buttons)
	{
		delete b.second;
	}
	buttons.clear();
}

/*
void MainMenuState::init_fonts()
{
	init_fonts_from_file("Fonts/greek_futura_ldr.ttf");
}
*/
#pragma endregion
}