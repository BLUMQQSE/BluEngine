#include "pch.h"
#include "EditorState.h"
#include "Input.h"
#include "GraphicsSettings.h"
#include "Input.h"
#include "Debug.h"
#include "Gui.h"
#include "Physics.h"
#include "PauseMenu.h"
#include "TilemapComponent.h"
#include "TilemapColliderComponent.h"
#include "Time.h"
#include "GameObject.h"
#include "SceneEditorView.h"
#include "PrefabEditor.h"
#include "FileManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteComponent.h"
namespace bm98
{
using namespace core;
#pragma region PUBLIC

EditorState::EditorState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	:State(window, states, graphics_settings)
{
	state_name = "Editor_State";

	init_variables();
	init_views();
	init_fonts();
	init_buttons();

	active_scene = new Scene("editor-test-scene.json");

	EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENEMANAGER_INITIALIZE_, static_cast<void*>(active_scene));
	//SceneManager::Instance()->init(active_scene);
	SceneManager::Instance()->load_scene_prefab("editor-test-scene.json");

	active_scene->set_view(scene_view);

	std::vector<GameObject*> obj = SceneManager::Instance()->get_objects_in_scene();

	for (std::size_t i = 0; i < obj.size(); i++)
		if (obj[i]->has_component<SpriteComponent>())
			obj[i]->get_component<SpriteComponent>().set_view(scene_view);

	pmenu = new PauseMenu(*window, font);
	pmenu->add_button("BACK", 500.f, 500.f, "Back");
	pmenu->add_button("SAVE", 500, 440, "Save");
	pmenu->add_button("LOAD", 500, 380, "Load");
	
	init_tilemap();
	init_gui();
	
	//texture_selector->toggle_hidden();

	camera_move_speed = 120;


	editor_view = new SceneEditorView();

	Renderer::Instance()->add(Renderer::RenderObject(&sidebar, outline_render, outline_layer, z_order));
	Renderer::Instance()->add(Renderer::RenderObject(&selector_rect, text_select_render, text_select_layer, z_order, &scene_view));
}

EditorState::~EditorState()
{
	Renderer::Instance()->remove(&sidebar);
	Renderer::Instance()->remove(&selector_rect);

	delete scene_view;
	delete active_scene;

	delete selected_gameobject;
	delete texture_selector;
	delete editor_view;
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != buttons.end(); ++it)
	{
		delete it->second;
	}
}

void EditorState::init_state()
{
	Debug::Instance()->core_log("ENTERING EditorState", Debug::LogLevel::INFO);
}

void EditorState::on_end_state()
{
	Debug::Instance()->core_log("EXITING EditorState", Debug::LogLevel::INFO);

	FileManager::Instance()->save_to_file_styled(selected_gameobject->serialize_json(), "tilemap_test.json");

	EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENEMANAGER_DESTROY_);
	State::on_end_state();
}

void EditorState::update_input()
{
	if (Input::Instance()->using_input_box())
		return;

	if (Input::Instance()->get_action_down("V"))
	{
		selected_gameobject->serialize_json();
	}

	if (Input::Instance()->get_action_down("MENU"))
	{
		std::cout << "MENU\n";
		if (!paused)
			pause_state();
		else
			unpause_state();
	}
	/*
	if (Input::Instance()->get_action_down("SPACE"))
	{
		FileManager::Instance()->save_to_file_styled(selected_gameobject->get_component<TilemapComponent>().serialize_json(),
			"test_file.json");

	}
	*/
}

void EditorState::update_sfml(sf::Event sfEvent)
{
	editor_view->update_sfml(sfEvent);
}

