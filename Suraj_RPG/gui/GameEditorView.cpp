#include "../pch.h"
#include "../core/Input.h"
#include "../core/SceneManager.h"
#include "../core/ResourceManager.h"
#include "../core/Physics.h"

#include "GameEditorView.h"
#include "../GameObject.h"

namespace bm98
{
GameEditorView::GameEditorView()
{
	EventSystem::Instance()->subscribe(EventID::SCENE_ADDED_GAMEOBJECT_FLAG, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_REMOVED_GAMEOBJECT_FLAG, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_CHANGE_FLAG, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_GAMEOBJECT_ORDER_CHANGE_FLAG, this);

	EventSystem::Instance()->subscribe(EventID::_SYSTEM_FIXED_UPDATE_START_FLAG_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_FIXED_UPDATE_END_FLAG_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_GAMEOBJECT_FIXED_UPDATE_START_FLAG_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_GAMEOBJECT_FIXED_UPDATE_END_FLAG_, this);

	init();
}

GameEditorView::~GameEditorView()
{
	delete heir_panel;
	delete inspec_panel;
	delete context_panel;
}

void GameEditorView::init()
{
	heir_panel = new GUI::Panel(0, 0, Renderer::Instance()->get_window_size().x / 6,
		Renderer::Instance()->get_window_size().y);
	inspec_panel = new GUI::Panel(Renderer::Instance()->get_window_size().x -
		Renderer::Instance()->get_window_size().x / 6, 0,
		Renderer::Instance()->get_window_size().x / 6, Renderer::Instance()->get_window_size().y);
	context_panel = new GUI::Panel(heir_panel->get_width() + 400, Renderer::Instance()->get_window_size().y - 200.f,
										inspec_panel->get_position().x - (heir_panel->get_position().x + heir_panel->get_width()) - 800, 200.f);

	context_panel->set_fill_color(sf::Color::Transparent);

	create_context_panel();

	heir_panel->set_render(false);
	inspec_panel->set_render(false);
	context_panel->set_render(true);

}

void GameEditorView::toggle_editor(EditorPanel panel_to_toggle)
{
	switch (panel_to_toggle)
	{
	case EditorPanel::ALL:
		if (heir_active || inspec_active || context_active)
		{
			heir_active = false;
			heir_panel->set_render(false);

			inspec_active = false;
			inspec_panel->set_render(false);

			context_active = false;
			context_panel->set_render(false);
		}
		else
		{
			heir_active = true;
			heir_panel->set_render(true);

			inspec_active = true;
			inspec_panel->set_render(true);

			context_active = true;
			context_panel->set_render(true);
			align_context_panel();
		}
		break;
	case EditorPanel::HEIRARCHY:
		heir_active = !heir_active;
		heir_panel->set_render(heir_active);
		align_context_panel();
		break;
	case EditorPanel::INSPECTOR:
		inspec_active = !inspec_active;
		inspec_panel->set_render(inspec_active);
		break;
	case EditorPanel::CONTEXT:
		context_active = !context_active;
		context_panel->set_render(context_active);
		align_context_panel();
	}
}

void GameEditorView::update()
{
	if (heir_active)
	{
		heir_panel->update();
		update_heir_panel();
	}
	if (inspec_active)
	{
		inspec_panel->update();
		update_inspec_panel();
	}
	if (context_active)
	{
		context_panel->update();
		update_context_panel();
	}

	if (update_intervals_ignored >= UPDATE_INTERVALS_TO_IGNORE)
	{
		update_intervals_ignored = 0;
		delta_string.str("");
		delta_string << PRECISION(2) << Time::Instance()->delta_time() * 1000 << "ms";
	}
	else
	{
		update_intervals_ignored++;
	}

}

void GameEditorView::update_sfml(sf::Event sfEvent)
{
	if (heir_active)
		heir_panel->update_sfml(sfEvent);
	if (inspec_active)
		inspec_panel->update_sfml(sfEvent);
	if (context_active)
		context_panel->update_sfml(sfEvent);
}

void GameEditorView::handle_event(Event* event)
{
	EventID id = event->get_event_id();

	switch (event->get_event_id())
	{
	case EventID::SCENE_ADDED_GAMEOBJECT_FLAG:
		create_heir_panel();
		create_context_panel();
		break;
	case EventID::SCENE_REMOVED_GAMEOBJECT_FLAG:
		create_heir_panel();
		create_context_panel();
		break;
	case EventID::SCENE_CHANGE_FLAG:
		create_heir_panel();
		create_context_panel();
		break;
	case EventID::SCENE_GAMEOBJECT_ORDER_CHANGE_FLAG:
		create_heir_panel();
		break;
	case EventID::_SYSTEM_FIXED_UPDATE_START_FLAG_:
	{
		fixed_update_timer.restart();
		break;
	}
	case EventID::_SYSTEM_FIXED_UPDATE_END_FLAG_:
		fixed_update_duration = fixed_update_timer.get_elapsed_time().asMilliseconds();
		break;
	case EventID::_SYSTEM_GAMEOBJECT_FIXED_UPDATE_START_FLAG_:
		entity_fixed_update_timer.restart();
		break;
	case EventID::_SYSTEM_GAMEOBJECT_FIXED_UPDATE_END_FLAG_:
		entity_fixed_update_duration = fixed_update_timer.get_elapsed_time().asMilliseconds();
		break;
	}
}

void GameEditorView::create_heir_panel()
{
	// wipe anything previously on the panel
	heir_panel->clear();
	objects_in_scene_map.clear();

	std::vector<std::weak_ptr<GameObject>> objs = SceneManager::Instance()->get_objects_in_scene();

	float x = 0;

	for (std::size_t i = 0; i < objs.size(); i++)
	{
		std::shared_ptr<GameObject> temp(objs[i].lock()->self());

		while (temp->get_parent().lock())
		{
			x += 30;
			temp = temp->get_parent().lock()->self();
		}

		heir_panel->add_element(std::to_string((size_t)objs[i].lock()->get_info().unique_id), new GUI::Button(x, 40 * i, 100.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
			objs[i].lock()->get_info().name + " [" + std::to_string((size_t)objs[i].lock()->get_info().unique_id) + "]", 12,
			sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
			sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255)));

		objects_in_scene_map[heir_panel->get_button(std::to_string((size_t)objs[i].lock()->get_info().unique_id))] = objs[i].lock().get();

		x = 0;
	}

