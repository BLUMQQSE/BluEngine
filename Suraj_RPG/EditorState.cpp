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
	main_view = new sf::View();
	state_name = "Editor_State";
	current_state = EditingState::TILEMAP;
	init_variables();
	init_views();
	init_background();
	init_fonts();
	init_buttons();

	pmenu = new PauseMenu(*window, font);
	pmenu->add_button("BACK", 900.f, 900.f, "Back");
	pmenu->add_button("SAVE", 900, 840, "Save");
	pmenu->add_button("LOAD", 900, 780, "Load");
	
	init_tilemap();
	init_gui();

	editing_scene = new SceneEditor(&font);
	editing_prefab = new PrefabEditor();

	tile_modifier.collision = false;
	tile_modifier.tile_type = TileType::DEFAULT;
	camera_move_speed = 120;

	Renderer::add(Renderer::RenderObject(&sidebar, outline_render, outline_layer, z_order));
	Renderer::add(Renderer::RenderObject(&selector_rect, text_select_render, text_select_layer, z_order, &main_view));
}

EditorState::~EditorState()
{
	Renderer::remove(&sidebar);
	Renderer::remove(&selector_rect);
	delete tilemap_go;
	delete main_view;
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

	if (Input::get_action_down("SCENE_EDITOR"))
	{
		current_state = EditingState::SCENE;
	}
	if (Input::get_action_down("PREFAB_EDITOR"))
	{
		current_state = EditingState::PREFAB;
	}
	if (Input::get_action_down("MENU"))
	{
		if (!paused)
			pause_state();
		else
			unpause_state();
	}
}

void EditorState::update_sfml(sf::Event sfEvent)
{
	if (current_state == EditingState::SCENE)
		editing_scene->update_sfml(sfEvent);
	if (current_state == EditingState::PREFAB)
		editing_prefab->update_sfml(sfEvent);
}

void EditorState::update()
{
	//Debug::mouse_position_display(font);
	update_input();
	if (!paused)
	{
		State::update();
		tilemap->update();

		if (current_state == EditingState::SCENE)
		{
			editing_scene->update();
			update_gui();
			return;
		}
		if (current_state == EditingState::PREFAB)
		{
			editing_prefab->update();
		}
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
			tilemap->save_to_json("Data/Tilemaps/test.json");
		}
		//tilemap->save_to_file("test.tm");
		if (pmenu->is_button_pressed("LOAD"))
		{
			//tilemap->load_from_file("test.tm");
			tilemap->load_from_json("Data/Tilemaps/test.json");
			texture_selector->set_texture_sheet(tilemap->get_texture());
		}
	}
}

void EditorState::late_update()
{

}

void EditorState::fixed_update()
{

}

void EditorState::render()
{
	if (current_state == EditingState::SCENE)
		editing_scene->render(main_view);
	if (current_state == EditingState::PREFAB)
		editing_prefab->render(main_view);
	//return;
	tilemap->add_to_buffer(main_view);

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
	main_view->setSize(
		sf::Vector2f(
			graphics_settings->resolution.width,
			graphics_settings->resolution.height
		));
	main_view->setCenter(
		graphics_settings->resolution.width / 2.f,
		graphics_settings->resolution.height / 2.f
	);

}

void EditorState::init_background()
{

}

void EditorState::init_gui()
{
	sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(graphics_settings->resolution.height)));
	sidebar.setFillColor(sf::Color(50, 50, 50, 100));
	sidebar.setOutlineColor(sf::Color(200, 200, 200, 150));
	sidebar.setOutlineThickness(1.f);

	texture_selector = new GUI::TextureSelector(120.f + 800, 20.f, 500.f, 500.f, UNIT_SIZE,
		tilemap->get_texture(), font, tilemap->get_tileset_keys());
	texture_selector->toggle_hidden();

	selector_rect.setSize(sf::Vector2f(tilemap->grid_size(), tilemap->grid_size()));
	selector_rect.setFillColor(sf::Color(255, 255, 255, 150));
	selector_rect.setOutlineThickness(2.f);
	selector_rect.setOutlineColor(sf::Color::White);
	selector_rect.setTexture(tilemap->get_texture());

}