void EditorState::update()
{
	update_input();
	editor_view->update();
	if (!paused)
	{
		State::update();
		if(selected_gameobject)
			selected_gameobject->update();

		for (auto& it : buttons)
		{
			it.second->update();
		}
		if (Input::Instance()->get_action_down("TS_MENU"))
			texture_selector->toggle_hidden();
		
		update_gui();
	}
	else
	{
		pmenu->update();
		if (pmenu->is_button_pressed("BACK"))
			isRequestingQuit = true;
		if (pmenu->is_button_pressed("SAVE"))
		{
			//tilemap->save_to_json("Data/Tilemaps/test.json");
		}
		//tilemap->save_to_file("test.tm");
		if (pmenu->is_button_pressed("LOAD"))
		{
			//tilemap->load_from_file("test.tm");
			//tilemap->load_from_json("Data/Tilemaps/test.json");
			//texture_selector->set_texture_sheet(tilemap->get_texture());
		}
	}
}

void EditorState::render()
{
	//return;
	if(selected_gameobject)
		selected_gameobject->add_to_buffer(scene_view);

	if (!paused)
	{
		active_scene->render(scene_view);

		render_gui();
		for (auto& it : buttons)
		{
			it.second->add_to_buffer();
		}
	}
	else
	{
		pmenu->add_to_buffer();
	}
}

void EditorState::pause_state()
{
	State::pause_state();
	pmenu->open();
}

void EditorState::unpause_state()
{
	State::unpause_state();
	pmenu->close();
}

#pragma endregion

#pragma region PROTECTED

void EditorState::init_variables()
{
	texture_rect = sf::IntRect(0, 0, UNIT_SIZE, UNIT_SIZE);
}

void EditorState::init_views()
{
	scene_view = new sf::View();
	scene_view->setSize(graphics_settings->resolution.width, graphics_settings->resolution.height);
	scene_view->setCenter(graphics_settings->resolution.width / 2.f, graphics_settings->resolution.height / 2.f);
	scene_view->zoom(1.f);
}

void EditorState::init_gui()
{
	sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(graphics_settings->resolution.height)));
	sidebar.setFillColor(sf::Color(50, 50, 50, 100));
	sidebar.setOutlineColor(sf::Color(200, 200, 200, 150));
	sidebar.setOutlineThickness(1.f);

	texture_selector = new GUI::TextureSelector(120.f + 800, 20.f, 500.f, 500.f, UNIT_SIZE,
		selected_gameobject->get_component<TilemapComponent>().get_texture(), 
		selected_gameobject->get_component<TilemapComponent>().get_tileset_keys());
	//texture_selector->toggle_hidden();

	selector_rect.setSize(sf::Vector2f(selected_gameobject->get_component<TilemapComponent>().grid_size(), 
		selected_gameobject->get_component<TilemapComponent>().grid_size()));
	selector_rect.setFillColor(sf::Color(255, 255, 255, 150));
	selector_rect.setOutlineThickness(2.f);
	selector_rect.setOutlineColor(sf::Color::White);
	selector_rect.setTexture(selected_gameobject->get_component<TilemapComponent>().get_texture());

}