	if (!heir_active)
		heir_panel->set_render(false);
	if (!inspec_active)
		inspec_panel->set_render(false);
	if (!selected_gameobject)
		selected_gameobject = objs[0].lock().get();

	heir_panel->fit_to_content();
	create_context_panel();
}

void GameEditorView::update_heir_panel()
{
	// NOTHING TO UPDATE
	if (objects_in_scene_map.size() == 0)
	{
		selected_gameobject = nullptr;
		return;
	}
	/*
	if (gameobject_held == true && core::Input::Instance()->get_mouse_up(core::Input::Mouse::LEFT))
	{
		if (heir_panel->mouse_in_bounds())
		{
			for (auto& ois : objects_in_scene_map)
			{
				if (ois.first->mouse_in_bounds())
				{

					if (ois.second != selected_gameobject)
					{
						selected_gameobject->set_parent(ois.second->self());
						gameobject_held = false;
						return;
					}
					else
					{
						gameobject_held = false;
						break;
					}
				}
			}
			if(gameobject_held)
				selected_gameobject->set_parent(nullptr);
		}
		else
		{
			gameobject_held = false;
		}
	}
	
	if (gameobject_held == true && !core::Input::Instance()->get_mouse(core::Input::Mouse::LEFT))
	{
		gameobject_held = false;
		return;
	}
	*/
	if (Input::Instance()->get_mouse_down(core::Input::Mouse::LEFT))
	{
		for (auto& ois : objects_in_scene_map)
		{
			if (ois.first->mouse_in_bounds())
			{
				EventSystem::Instance()->push_event(EventID::_SYSTEM_SCENE_SET_SELECTED_GAMEOBJECT_, ois.second);
				selected_gameobject = ois.second;
				//gameobject_held = true;	
				create_inspec_panel();
			}
		}
	}
}

