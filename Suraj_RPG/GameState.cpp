#include "pch.h"
#include "GameState.h"

#include "core/Input.h"
#include "core/Debug.h"
#include "core/SceneManager.h"
#include "core/FileManager.h"

#include "core/DialogueSystem.h"

#include "GraphicsSettings.h"
#include "Gui.h"
#include "Scene.h"
#include "GameClock.h"
#include "PauseMenu.h"
#include "ParticleSystem.h"
#include "GameEditorView.h"
#include "Dialogue.h"

namespace bm98
{
using namespace core;

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings,
	std::string game_save_name)
	: State(window, states, graphics_settings)
{
	state_name = "Game_State";

	FileManager::Instance()->set_save_name("Saves/" +game_save_name + "/");

	unserialize_json(FileManager::Instance()->load_from_file(
		FileManager::Instance()->get_save_name() + "gamestate.json"));

	//active_scene = new Scene(active_scene_name);
	active_scene = std::make_unique<Scene>(active_scene_name);

	//SceneManager::Instance()->init(active_scene);
	EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENEMANAGER_INITIALIZE_, (void*)active_scene.get());
	
	//load in dont destroy objects on load
	// 
	std::string n = active_scene->get_file_name();

	active_scene->unserialize_json(FileManager::Instance()->load_from_file(FileManager::Instance()->get_save_name()
		+ "Data/Scenes/dont_destroy_on_load_objects.json"));
	
	SceneManager::Instance()->load_scene(active_scene_name);
	active_scene->set_file_name(n);

	//active_scene->init();

	init_fonts();
	init_view();
	init_singletons();

	//active_scene->set_view(view);

	pmenu = std::make_unique<PauseMenu>(*window, font);
	pmenu->add_button("QUIT", 500.f, 500.f, "Quit Game");

	editor_view = std::make_unique<GameEditorView>();
	//Time::Instance()->set_time_scale(5000.f);
	test_dialogue = new Dialogue("Sample_Dialogue.json");


}

GameState::~GameState()
{
	delete view;

	active_scene = nullptr;
	delete test_dialogue;
}

void GameState::init_state()
{
	Debug::Instance()->core_log("[GameState] Initialized", Debug::LogLevel::INFO);
}

void GameState::on_end_state()
{
	Debug::Instance()->core_log("[GameState] Shutdown", Debug::LogLevel::INFO);
	SceneManager::Instance()->save_scene(true);
	FileManager::Instance()->save_to_file_styled(serialize_json(),
		FileManager::Instance()->get_save_name()+"gamestate.json");
	//SceneManager::Instance()->destroy();
	EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENEMANAGER_DESTROY_);
	State::on_end_state();
}

void GameState::update_input()
{
	if (Input::Instance()->get_action_down("TILDE"))
		editor_view->toggle_editor(EditorPanel::ALL);

	if (Input::Instance()->get_action_down("ESCAPE"))
	{
		if (!paused) 
			pause_state();
		else
			unpause_state();
		pmenu->set_render(paused);
	}
}

void GameState::pause_state()
{
	State::pause_state();
	pmenu->open();
}

void GameState::unpause_state()
{
	State::unpause_state();
	pmenu->close();
}

void GameState::update()
{
	if (Input::Instance()->get_mouse_down(Input::Mouse::RIGHT))
	{
		if (!DialogueSystem::Instance()->is_open())
			DialogueSystem::Instance()->open_dialogue(*test_dialogue);
		else
			DialogueSystem::Instance()->close_dialogue();
	}
	//update editor first in case event occured last frame
	editor_view->update();

	GameClock::Instance()->update();

	update_input();
	if (!paused)
	{
		State::update();
		active_scene->update();
		view->setCenter(sf::Vector2f(0,0));
	}
	else
	{
		pmenu->update();
		if (pmenu->is_button_pressed("QUIT"))
			isRequestingQuit = true;
	}
}

void GameState::update_sfml(sf::Event sfEvent)
{
	editor_view->update_sfml(sfEvent);
}

void GameState::fixed_update()
{
	State::fixed_update();


	active_scene->fixed_update();
}

void GameState::late_update()
{
	State::late_update();
	active_scene->late_update();
}

void GameState::render()
{
	active_scene->render(this->view);
}

Json::Value GameState::serialize_json()
{
	Json::Value obj;
	obj["active-scene-name"] = SceneManager::Instance()->get_active_scene_name();
	return obj;
}

void GameState::unserialize_json(Json::Value obj)
{
	if (obj == Json::Value::null)
	{
		active_scene_name = SceneManager::Instance()->get_default_scene();

		FileManager::Instance()->save_to_file_styled(serialize_json(), FileManager::Instance()->get_save_name() + "/gamestate.json");

		return;
	}
	active_scene_name = obj["active-scene-name"].asString();
}

void GameState::init_view()
{
	view = new sf::View();
	view->setSize(graphics_settings->resolution.width, graphics_settings->resolution.height);
	view->setCenter(graphics_settings->resolution.width / 2.f, graphics_settings->resolution.height / 2.f);
	view->zoom(1.f);
}

void GameState::init_singletons()
{
	GameClock::Instance()->init();
	GUI::GameClockDisplay::Instance()->init();
}

}