void EditorState::update_gui()
{
	if (sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(
		Input::Instance()->mouse_position_window())))
	{
		return;
	}
	if (!Input::Instance()->using_input_box() && !editor_view->in_bound())
	{
		float delta = Time::Instance()->delta_time();
		if (Input::Instance()->get_action("CAM_UP"))
			scene_view->move(0, -camera_move_speed * delta);
		if (Input::Instance()->get_action("CAM_DOWN"))
			scene_view->move(0, camera_move_speed * delta);
		if (Input::Instance()->get_action("CAM_LEFT"))
			scene_view->move(-camera_move_speed * delta, 0);
		if (Input::Instance()->get_action("CAM_RIGHT"))
			scene_view->move(camera_move_speed * delta, 0);

		if (!texture_selector->mouse_in_container())
		{
			if (Input::Instance()->get_mouse_scroll_delta() > 0.f)
				scene_view->zoom(.9f);
			if (Input::Instance()->get_mouse_scroll_delta() < 0.f)
				scene_view->zoom(1.1f);
		}
	}
	if (texture_selector->mouse_in_container())
	{
		
		selected_gameobject->get_component<TilemapComponent>().highlight_layer(texture_selector->get_layer());
		texture_selector->update();
		if (texture_selector->get_tileset_selector()->changed_selection())
		{
			// texture sheet change
			std::string set_key = texture_selector->get_tileset_selector()->get_selected_button()->get_text();
			texture_selector->set_texture_sheet(selected_gameobject->get_component<TilemapComponent>().tile_sheet(set_key));
			selected_gameobject->get_component<TilemapComponent>().set_texture(set_key);
			// below line did not fix changing the selector texture
			selector_rect.setTexture(selected_gameobject->get_component<TilemapComponent>().get_texture());

		}
		if (texture_selector->mouse_in_bounds())
		{
			// Here code to set selector size
			if (Input::Instance()->get_mouse(Input::Mouse::LEFT) ||
				Input::Instance()->get_mouse_down(Input::Mouse::LEFT) ||
				Input::Instance()->get_mouse_up(Input::Mouse::LEFT))
			{
				texture_selector->set_texture_rect();
				texture_rect = texture_selector->get_texture_rect();
			}

			return;
		}
		return;
	}
	if (texture_selector->get_animation_checkbox()->is_checked())
	{
		selector_rect.setOutlineColor(sf::Color::Red);
		selector_rect.setSize(sf::Vector2f(selected_gameobject->get_component<TilemapComponent>().grid_size(), 
			selected_gameobject->get_component<TilemapComponent>().grid_size()));
	}
	else
	{
		selector_rect.setOutlineColor(sf::Color::White);
		selector_rect.setSize(sf::Vector2f(texture_rect.width, texture_rect.height));
	}
	selector_rect.setTexture(selected_gameobject->get_component<TilemapComponent>().get_texture());
	selector_rect.setTextureRect(texture_rect);

	selector_rect.setPosition(Input::Instance()->mouse_position_grid(UNIT_SIZE, scene_view).x * UNIT_SIZE,
		Input::Instance()->mouse_position_grid(UNIT_SIZE, scene_view).y * UNIT_SIZE);

	if (Input::Instance()->get_mouse_down(Input::Mouse::LEFT) || 
		Input::Instance()->get_mouse(Input::Mouse::LEFT))
	{
		
		selected_gameobject->get_component<TilemapComponent>().add_tiles(
			Input::Instance()->mouse_position_grid(UNIT_SIZE, scene_view).x,
			Input::Instance()->mouse_position_grid(UNIT_SIZE, scene_view).y,
			texture_selector->get_layer(),
			texture_rect,
			(TileNS::Type)texture_selector->get_tiletype_selector()->get_selected_index(),
			texture_selector->get_collision_checkbox()->is_checked(),
			texture_selector->get_animation_checkbox()->is_checked()
		);
		
	}
	if (Input::Instance()->get_mouse_down(Input::Mouse::RIGHT) ||
		Input::Instance()->get_mouse(Input::Mouse::RIGHT))
	{
		selected_gameobject->get_component<TilemapComponent>().remove_tiles(
			Input::Instance()->mouse_position_grid(UNIT_SIZE, scene_view).x,
			Input::Instance()->mouse_position_grid(UNIT_SIZE, scene_view).y,
			texture_selector->get_layer(),
			texture_rect,
			texture_selector->get_animation_checkbox()->is_checked()
		);
	}
	

}


void EditorState::render_gui()
{
	if (!texture_selector->mouse_in_bounds() &&
		!sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(
			Input::Instance()->mouse_position_window())))
		text_select_render = true;
	else
		text_select_render = false;

	//texture_selector->add_to_buffer();
}

void EditorState::init_tilemap()
{
	selected_gameobject = new GameObject();
	//selected_gameobject->unserialize_json(FileManager::Instance()->load_from_file("tilemap_test.json"));
	selected_gameobject->set_world_position(Vector2f(0, 0));
	selected_gameobject->add_component<TilemapComponent>(Vector2i(0, 0), UNIT_SIZE, 20, 20);
	
	selected_gameobject->add_component<TilemapColliderComponent>();
	selected_gameobject->init();
	selected_gameobject->awake();
	selected_gameobject->start();
}

}