void EditorState::update_gui()
{
	if (sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position_window())))
		return;

	if (!Input::using_input_box())
	{
		if (Input::get_action("CAM_UP"))
			main_view->move(0, -camera_move_speed * Time::delta_time());
		if (Input::get_action("CAM_DOWN"))
			main_view->move(0, camera_move_speed * Time::delta_time());
		if (Input::get_action("CAM_LEFT"))
			main_view->move(-camera_move_speed * Time::delta_time(), 0);
		if (Input::get_action("CAM_RIGHT"))
			main_view->move(camera_move_speed * Time::delta_time(), 0);

		if (Input::get_mouse_scroll_delta() > 0)
			main_view->zoom(.9f);
		if (Input::get_mouse_scroll_delta() < 0)
			main_view->zoom(1.1);
	}

	if (current_state == EditingState::TILEMAP)
	{

		if (texture_selector->mouse_in_container())
		{
			tilemap->highlight_layer(texture_selector->get_layer());
			texture_selector->update();
			if (texture_selector->get_tileset_selector()->changed_selection())
			{
				// texture sheet change
				std::string set_key = texture_selector->get_tileset_selector()->get_selected_button()->get_text();
				texture_selector->set_texture_sheet(tilemap->tile_sheet(set_key));
				tilemap->set_texture(set_key);
				// below line did not fix changing the selector texture
				selector_rect.setTexture(tilemap->get_texture());

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
			selector_rect.setSize(sf::Vector2f(tilemap->grid_size(), tilemap->grid_size()));
		}
		else
		{
			selector_rect.setOutlineColor(sf::Color::White);
			selector_rect.setSize(sf::Vector2f(texture_rect.width, texture_rect.height));
		}
		selector_rect.setTexture(tilemap->get_texture());
		selector_rect.setTextureRect(texture_rect);

		selector_rect.setPosition(Input::mouse_position_grid(UNIT_SIZE, main_view).x * UNIT_SIZE, Input::mouse_position_grid(UNIT_SIZE, main_view).y * UNIT_SIZE);

		if (Input::get_mouse_down(Input::Mouse::LEFT) || Input::get_mouse(Input::Mouse::LEFT))
		{
			tilemap->add_tiles(
				Input::mouse_position_grid(UNIT_SIZE, main_view).x,
				Input::mouse_position_grid(UNIT_SIZE, main_view).y,
				texture_selector->get_layer(),
				texture_rect,
				(TileType)texture_selector->get_tiletype_selector()->get_selected_index(),
				texture_selector->get_collision_checkbox()->is_checked(),
				texture_selector->get_animation_checkbox()->is_checked()
			);
		}
		if (Input::get_mouse_down(Input::Mouse::RIGHT) || Input::get_mouse(Input::Mouse::RIGHT))
		{
			tilemap->remove_tiles(
				Input::mouse_position_grid(UNIT_SIZE, main_view).x,
				Input::mouse_position_grid(UNIT_SIZE, main_view).y,
				texture_selector->get_layer(),
				texture_rect,
				texture_selector->get_animation_checkbox()->is_checked()
			);
		}
	}

}


void EditorState::render_gui()
{
	//Renderer::add(Renderer::RenderObject(&sidebar, _render, SortingLayer::UI));
	if (!texture_selector->mouse_in_bounds() &&
		!sidebar.getGlobalBounds().contains(static_cast<sf::Vector2f>(Input::mouse_position_window())))
		text_select_render = true;
	else
		text_select_render = false;
		//Renderer::add(Renderer::RenderObject(&selector_rect, _render, SortingLayer::UI, 0, &main_view));

	texture_selector->add_to_buffer();
}

void EditorState::init_buttons()
{
	/*
	buttons["BACK"] = new GUI::Button(100.f, 540.f, 150.f, 60.f, &font, "BACK", 28,
		sf::Color(70, 70, 70, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));
	*/
	buttons["TS_BTN"] = new GUI::Button(20, 20, 50.f, 50.f, &font, "T", 36,
		sf::Color(270, 270, 270, 250), sf::Color(20, 20, 20, 100), sf::Color(250, 250, 250, 250),
		sf::Color(70, 70, 70, 255), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 255));
}

void EditorState::init_tilemap()
{
	//tilemap = new TilemapComponent(0, 0, UNIT_SIZE, 32, 32);
	tilemap_go = new GameObject();
	tilemap_go->add_component<TilemapComponent>(0, 0, UNIT_SIZE, 32, 32);
	tilemap = &tilemap_go->get_component<TilemapComponent>();
}
}