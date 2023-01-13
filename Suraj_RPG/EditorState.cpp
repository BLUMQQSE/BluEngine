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
#include "Time.h"
#include "GameObject.h"
#include "SceneEditor.h"
#include "PrefabEditor.h"
namespace bm98
{
using namespace core;
#pragma region PUBLIC

EditorState::EditorState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
	:State(window, states, graphics_settings)
{
	state_name = "Editor_State";
	current_state = EditingState::SCENE;

	init_variables();
	init_views();
	init_fonts();
	init_buttons();


	pmenu = new PauseMenu(*window, font);
	pmenu->add_button("BACK", 500.f, 500.f, "Back");
	pmenu->add_button("SAVE", 500, 440, "Save");
	pmenu->add_button("LOAD", 500, 380, "Load");
	
	init_tilemap();
	init_gui();
	
	//texture_selector->toggle_hidden();

	tile_modifier.collision = false;
	tile_modifier.tile_type = TileType::DEFAULT;
	camera_move_speed = 120;

	Renderer::add(Renderer::RenderObject(&sidebar, outline_render, outline_layer, z_order));
	Renderer::add(Renderer::RenderObject(&selector_rect, text_select_render, text_select_layer, z_order, &scene_view));
}

EditorState::~EditorState()
{
	Renderer::remove(&sidebar);
	Renderer::remove(&selector_rect);
	
	delete scene_view;
	delete tile_selector_view;

	delete selected_gameobject;
	delete texture_selector;
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != buttons.end(); ++it)
	{
		delete it->second;
	}
}

void EditorState::on_end_state()
{
	Debug::Log("Will now clean up editor state on exit");
}

void EditorState::update_input()
{
	//if (buttons["BACK"]->is_pressed())
	//{
	//	isRequestingQuit = true;
   //}
	if (Input::using_input_box())
		return;

	if (Input::get_action_down("MENU"))
	{
		std::cout << "MENU\n";
		if (!paused)
			pause_state();
		else
			unpause_state();
	}
}

void EditorState::update_sfml(sf::Event sfEvent)
{
	
}

void EditorState::update()
{
	//Debug::mouse_position_display(font);
	update_input();
	if (!paused)
	{
		State::update();

		if(selected_gameobject)
			selected_gameobject->update();

		for (auto& it : buttons)
		{
			it.second->update();
		}
		if (Input::get_action_down("TS_MENU"))
			buttons.at("TS_BTN")->set_pressed();
		if (buttons.at("TS_BTN")->is_pressed())
			texture_selector->toggle_hidden();
		if (Input::get_action_down("COLLISION"))
			tile_modifier.collision = !tile_modifier.collision;
		if (Input::get_action_down("TYPE"))
			tile_modifier.tile_type = TileType::DAMAGING;
		
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
	scene_view->setSize(
		sf::Vector2f(
			graphics_settings->resolution.width,
			graphics_settings->resolution.height
		)
	);
	scene_view->setCenter(
		graphics_settings->resolution.width / 2.f,
		graphics_settings->resolution.height / 2.f
	);

	tile_selector_view = new sf::View();
	
	tile_selector_view->setSize
	(
		500.f, 500.f
	);
	tile_selector_view->setCenter(1170, 270);
}

void EditorState::init_gui()
{
	sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(graphics_settings->resolution.height)));
	sidebar.setFillColor(sf::Color(50, 50, 50, 100));
	sidebar.setOutlineColor(sf::Color(200, 200, 200, 150));
	sidebar.setOutlineThickness(1.f);

	texture_selector = new GUI::TextureSelector(120.f + 800, 20.f, 500.f, 500.f, UNIT_SIZE,
		selected_gameobject->get_component<TilemapComponent>().get_texture(), font, 
		selected_gameobject->get_component<TilemapComponent>().get_tileset_keys(), tile_selector_view);
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
	if (sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position_window())))
	{
		return;
	}

	if (!Input::using_input_box())
	{
		if (Input::get_action("CAM_UP"))
			scene_view->move(0, -camera_move_speed * Time::delta_time());
		if (Input::get_action("CAM_DOWN"))
			scene_view->move(0, camera_move_speed * Time::delta_time());
		if (Input::get_action("CAM_LEFT"))
			scene_view->move(-camera_move_speed * Time::delta_time(), 0);
		if (Input::get_action("CAM_RIGHT"))
			scene_view->move(camera_move_speed * Time::delta_time(), 0);

		if (!texture_selector->mouse_in_container())
		{
			if (Input::get_mouse_scroll_delta() > 0.f)
				scene_view->zoom(.9f);
			if (Input::get_mouse_scroll_delta() < 0.f)
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
			if (Input::get_mouse(Input::Mouse::LEFT) ||
				Input::get_mouse_down(Input::Mouse::LEFT) ||
				Input::get_mouse_up(Input::Mouse::LEFT))
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
		selector_rect.setSize(sf::Vector2f(selected_gameobject->get_component<TilemapComponent>().grid_size(), selected_gameobject->get_component<TilemapComponent>().grid_size()));
	}
	else
	{
		selector_rect.setOutlineColor(sf::Color::White);
		selector_rect.setSize(sf::Vector2f(texture_rect.width, texture_rect.height));
	}
	selector_rect.setTexture(selected_gameobject->get_component<TilemapComponent>().get_texture());
	selector_rect.setTextureRect(texture_rect);

	selector_rect.setPosition(Input::mouse_position_grid(UNIT_SIZE, scene_view).x * UNIT_SIZE, Input::mouse_position_grid(UNIT_SIZE, scene_view).y * UNIT_SIZE);

	if (Input::get_mouse_down(Input::Mouse::LEFT) || Input::get_mouse(Input::Mouse::LEFT))
	{
		selected_gameobject->get_component<TilemapComponent>().add_tiles(
			Input::mouse_position_grid(UNIT_SIZE, scene_view).x,
			Input::mouse_position_grid(UNIT_SIZE, scene_view).y,
			texture_selector->get_layer(),
			texture_rect,
			(TileType)texture_selector->get_tiletype_selector()->get_selected_index(),
			texture_selector->get_collision_checkbox()->is_checked(),
			texture_selector->get_animation_checkbox()->is_checked()
		);
	}
	if (Input::get_mouse_down(Input::Mouse::RIGHT) || Input::get_mouse(Input::Mouse::RIGHT))
	{
		selected_gameobject->get_component<TilemapComponent>().remove_tiles(
			Input::mouse_position_grid(UNIT_SIZE, scene_view).x,
			Input::mouse_position_grid(UNIT_SIZE, scene_view).y,
			texture_selector->get_layer(),
			texture_rect,
			texture_selector->get_animation_checkbox()->is_checked()
		);
	}
	

}


void EditorState::render_gui()
{
	if (!texture_selector->mouse_in_bounds() &&
		!sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position_window())))
		text_select_render = true;
	else
		text_select_render = false;

	texture_selector->add_to_buffer(tile_selector_view);
	//texture_selector->add_to_buffer();
}

void EditorState::init_buttons()
{
	buttons["TS_BTN"] = new GUI::Button(20, 20, 50.f, 50.f, &font, "T", 36,
		sf::Color(270, 270, 270, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 255));
}

void EditorState::init_tilemap()
{
	selected_gameobject = new GameObject();
	selected_gameobject->add_component<TilemapComponent>(0, 0, UNIT_SIZE, 20, 20);
	selected_gameobject->init();
	selected_gameobject->awake();
	selected_gameobject->start();
	//selected_gameobject->get_component<TilemapComponent>().set_position(Vector2i(100, 0));
}

}