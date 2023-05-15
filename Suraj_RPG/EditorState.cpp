#include "pch.h"
#include "EditorState.h"

#include "core/Input.h"
#include "core/Input.h"
#include "core/Debug.h"
#include "core/Physics.h"
#include "core/SceneManager.h"

#include "Gui.h"
#include "PauseMenu.h"
#include "TilemapComponent.h"
#include "TilemapColliderComponent.h"
#include "Time.h"
#include "GameObject.h"
#include "SceneEditorView.h"
#include "PrefabEditor.h"
#include "Scene.h"
#include "SpriteComponent.h"
#include "GraphicsSettings.h"
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

	active_scene = new Scene("default.json");

	EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENEMANAGER_INITIALIZE_, static_cast<void*>(active_scene));
	//SceneManager::Instance()->init(active_scene);
	SceneManager::Instance()->load_scene_prefab("default.json");

	active_scene->set_view(scene_view);

	pmenu = new PauseMenu(*window, font);
	pmenu->add_button("BACK", 500.f, 500.f, "Back");
	pmenu->add_button("SAVE", 500, 440, "Save");
	pmenu->add_button("LOAD", 500, 380, "Load");
	
	init_gui();
	
	//texture_selector->toggle_hidden();

	camera_move_speed = 180;


	scene_editor = new SceneEditorView();
	//editor_view->set_view(scene_view);

	Renderer::Instance()->add(Renderer::RenderObject(&selector_rect, text_select_render, text_select_layer, z_order, &scene_view));
}

EditorState::~EditorState()
{
	Renderer::Instance()->remove(&selector_rect);

	delete scene_view;
	delete active_scene;

	//delete selected_gameobject;
	
	delete texture_selector;
	delete scene_editor;
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != buttons.end(); ++it)
	{
		delete it->second;
	}
}

void EditorState::init_state()
{
	Debug::Instance()->core_log("[EditorState] Initialized", Debug::LogLevel::INFO);
}

void EditorState::on_end_state()
{
	Debug::Instance()->core_log("[EditorState] Shutdown", Debug::LogLevel::INFO);

	//FileManager::Instance()->save_to_file_styled(selected_gameobject->serialize_json(), "tilemap_test.json");

	EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENEMANAGER_DESTROY_);
	State::on_end_state();
}

void EditorState::update_input()
{
	if (Input::Instance()->using_input_box())
		return;

	if (Input::Instance()->get_action_down("V"))
	{
		scene_editor->toggle_editor(SceneEditorView::EditorPanel::ALL);
	}

	if (Input::Instance()->get_action_down("MENU"))
	{
		if (!paused)
			pause_state();
		else
			unpause_state();
	}
}

void EditorState::update_sfml(sf::Event sfEvent)
{
	scene_editor->update_sfml(sfEvent);
}

void EditorState::update()
{
	update_input();
	scene_editor->update();
	if (!paused)
	{
		State::update();
		//if(selected_gameobject)
		//	selected_gameobject->update();

		for (auto& it : buttons)
		{
			it.second->update();
		}
		if (Input::Instance()->get_action_down("TS_MENU") && !Input::Instance()->using_input_box())
		{
			texture_selector->toggle_hidden();
		}
		update_gui();
	}
	else
	{
		pmenu->update();
		if (pmenu->is_button_pressed("BACK"))
			isRequestingQuit = true;
		if (pmenu->is_button_pressed("SAVE"))
		{
		}
		if (pmenu->is_button_pressed("LOAD"))
		{
		}
	}
}

void EditorState::render()
{
	//if(selected_gameobject)
	//	selected_gameobject->add_to_buffer(scene_view);

	if (!paused)
	{
		active_scene->render(scene_view);
		scene_editor->add_to_buffer(scene_view);
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
	texture_selector = new GUI::TextureSelector(120.f + 800, 20.f, 500.f, 500.f,
		TilemapComponent::GetTilesetKeys());
	
	selector_rect.setFillColor(sf::Color(255, 255, 255, 150));
	selector_rect.setOutlineThickness(2.f);
	selector_rect.setOutlineColor(sf::Color::White);
	//selector_rect.setTexture(selected_gameobject->get_component<TilemapComponent>().get_texture());

}

void EditorState::init_tilemap_stuff(GameObject* selected_gameobject)
{
	// dont activate again
	if (texture_selector->get_active())
		return;

	texture_selector->init(UNIT_SIZE,
						   selected_gameobject->get_component<TilemapComponent>().get_texture());

	selector_rect.setSize(sf::Vector2f(selected_gameobject->get_component<TilemapComponent>().grid_size(),
						  selected_gameobject->get_component<TilemapComponent>().grid_size()));

	selector_rect.setFillColor(sf::Color(255, 255, 255, 150));
	selector_rect.setOutlineThickness(2.f);
	selector_rect.setOutlineColor(sf::Color::White);
	selector_rect.setTexture(selected_gameobject->get_component<TilemapComponent>().get_texture());
}

void EditorState::remove_tilemap_stuff()
{
	//dont deactivate again
	if (!texture_selector->get_active())
		return;

	texture_selector->set_inactive();
}

void EditorState::update_gui()
{
	if (!Input::Instance()->using_input_box() && !scene_editor->in_bound())
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

	GameObject* selected_gameobject = scene_editor->get_selected_gameobject();
	if (!selected_gameobject)
	{
		remove_tilemap_stuff();
		return;
	}
	if (!selected_gameobject->has_component<TilemapComponent>() ||
		selected_gameobject->get_world_position() != Vector2f::Zero())
	{
		remove_tilemap_stuff();
		return;
	}
	
	init_tilemap_stuff(selected_gameobject);

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

	if (texture_selector->get_render())
	{
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

}


void EditorState::render_gui()
{
	if (!texture_selector->mouse_in_bounds() && texture_selector->get_render())
		text_select_render = true;
	else
		text_select_render = false;

	texture_selector->add_to_buffer();
}

}