void GameEditorView::create_inspec_panel()
{
	inspec_panel->clear();
	variables_in_components.clear();
	std::vector<std::pair<std::weak_ptr<Component>,
		std::vector<bm98::Editor::SerializedVar>>> selected_gameobject_components;

	std::vector<std::weak_ptr<Component>> components = selected_gameobject->get_components();

	for (std::size_t i = 0; i < components.size(); i++)
	{
		selected_gameobject_components.push_back(std::make_pair(components[i], components[i].lock()->get_editor_values()));
	}


	int component_panel_height = 0;

	inspec_panel->add_element(selected_gameobject->get_info().name, create_component_panel(component_panel_height, inspec_panel->get_width(),
		selected_gameobject->get_info().name, selected_gameobject->get_editor_values()));

	component_panel_height += inspec_panel->get_panel(selected_gameobject->get_info().name)->get_height();

	for (std::size_t i = 0; i < selected_gameobject_components.size(); i++)
	{
		std::string component_name = RemoveNamespace(typeid(*components[i].lock()).name());

		inspec_panel->add_element(component_name, create_component_panel(component_panel_height, inspec_panel->get_width(), 
			component_name, selected_gameobject_components[i].second));

		component_panel_height += inspec_panel->get_panel(component_name)->get_height();

	}
	//if (!inspec_active)
	//	inspec_panel->set_render(false);
}

void GameEditorView::update_inspec_panel()
{
	for (auto& component_panel : variables_in_components)
	{
		update_component_panel(component_panel);
	}
}

