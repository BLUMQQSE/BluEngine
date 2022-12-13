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

GameState::GameState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	: State(window, states, graphics_settings)
{
	state_name = "Game_State";
	init_fonts();
	init_players();
	init_view();
	active_scene = new Scene("Dafuq.json");
	SceneManager::init(active_scene);
	//SceneManager::load_scene("test.json");

	pmenu = new PauseMenu(*window, font);
	pmenu->add_button("QUIT", 900.f, 900.f, "Quit Game");
	particles = new ParticleSystem(1000);
	particles->set_emitter(sf::Vector2f(200, 200));
}

GameState::~GameState()
{
	FileManager::save_to_file_styled(component_player->serialize_json(), "Data/DontDestroyObjects/player.json");
	delete component_player;
	
}

void GameState::init_state()
{
	Debug::Log("init state called");
	//Physics::add_to_physics(player);
}

void GameState::on_end_state()
{
	Debug::Log("Will now clean up game state on exit");
	SceneManager::save_scene();
	Physics::remove_from_physics(component_player);
	State::on_end_state();

}

void GameState::update_input()
{

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
	}
}

void GameState::update()
{
	GameClock::update();
	SceneManager::update();
	update_input();
	if (!paused)
	{
		State::update();
		//player->update(); 
		component_player->update();
		active_scene->update();

		//view.setCenter(player->get_transform().position);
		view.setCenter(component_player->transform.position);
		particles->update();
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
	//player->fixed_update();
	component_player->fixed_update();
	active_scene->fixed_update();
}

void GameState::late_update()
{
	//player->late_update();
	active_scene->late_update();

}

void GameState::render()
{
	//player->add_to_buffer(&this->view);
	component_player->add_to_buffer(&this->view);
	active_scene->render(&this->view);
	Renderer::add(Renderer::RenderObject(particles, SortingLayer::EFFECTS, 0, &this->view));

	if (paused)
	{
		pmenu->add_to_buffer();
	}
}

void GameState::init_players()
{
	//player = new Player(0.f, 200.f);
	//init_components doing some interesting stuff
	//player->init_components();

	
	component_player = new GameObject();
	
	
	//component_player->add_component<PlayerController>();
	//component_player->add_component<SpriteComponent>("Player/player_sprite_sheet.png");
	//component_player->add_component<RigidbodyComponent>();
	//component_player->add_component<BoxColliderComponent>();
	//component_player->add_component<AnimationComponent>();

	
	
	component_player->unserialize_json(FileManager::load_from_file("Data/DontDestroyObjects/player.json", true));
	
	component_player->init();
	component_player->awake();
	component_player->start();

	Physics::add_to_physics(component_player);
	/*
	Dummy* root = new Dummy(0, 0);
	Dummy* dummy_parent = new Dummy(0, 110);
	Dummy* dummy = new Dummy(0,100);

	Dummy* dif_group_root = new Dummy(0, 0);
	Dummy* dif_group_child = new Dummy(0, 0);
	dif_group_child->name = "dif group child";
	dif_group_root->name = "dif group root";
	dif_group_child->set_parent(dif_group_root);

	Dummy* dif2_group_root = new Dummy(0, 0);
	Dummy* dif2_group_child = new Dummy(0, 0);
	dif2_group_child->name = "extra1";
	dif2_group_root->name = "extra2";
	dif2_group_child->set_parent(dif2_group_root);

	dummy->set_parent(dummy_parent);
	dummy_parent->name = "middle";

	dummy->name = "child";
	player->add_child(dummy);
	//dummy->set_parent(player);
	//dummy->add_child_animation_component();

	dummy_parent->set_parent(root);
	root->name = "root";

	objects.push_back(dif_group_root);
	objects.push_back(dummy);
	objects.push_back(root);
	objects.push_back(dif_group_child);
	objects.push_back(dif2_group_root);
	objects.push_back(dif2_group_child);

	objects.push_back(dummy_parent);

	insert_sort();
	*/

}

void GameState::init_view()
{
	view.setSize(graphics_settings->resolution.width, graphics_settings->resolution.height);
	view.setCenter(graphics_settings->resolution.width / 2.f, graphics_settings->resolution.height / 2.f);
	view.zoom(.6f);
}

}