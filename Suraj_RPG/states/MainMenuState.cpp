#include "../pch.h"
#include "MainMenuState.h"

#include "../core/Input.h"
#include "../core/Debug.h"
#include "../core/FileManager.h"
#include "../core/ResourceManager.h"
#include "../core/Game.h"
#include "../core/Physics.h"

#include "../GraphicsSettings.h"
#include "../gui/Gui.h"
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

	if (!music.openFromFile("Resources/Audio/Music/mainmenu.wav"))
		std::cout << "failure\n";

	music.setVolume(30.f * Game::get_game_settings().audio_settings.master_volume * Game::get_game_settings().audio_settings.music_volume);
	music.setVolume(0.0);
	music.setLoop(true);
	music.play();


}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::init_state()
{
	Debug::Instance()->core_log("[MainMenuState] Initialized", Debug::LogLevel::INFO);
	init_background();
	init_buttons();

	music.setVolume(30.f * Game::get_game_settings().audio_settings.master_volume * Game::get_game_settings().audio_settings.music_volume);
}

void MainMenuState::on_end_state()
{
	music.stop();
	Renderer::Instance()->remove_ui(&background);
	Debug::Instance()->core_log("[MainMenuState] Shutdown", Debug::LogLevel::INFO);

}

void MainMenuState::update_input()
{

	if (buttons["NEW_GAME"]->is_pressed())
	{
		//TODO: Add a new game state which options for creating a new game

		std::filesystem::remove_all("Saves");

		std::filesystem::create_directories("Saves/DefaultSave/Data/Scenes");
		std::filesystem::create_directory("Saves/DefaultSave/Backups");


		std::string path = "Data/Scenes/";
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string file_name = entry.path().string().substr(path.size(), entry.path().string().size() - 1);
			if (!entry.is_directory())
				FileManager::Instance()->save_to_file_styled(FileManager::Instance()->load_from_file(entry.path().string()), "Saves/DefaultSave/Data/Scenes/" + file_name);
		}
		on_end_state();
		states->push(new GameState(window, states, graphics_settings, "DefaultSave"));
		return;
	
	}
	if (buttons["LOAD_GAME"]->is_pressed())
	{
		//TODO: Add a load game state showing saved game files
		on_end_state();
		states->push(new GameState(window, states, graphics_settings, "DefaultSave"));
		return;
	}
	if (buttons["EDITOR"]->is_pressed())
	{
		//TODO: Add an editor state for dev purposes
		on_end_state();
		states->push(new EditorState(window, states, graphics_settings));
		return;
	}
	if (buttons["OPTIONS"]->is_pressed())
	{
		//TODO: Add an options state which gives players options to change things
		on_end_state();
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
}

#pragma endregion

#pragma region PROTECTED

void MainMenuState::init_variables()
{
	background.setSize((sf::Vector2f)window->getSize());
	background.setTexture(&ResourceManager::Instance()->get_texture("mainmenu_bg.png"));
}

void MainMenuState::init_background()
{
	Renderer::Instance()->add_ui(Renderer::RenderObject(&background, &background_renderable));
}

void MainMenuState::init_buttons()
{
	buttons["NEW_GAME"] = std::make_unique<GUI::Button>(Vector2f(400, 180), Vector2f(250, 100), &font,
														"New Game", 42, sf::Color(70, 70, 70, 255), sf::Color(20, 20, 20, 255),
														sf::Color(250, 250, 250, 255), true);
	buttons["LOAD_GAME"] = std::make_unique<GUI::Button>(Vector2f(400, 300), Vector2f(250, 100), &font,
														"Load Game", 42, sf::Color(70, 70, 70, 255), sf::Color(20, 20, 20, 255),
														sf::Color(250, 250, 250, 255), true);
	buttons["EDITOR"] = std::make_unique<GUI::Button>(Vector2f(400, 420), Vector2f(250, 100), &font,
														 "Editor", 42, sf::Color(70, 70, 70, 255), sf::Color(20, 20, 20, 255),
														 sf::Color(250, 250, 250, 255), true);
	buttons["OPTIONS"] = std::make_unique<GUI::Button>(Vector2f(400, 540), Vector2f(250, 100), &font,
													  "Options", 42, sf::Color(70, 70, 70, 255), sf::Color(20, 20, 20, 255),
													  sf::Color(250, 250, 250, 255), true);
	buttons["QUIT"] = std::make_unique<GUI::Button>(Vector2f(400, 660), Vector2f(250, 100), &font,
													   "Quit", 42, sf::Color(70, 70, 70, 255), sf::Color(20, 20, 20, 255),
													   sf::Color(250, 250, 250, 255), true);

}

#pragma endregion
}