void GameEditorView::create_context_panel()
{
	context_panel->clear();

	align_context_panel();

	GUI::Label* scene_label = new GUI::Label(5, 5, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Scene:", sf::Color::Cyan);

	std::string s = SceneManager::Instance()->get_active_scene_name().substr(0, SceneManager::Instance()->get_active_scene_name().size()-5);
	GUI::Label* scene_name = new GUI::Label(140, 5, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
											s, sf::Color::Cyan);
	
	scene_name->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("scene_label", scene_label);
	context_panel->add_element("scene_name", scene_name);

	GUI::Label* fps_label = new GUI::Label(5, 20, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "FPS:", sf::Color::Cyan);
	GUI::Label* fps_count = new GUI::Label(140, 20, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
											std::to_string(Time::Instance()->get_fps_average()), sf::Color::Cyan);

	fps_count->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("fps_label", fps_label);
	context_panel->add_element("fps_count", fps_count);

	GUI::Label* entities_label = new GUI::Label(5, 35, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Entities:", sf::Color::Cyan);
	GUI::Label* entities_count = new GUI::Label(140, 35, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
										   std::to_string(SceneManager::Instance()->get_objects_in_scene().size()), sf::Color::Cyan);

	entities_count->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("entities_label", entities_label);
	context_panel->add_element("entities_count", entities_count);

	GUI::Label* physics_ent_label = new GUI::Label(5, 50, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Phys Ent:", sf::Color::Cyan);
	GUI::Label* physics_ent_count = new GUI::Label(140, 50, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
												std::to_string(Physics::Instance()->get_physics_objects_count()), sf::Color::Cyan);
	
	physics_ent_count->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("physics_ent_label", physics_ent_label);
	context_panel->add_element("physics_ent_count", physics_ent_count);

	GUI::Label* renderable_ent_label = new GUI::Label(5, 65, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Renders:", sf::Color::Cyan);
	GUI::Label* renderable_ent_count = new GUI::Label(140, 65, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
												   std::to_string(Renderer::Instance()->get_renderables_count()), sf::Color::Cyan);

	renderable_ent_count->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("renderable_ent_label", renderable_ent_label);
	context_panel->add_element("renderable_ent_count", renderable_ent_count);

	GUI::Label* ui_renderable_ent_label = new GUI::Label(5, 80, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "UI Renders:", sf::Color::Cyan);
	GUI::Label* ui_renderable_ent_count = new GUI::Label(140, 80, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
													  std::to_string(Renderer::Instance()->get_ui_renderables_count()), sf::Color::Cyan);

	ui_renderable_ent_count->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("ui_renderable_ent_label", ui_renderable_ent_label);
	context_panel->add_element("ui_renderable_ent_count", ui_renderable_ent_count);

	GUI::Label* update_label = new GUI::Label(5, 95, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Update:", sf::Color::Cyan);
	delta_string.str("");
	delta_string << PRECISION(2) << Time::Instance()->delta_time() * 1000 << "ms";
	GUI::Label* update_time = new GUI::Label(135, 95, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
												   delta_string.str(), sf::Color::Cyan);

	update_time->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("update_label", update_label);
	context_panel->add_element("update_time", update_time);

	/*
	GUI::Label* fixed_update_label = new GUI::Label(5, 115, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Phys Fixed:", sf::Color::Cyan);
	GUI::Label* fixed_update_time = new GUI::Label(140, 115, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
														 std::to_string(fixed_update_duration), sf::Color::Cyan);

	fixed_update_time->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("fixed_update_label", fixed_update_label);
	context_panel->add_element("fixed_update_time", fixed_update_time);


	GUI::Label* ent_fixed_update_label = new GUI::Label(5, 130, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Ent Fixed:", sf::Color::Cyan);
	GUI::Label* ent_fixed_update_time = new GUI::Label(140, 130, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"),
												   std::to_string(entity_fixed_update_duration), sf::Color::Cyan);

	ent_fixed_update_time->align_text(GUI::Align::CENTER_RIGHT);

	context_panel->add_element("ent_fixed_update_label", ent_fixed_update_label);
	context_panel->add_element("ent_fixed_update_time", ent_fixed_update_time);
	*/


	GUI::Label* draw_label = new GUI::Label(5, 115, 14, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Draw Gizmos:", sf::Color::Cyan);
	GUI::Checkbox* draw_gizmos = new GUI::Checkbox(120, 115, 15);
	draw_gizmos->set_checked(Renderer::Instance()->get_draw_gizmos());

	context_panel->add_element("draw_label", draw_label);
	context_panel->add_element("draw_gizmo", draw_gizmos);

	context_panel->fit_to_content(GUI::Align::TOP_LEFT, Vector2f(5,5));
}

void GameEditorView::update_context_panel()
{
	Renderer::Instance()->set_draw_gizmos(context_panel->get_checkbox("draw_gizmo")->is_checked());
	static_cast<GUI::Label*>(context_panel->get_element("fps_count"))->set_text(std::to_string(Time::Instance()->get_fps_average()));

	static_cast<GUI::Label*>(context_panel->get_element("update_time"))->set_text(delta_string.str());

	//static_cast<GUI::Label*>(context_panel->get_element("fixed_update_time"))->set_text(std::to_string(fixed_update_duration));
	//static_cast<GUI::Label*>(context_panel->get_element("ent_fixed_update_time"))->set_text(std::to_string(entity_fixed_update_duration));

}

void GameEditorView::align_context_panel()
{
	if (heir_active)
		context_panel->set_position(heir_panel->get_width()+10, 80);
	else
		context_panel->set_position(10, 80);
}

GUI::Panel* GameEditorView::create_component_panel(float pos_y, float width, std::string component_name
	, std::vector<Editor::SerializedVar> vars)
{
	float height = 0;
	std::vector<std::pair<std::string, GUI::GUIObject*>> items_in_panel;

	items_in_panel.push_back(std::make_pair("title", new GUI::Label(0, 0, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"), component_name)));
	height += 25;

	for (std::size_t i = 0; i < vars.size(); i++)
	{
		switch (vars[i].type)
		{
		case Var::Type::Int:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair(vars[i].name, new GUI::InputBox(150, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));

			height += 20;
			break;
		}
		case Var::Type::Float:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair(vars[i].name, new GUI::InputBox(150, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));

			height += 20;
			break;
		}
		case Var::Type::Bool:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair(vars[i].name, new GUI::Checkbox(150, height, 12)));
			
			height += 20;
			break;
		}
		case Var::Type::Dropdown:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair(vars[i].name, new GUI::DropDownList(150, height, 80, 15, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"),
				vars[i].extra_data, 0, 12)));

			height += 30;
			break;
		}
		case Var::Type::FlagDropdown:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
									 ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::FlagDropDownList* drop_down = new GUI::FlagDropDownList(150, height, 80, 15,
																		 ResourceManager::Instance()->get_font("calibri-regular.ttf"),
																		 *static_cast<EnumFlag*>(vars[i].variable), 0, 12);

			items_in_panel.push_back(std::make_pair(vars[i].name, drop_down));

			height += 30;

			break;
		}
		case Var::Type::Vector2f:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair("x_" + vars[i].name, new GUI::InputBox(110, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));
			items_in_panel.push_back(std::make_pair("y_" + vars[i].name, new GUI::InputBox(170, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));

			height += 20;
			break;
		}
		case Var::Type::Vector2i:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair("ix_" + vars[i].name, new GUI::InputBox(110, height, 40, 12, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));
			items_in_panel.push_back(std::make_pair("iy_" + vars[i].name, new GUI::InputBox(170, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));

			height += 20;
			break;
		}
		case Var::Type::FloatConvex:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			FloatConvex polygon = *static_cast<FloatConvex*>(vars[i].variable);

			for (std::size_t i = 0; i < polygon.getPointCount(); i++)
			{
				items_in_panel.push_back(std::make_pair(std::to_string(i) + "_x_" + vars[i].name, new GUI::InputBox(110, height, 40, 12, 12,
					ResourceManager::Instance()->get_font("calibri-regular.ttf"))));
				items_in_panel.push_back(std::make_pair(std::to_string(i) + "_y_" + vars[i].name, new GUI::InputBox(170, height, 40, 12, 12,
					ResourceManager::Instance()->get_font("calibri-regular.ttf"))));
				height += 20;
			}

			break;
		}
		case Var::Type::Header:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 16, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			
			height += 20;
			break;
		}
		case Var::Type::String:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			items_in_panel.push_back(std::make_pair(vars[i].name, new GUI::InputBox(70, height, 140, 12, 12, 
				ResourceManager::Instance()->get_font("calibri-regular.ttf"))));
			
			height += 20;
			break;
		}
		}
	}

	GUI::Panel* panel = new GUI::Panel(0, pos_y, width, height, sf::Color(200, 200, 200, 255));

	for (std::size_t i = 0; i < items_in_panel.size(); i++)
	{
		panel->add_element(items_in_panel[i].first, items_in_panel[i].second);
	}
	
	variables_in_components.push_back(std::make_pair(panel, vars));
	return panel;
}

