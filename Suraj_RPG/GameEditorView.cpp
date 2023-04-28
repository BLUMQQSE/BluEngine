#include "pch.h"
#include "core/Input.h"
#include "GameEditorView.h"
#include "core/SceneManager.h"
#include "GameObject.h"
#include "core/ResourceManager.h"
namespace bm98
{
GameEditorView::GameEditorView()
{
	EventSystem::Instance()->subscribe(EventID::SCENE_ADDED_GAMEOBJECT, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_REMOVED_GAMEOBJECT, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_CHANGE, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_GAMEOBJECT_ORDER_CHANGE, this);


	init();
}

GameEditorView::~GameEditorView()
{
	delete heir_panel;
	delete inspec_panel;
}

void GameEditorView::init()
{
	heir_panel = new GUI::Panel(0, 0, Renderer::Instance()->get_window_size().x / 6,
		Renderer::Instance()->get_window_size().y);
	inspec_panel = new GUI::Panel(Renderer::Instance()->get_window_size().x -
		Renderer::Instance()->get_window_size().x / 6, 0,
		Renderer::Instance()->get_window_size().x / 6, Renderer::Instance()->get_window_size().y);
}

void GameEditorView::toggle_editor(EditorPanel panel_to_toggle)
{
	switch (panel_to_toggle)
	{
	case EditorPanel::ALL:
		if (heir_active || inspec_active)
		{
			heir_active = false;
			heir_panel->set_render(false);

			inspec_active = false;
			inspec_panel->set_render(false);
		}
		else
		{
			heir_active = true;
			heir_panel->set_render(true);

			inspec_active = true;
			inspec_panel->set_render(true);
		}
		break;
	case EditorPanel::HEIRARCHY:
		heir_active = !heir_active;
		heir_panel->set_render(heir_active);
		break;
	case EditorPanel::INSPECTOR:
		inspec_active = !inspec_active;
		inspec_panel->set_render(inspec_active);
		break;
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
}

void GameEditorView::update_sfml(sf::Event sfEvent)
{
	if (heir_active)
		heir_panel->update_sfml(sfEvent);
	if (inspec_active)
		inspec_panel->update_sfml(sfEvent);
}

void GameEditorView::handle_event(Event* event)
{
	EventID id = event->get_event_id();


	switch (event->get_event_id())
	{
	case EventID::SCENE_ADDED_GAMEOBJECT:
		create_heir_panel();

		break;
	case EventID::SCENE_REMOVED_GAMEOBJECT:
		create_heir_panel();
		break;
	case EventID::SCENE_CHANGE:
		create_heir_panel();
		break;
	case EventID::SCENE_GAMEOBJECT_ORDER_CHANGE:
		create_heir_panel();
		break;
	}
}

void GameEditorView::create_heir_panel()
{
	// wipe anything previously on the panel
	heir_panel->clear();
	objects_in_scene_map.clear();

	std::vector<GameObject*> objs = SceneManager::Instance()->get_objects_in_scene();

	float x = 0;

	for (std::size_t i = 0; i < objs.size(); i++)
	{
		GameObject* temp = objs[i];

		while (temp->get_parent())
		{
			x += 30;
			temp = temp->get_parent();
		}

		heir_panel->add_element(std::to_string(objs[i]->get_unique_runtime_id()), new GUI::Button(x, 40 * i, 100.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
			objs[i]->get_info().name + " " + std::to_string(objs[i]->get_unique_runtime_id()), 12,
			sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
			sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255)));

		objects_in_scene_map[heir_panel->get_button(std::to_string(objs[i]->get_unique_runtime_id()))] = objs[i];

		x = 0;
	}

	if (!heir_active)
		heir_panel->set_render(false);
	if (!inspec_active)
		inspec_panel->set_render(false);
	if (!selected_gameobject)
		selected_gameobject = objs[0];

}

void GameEditorView::update_heir_panel()
{
	// NOTHING TO UPDATE
	if (objects_in_scene_map.size() == 0)
	{
		selected_gameobject = nullptr;
		return;
	}
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
						selected_gameobject->set_parent(ois.second);
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

	if (Input::Instance()->get_mouse_down(core::Input::Mouse::LEFT))
	{
		for (auto& ois : objects_in_scene_map)
		{
			if (ois.first->mouse_in_bounds())
			{
				selected_gameobject = ois.second;
				gameobject_held = true;	
				create_inspec_panel();
			}
		}
	}
}

void GameEditorView::create_inspec_panel()
{
	inspec_panel->clear();
	variables_in_components.clear();
	std::vector<std::pair<Component*,
		std::vector<bm98::Editor::SerializedVar>>> selected_gameobject_components;

	std::vector<Component*> components = selected_gameobject->get_components();

	for (std::size_t i = 0; i < components.size(); i++)
	{
		selected_gameobject_components.push_back(std::make_pair(components[i], components[i]->get_editor_values()));
	}


	int component_panel_height = 0;

	inspec_panel->add_element(selected_gameobject->get_info().name, create_component_panel(component_panel_height, inspec_panel->get_width(),
		selected_gameobject->get_info().name, selected_gameobject->get_editor_values()));

	component_panel_height += inspec_panel->get_panel(selected_gameobject->get_info().name)->get_height();

	for (std::size_t i = 0; i < selected_gameobject_components.size(); i++)
	{
		std::string component_name = typeid(*components[i]).name();
		component_name = component_name.substr(12);

		inspec_panel->add_element(typeid(*components[i]).name(), create_component_panel(component_panel_height, inspec_panel->get_width(), 
			component_name, selected_gameobject_components[i].second));

		component_panel_height += inspec_panel->get_panel(typeid(*components[i]).name())->get_height();

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
			s << PRECISION << value;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->set_text(s.str());
			break;
		}
		case Var::Type::Float:
		{
			float value = *static_cast<float*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION << value;
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
		case Var::Type::Vector2f:
		{
			Vector2f vec = *static_cast<Vector2f*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION << vec.x;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("x_" + vars.second[i].name))->set_text(s.str());
			s.str("");
			s << PRECISION << vec.y;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("y_" + vars.second[i].name))->set_text(s.str());

			break;
		}
		case Var::Type::Vector2i:
		{
			Vector2i vec = *static_cast<Vector2i*>(vars.second[i].variable);
			std::stringstream s;
			s << PRECISION << vec.x;
			dynamic_cast<GUI::InputBox*>(vars.first->get_element("ix_" + vars.second[i].name))->set_text(s.str());
			s.str("");
			s << PRECISION << vec.y;
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
				s << PRECISION << model[i].x;
				dynamic_cast<GUI::InputBox*>(vars.first->get_element(std::to_string(i)+"_x_" + vars.second[i].name))->set_text(s.str());
				s.str("");
				s << PRECISION << model[i].y;
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