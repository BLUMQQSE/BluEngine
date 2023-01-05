#include "pch.h"
#include "GameState.h"
#include "GraphicsSettings.h"
#include "Input.h"
#include "Debug.h"
#include "Gui.h"
#include "Physics.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameClock.h"
#include "PauseMenu.h"
#include "ParticleSystem.h"
#include "SceneManager.h"
#include "FileManager.h"

#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "PlayerController.h"

namespace bm98
{
using namespace core;

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings,
	std::string game_save_name)
	: State(window, states, graphics_settings)
{
	state_name = "Game_State";

	FileManager::set_save_name("Saves/" +game_save_name + "/");

	unserialize_json(FileManager::load_from_file(FileManager::get_save_name() + "gamestate.json"));

	active_scene = new Scene(active_scene_name);
	SceneManager::init(active_scene);
	SceneManager::load_scene(active_scene_name);
	//load in dont destroy objects on load
	// 
	std::string n = active_scene->get_name();
	active_scene->unserialize_json(FileManager::load_from_file(FileManager::get_save_name() + "Data/Scenes/dont_destroy_on_load_objects.json"));
	active_scene->set_name(n);

	active_scene->init();

	init_fonts();
	init_view();

	pmenu = new PauseMenu(*window, font);
	pmenu->add_button("QUIT", 900.f, 900.f, "Quit Game");
}

GameState::~GameState()
{
	delete pmenu;
	delete view;
	delete active_scene;
	active_scene = nullptr;
}

void GameState::init_state()
{
}

void GameState::on_end_state()
{
	Debug::Log("Will now clean up game state on exit");
	//somewhere I am losing player before I even resave scene
	SceneManager::save_scene(true); 
	FileManager::save_to_file_styled(serialize_json(), FileManager::get_save_name()+"gamestate.json");
	SceneManager::destroy();

	State::on_end_state();
}

void GameState::update_input()
{

	if (Input::get_mouse_down(Input::Mouse::RIGHT))
		SceneManager::load_scene("test.json");

	if (Input::get_action_down("ESCAPE"))
	{
		isRequestingQuit = true;
	}
	if (Input::get_action_down("MENU"))
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
	GameClock::update();
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
	obj["active-scene-name"] = SceneManager::get_active_scene_name();
	return obj;
}

void GameState::unserialize_json(Json::Value obj)
{
	if (obj == Json::Value::null)
	{
		active_scene_name = SceneManager::get_default_scene();

		FileManager::save_to_file_styled(serialize_json(), FileManager::get_save_name() + "/gamestate.json");

		return;
	}
	active_scene_name = obj["active-scene-name"].asString();
}

void GameState::init_view()
{
	view = new sf::View();
	view->setSize(graphics_settings->resolution.width, graphics_settings->resolution.height);
	view->setCenter(graphics_settings->resolution.width / 2.f, graphics_settings->resolution.height / 2.f);
	view->zoom(.6f);
}

}