void GameEditorView::update_component_panel(std::pair<GUI::Panel*, std::vector<Editor::SerializedVar>> vars)
{
	for (std::size_t i = 0; i < vars.second.size(); i++)
	{
		// here handle updating for all objects in scene
		switch (vars.second[i].type)
		{
		case Var::Type::Int:
		{
			int value = *static_cast<int*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION(1) << value;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->set_text(s.str());
			break;
		}
		case Var::Type::Float:
		{
			float value = *static_cast<float*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION(1) << value;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->set_text(s.str());
			break;
		}
		case Var::Type::Bool:
		{

			bool value = *static_cast<bool*>(vars.second[i].variable);
			dynamic_cast<GUI::Checkbox*>(vars.first->get_element(vars.second[i].name))->set_checked(value);

			break;
		}
		case Var::Type::Dropdown:
		{
			int value = *static_cast<int*>(vars.second[i].variable);
			dynamic_cast<GUI::DropDownList*>(vars.first->get_element(vars.second[i].name))->set_selected_index(value);

			break;
		}
		case Var::Type::FlagDropdown:
		{

			//EnumFlag value = *static_cast<EnumFlag*>(vars.second[i].variable);
			//dynamic_cast<GUI::FlagDropDownList*>(vars.first->get_element(vars.second[i].name))->set_enum_flag(value);

			break;
		}
		case Var::Type::Vector2f:
		{
			Vector2f vec = *static_cast<Vector2f*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION(1) << vec.x;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("x_" + vars.second[i].name))->set_text(s.str());
			s.str("");
			s << PRECISION(1) << vec.y;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("y_" + vars.second[i].name))->set_text(s.str());

			break;
		}
		case Var::Type::Vector2i:
		{
			Vector2i vec = *static_cast<Vector2i*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION(1) << vec.x;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("ix_" + vars.second[i].name))->set_text(s.str());
			s.str("");
			s << PRECISION(1) << vec.y;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("iy_" + vars.second[i].name))->set_text(s.str());

			break;
		}
		case Var::Type::FloatConvex:
		{
			FloatConvex poly = *static_cast<FloatConvex*>(vars.second[i].variable);
			std::vector<Vector2f> model = poly.get_model();
			std::stringstream s;
			for (std::size_t i = 0; i < model.size(); i++)
			{
				s.str("");
				s << PRECISION(1) << model[i].x;
				dynamic_cast<GUI::InputBox*>(vars.first->get_element(std::to_string(i)+"_x_" + vars.second[i].name))->set_text(s.str());
				s.str("");
				s << PRECISION(1) << model[i].y;
				dynamic_cast<GUI::InputBox*>(vars.first->get_element(std::to_string(i) + "_y_" + vars.second[i].name))->set_text(s.str());

			}
			break;
		}
		case Var::Type::Header:
		{
			break;
		}
		case Var::Type::String:
		{
			std::string value = "empty";
			if(vars.second[i].variable)
				value = *static_cast<std::string*>(vars.second[i].variable);
			dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->set_text(value);
			break;
		}
		}

	}
}



}