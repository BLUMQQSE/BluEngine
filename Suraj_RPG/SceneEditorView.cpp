#include "pch.h"
#include "Input.h"
#include "SceneEditorView.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextureSelector.h"
#include "TilemapComponent.h"
namespace bm98
{
SceneEditorView::SceneEditorView()
{
	EventSystem::Instance()->subscribe(EventID::SCENE_ADDED_GAMEOBJECT, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_REMOVED_GAMEOBJECT, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_CHANGE, this);
	EventSystem::Instance()->subscribe(EventID::SCENE_GAMEOBJECT_ORDER_CHANGE, this);
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_COMPONENT_ADDED, this);
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_COMPONENT_REMOVED, this);

	init();
	create_scene_editor_panel();

	texture_selector = std::make_unique<GUI::TextureSelector>();
	transform_mover = new GUI::TransformMover(Vector2f(0,0), Vector2f(80, 20));
	transform_mover->set_render(true);
}

SceneEditorView::~SceneEditorView()
{
	delete heir_panel;
	delete inspec_panel;
	delete scene_editor_panel;
	delete transform_mover;
}

void SceneEditorView::init()
{
	heir_panel = new GUI::Panel(0, 0, Renderer::Instance()->get_window_size().x / 6,
		Renderer::Instance()->get_window_size().y);
	inspec_panel = new GUI::Panel(Renderer::Instance()->get_window_size().x -
		Renderer::Instance()->get_window_size().x / 6, 0,
		Renderer::Instance()->get_window_size().x / 6, Renderer::Instance()->get_window_size().y);

	scene_editor_panel = new GUI::Panel(heir_panel->get_width(), Renderer::Instance()->get_window_size().y - 200.f,
		inspec_panel->get_position().x - (heir_panel->get_position().x + heir_panel->get_width()), 200.f);

	heir_panel->set_render(heir_active);
	inspec_panel->set_render(inspec_active);
	scene_editor_panel->set_render(scene_editor_active);

}

const bool SceneEditorView::in_bound() const
{
	if (inspec_active)
	{
		if (inspec_panel->mouse_in_bounds())
			return true;
	}
	if (heir_active)
	{
		if (heir_panel->mouse_in_bounds())
			return true;
	}
	if (scene_editor_active)
	{
		if (scene_editor_panel->mouse_in_bounds())
			return true;
	}

	return false;
}

void SceneEditorView::toggle_editor(EditorPanel panel_to_toggle)
{
	switch (panel_to_toggle)
	{
	case EditorPanel::ALL:
		if (heir_active || inspec_active || scene_editor_active)
		{
			heir_active = false;
			heir_panel->set_render(false);

			inspec_active = false;
			inspec_panel->set_render(false);

			scene_editor_active = false;
			scene_editor_panel->set_render(false);
		}
		else
		{
			heir_active = true;
			heir_panel->set_render(true);

			inspec_active = true;
			inspec_panel->set_render(true);

			scene_editor_active = true;
			scene_editor_panel->set_render(true);
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

void SceneEditorView::update()
{
	for (auto& go : objects_in_scene_map)
	{
		if (go.second->has_component<TilemapComponent>())
		{
			go.second->get_component<TilemapComponent>().update();
		}
	}
	if (selected_gameobject)
	{
		transform_mover->update();
		if (transform_mover->is_held())
		{
			selected_gameobject->set_world_position(transform_mover->get_position());
			create_inspec_panel();
		}
	}
	if (heir_active)
	{
		heir_panel->update();
		update_heir_panel();
	}
	if (inspec_active)
	{
		inspec_panel->update();
		//update_inspec_panel();

		if (selected_gameobject
			&&
			(
				Input::Instance()->get_action_down("ENTER") ||
				Input::Instance()->get_action_down("SPACE")) ||
				Input::Instance()->get_mouse_down() ||
				Input::Instance()->get_mouse_up()
			)
		{
			// Reinitialize gameobject to update any changes which may have occured
			update_inspec_panel();
			selected_gameobject->editor_update();
			transform_mover->set_position(selected_gameobject->get_world_position().x, selected_gameobject->get_world_position().y);
			
		}
	}
	if (scene_editor_active)
	{
		scene_editor_panel->update();
		update_scene_editor_panel();
	}
}

void SceneEditorView::update_sfml(sf::Event sfEvent)
{
	if (heir_active)
		heir_panel->update_sfml(sfEvent);
	if (inspec_active)
		inspec_panel->update_sfml(sfEvent);
	if (scene_editor_active)
		scene_editor_panel->update_sfml(sfEvent);
}

void SceneEditorView::add_to_buffer(sf::View* view)
{
	transform_mover->set_view(view);
}

void SceneEditorView::handle_event(Event* event)
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
	case EventID::GAMEOBJECT_COMPONENT_ADDED:
		if (selected_gameobject)
			create_inspec_panel();
		break;
	case EventID::GAMEOBJECT_COMPONENT_REMOVED:
		if (selected_gameobject)
			create_inspec_panel();
		break;
	}
}

void SceneEditorView::set_selected_gameobject(GameObject* object)
{
	if (!object)
	{
		selected_gameobject = nullptr;
	}
	else
	{
		selected_gameobject = object;
	}
}

void SceneEditorView::create_heir_panel()
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

		GUI::Button* new_button = new GUI::Button(x, 40 * i, 100.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
			objs[i]->get_info().name + " " + std::to_string(objs[i]->get_unique_runtime_id()), 12,
			sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
			sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));

		objects_in_scene_map[new_button] = objs[i];

		heir_panel->add_element(std::to_string(objs[i]->get_unique_runtime_id()), new_button);

		x = 0;
	}

	heir_panel->set_render(heir_active);

	if (!selected_gameobject && objs.size() > 0)
	{
		set_selected_gameobject(objs[0]);
		transform_mover->set_position(selected_gameobject->get_world_position().x, 
					selected_gameobject->get_world_position().y);
		transform_mover->set_render(true);
	}
}

void SceneEditorView::update_heir_panel()
{
	// nothing to update
	if (objects_in_scene_map.size() == 0)
	{
		set_selected_gameobject(nullptr);
		return;
	}
	if (gameobject_held == true && core::Input::Instance()->get_mouse_up())
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
			if (gameobject_held)
			{
				selected_gameobject->set_parent(nullptr);
			}
		}
		else
		{
			gameobject_held = false;
		}
	}

	if (gameobject_held == true && !Input::Instance()->get_mouse())
	{
		gameobject_held = false;
		return;
	}

	if (Input::Instance()->get_mouse_down())
	{
		for (auto& ois : objects_in_scene_map)
		{
			if (ois.first->mouse_in_bounds())
			{
				// TODO: need to reset or clear texture selector
				set_selected_gameobject(ois.second);
				transform_mover->set_position(selected_gameobject->get_world_position().x, 
					selected_gameobject->get_world_position().y);
				transform_mover->set_render(true);
				
				gameobject_held = true;
				create_inspec_panel();
			}
		}
	}
	//heir_panel->set_render(heir_active);
}

void SceneEditorView::clear_inspec_panel()
{
	inspec_panel->clear();
	variables_in_components.clear();
}

void SceneEditorView::create_inspec_panel()
{
	clear_inspec_panel();
	std::vector<std::pair<Component*,
		std::vector<bm98::Editor::SerializedVar>>> selected_gameobject_components;

	std::vector<Component*> components = selected_gameobject->get_components();

	for (std::size_t i = 0; i < components.size(); i++)
	{
		if (components[i]->get_editor_values().size() > 0)
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
	//inspec_panel->set_render(inspec_active);
}

void SceneEditorView::update_inspec_panel()
{
	for (auto& component_panel : variables_in_components)
	{
		update_component_panel(component_panel);
	}
	//inspec_panel->set_render(inspec_active);
}

void SceneEditorView::create_scene_editor_panel()
{
	file_name = new GUI::InputBox(50, 0, 300, 30, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"));

	load_file = new GUI::Button(380, 0, 60.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
		"Load", 12,
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));
	save_file = new GUI::Button(460, 0, 60.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
		"Save", 12,
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));

	component_name = new GUI::InputBox(50, 50, 300, 30, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"));
	add_component = new GUI::Button(380, 50, 60.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
		"Add", 12,
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));
	remove_component = new GUI::Button(460, 50, 60.f, 40.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
		"Remove", 12,
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));

	add_gameobject = new GUI::Button(50, 100, 120.f, 50.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
		"Add GameObject", 12,
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));
	remove_gameobject = new GUI::Button(180, 100, 120.f, 50.f, &ResourceManager::Instance()->get_font("calibri-regular.ttf"),
		"Remove GameObject", 12,
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
		sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));

	scene_editor_panel->add_element("file_name", file_name);
	scene_editor_panel->add_element("load_file", load_file);
	scene_editor_panel->add_element("save_file", save_file);
	scene_editor_panel->add_element("component_name", component_name);
	scene_editor_panel->add_element("add_component", add_component);
	scene_editor_panel->add_element("remove_component", remove_component);
	scene_editor_panel->add_element("add_gameobject", add_gameobject);
	scene_editor_panel->add_element("remove_gameobject", remove_gameobject);

	//scene_editor_panel->set_render(scene_editor_active);

}

void SceneEditorView::update_scene_editor_panel()
{
	if (!scene_editor_panel->mouse_in_bounds())
		return;
	std::string input;
	if (scene_editor_panel->get_button("load_file")->is_pressed())
	{
		input = scene_editor_panel->get_inputbox("file_name")->get_text();
		if (input.find(".json") == std::string::npos)
			return;

		SceneManager::Instance()->load_scene_prefab(input);
		set_selected_gameobject(nullptr);
		create_heir_panel();

	}
	else if (scene_editor_panel->get_button("save_file")->is_pressed())
	{
		SceneManager::Instance()->save_scene_prefab();
	}
	else if (scene_editor_panel->get_button("add_component")->is_pressed())
	{
		if (!selected_gameobject)
			return;
		input = scene_editor_panel->get_inputbox("component_name")->get_text();

		Component* c = 
			selected_gameobject->editor_add_component(input);
		//selected_gameobject->init();
		c->init();
		c->awake();
		c->start();
		selected_gameobject->editor_update();
	}
	else if (scene_editor_panel->get_button("remove_component")->is_pressed())
	{
		if (!selected_gameobject)
			return;

		input = scene_editor_panel->get_inputbox("component_name")->get_text();

		selected_gameobject->editor_remove_component(input);
		create_inspec_panel();
	}
	else if (scene_editor_panel->get_button("add_gameobject")->is_pressed())
	{
		GameObject* go = new GameObject();
		go->get_info().name = "Empty";
		SceneManager::Instance()->instantiate_gameobject(go);
	}
	else if (scene_editor_panel->get_button("remove_gameobject")->is_pressed())
	{
		if (!selected_gameobject)
			return;
		SceneManager::Instance()->destroy_gameobject(selected_gameobject);
		clear_inspec_panel();
		set_selected_gameobject(nullptr);
	}

	//scene_editor_panel->set_render(scene_editor_active);
}

GUI::Panel* SceneEditorView::create_component_panel(float pos_y, float width, std::string component_name
	, std::vector<Editor::SerializedVar> vars)
{
	float height = 0;
	std::vector<std::pair<std::string, GUI::GUIObject*>> items_in_panel;

	items_in_panel.push_back(std::make_pair("title", new GUI::Label(0, 0, 16,
		ResourceManager::Instance()->get_font("calibri-regular.ttf"), component_name)));
	height += 25;

	for (std::size_t i = 0; i < vars.size(); i++)
	{
		switch (vars[i].type)
		{
		case Var::Type::Int:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::InputBox* input_box = new GUI::InputBox(150, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair(vars[i].name, input_box));

			int value = *static_cast<int*>(vars[i].variable);
			std::stringstream s;
			s << PRECISION << value;
			input_box->set_text(s.str());

			height += 20;
			break;
		}
		case Var::Type::Float:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::InputBox* input_box = new GUI::InputBox(150, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair(vars[i].name, input_box));

			float value = *static_cast<float*>(vars[i].variable);
			std::stringstream s;
			s << PRECISION << value;
			input_box->set_text(s.str());

			height += 20;
			break;
		}
		case Var::Type::Bool:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::Checkbox* check_box = new GUI::Checkbox(150, height, 12);

			items_in_panel.push_back(std::make_pair(vars[i].name, check_box));

			bool value = *static_cast<bool*>(vars[i].variable);
			check_box->set_checked(value);

			height += 20;
			break;
		}
		case Var::Type::Dropdown:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::DropDownList* drop_down = new GUI::DropDownList(150, height, 80, 15,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"),
				vars[i].extra_data, 0, 12);

			items_in_panel.push_back(std::make_pair(vars[i].name, drop_down));

			int value = *static_cast<int*>(vars[i].variable);
			drop_down->set_selected_index(value);

			height += 30;
			break;
		}
		case Var::Type::Vector2f:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::InputBox* x = new GUI::InputBox(110, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair("x_" + vars[i].name, x));

			GUI::InputBox* y = new GUI::InputBox(170, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair("y_" + vars[i].name, y));

			Vector2f vec = *static_cast<Vector2f*>(vars[i].variable);
			std::stringstream s;
			s << PRECISION << vec.x;
			x->set_text(s.str());
			s.str("");
			s << PRECISION << vec.y;
			y->set_text(s.str());

			height += 20;
			break;
		}
		case Var::Type::Vector2i:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));

			GUI::InputBox* x = new GUI::InputBox(110, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair("ix_" + vars[i].name, x));

			GUI::InputBox* y = new GUI::InputBox(170, height, 40, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair("iy_" + vars[i].name, y));

			Vector2i vec = *static_cast<Vector2i*>(vars[i].variable);
			std::stringstream s;
			s << PRECISION << vec.x;
			x->set_text(s.str());
			s.str("");
			s << PRECISION << vec.y;
			y->set_text(s.str());

			height += 20;
			break;
		}
		case Var::Type::FloatConvex:
		{
			items_in_panel.push_back(std::make_pair("l_" + vars[i].name, new GUI::Label(0, height, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"), vars[i].name)));
			FloatConvex polygon = *static_cast<FloatConvex*>(vars[i].variable);
			std::cout << polygon.getPointCount() << " points\n";

			FloatConvex poly = *static_cast<FloatConvex*>(vars[i].variable);
			std::vector<Vector2f> model = poly.get_model();
			std::stringstream s;
			for (std::size_t i = 0; i < polygon.getPointCount(); i++)
			{
				GUI::InputBox* x = new GUI::InputBox(110, height, 40, 12, 12,
					ResourceManager::Instance()->get_font("calibri-regular.ttf"));
				items_in_panel.push_back(std::make_pair(std::to_string(i) + "_x_" + vars[i].name, x));
				GUI::InputBox* y = new GUI::InputBox(170, height, 40, 12, 12,
					ResourceManager::Instance()->get_font("calibri-regular.ttf"));
				items_in_panel.push_back(std::make_pair(std::to_string(i) + "_y_" + vars[i].name, y));

				s.str("");
				s << PRECISION << model[i].x;
				x->set_text(s.str());
				s.str("");
				s << PRECISION << model[i].y;
				y->set_text(s.str());

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

			GUI::InputBox* input_box = new GUI::InputBox(70, height, 140, 12, 12,
				ResourceManager::Instance()->get_font("calibri-regular.ttf"));

			items_in_panel.push_back(std::make_pair(vars[i].name, input_box));

			std::string value = "empty";
			if (vars[i].variable)
				value = *static_cast<std::string*>(vars[i].variable);
			input_box->set_text(value);

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

void SceneEditorView::update_component_panel(std::pair<GUI::Panel*, std::vector<Editor::SerializedVar>> vars)
{
	for (std::size_t i = 0; i < vars.second.size(); i++)
	{
		// here handle updating for all objects in scene

		if (vars.second[i].variable == nullptr)
			continue;

		switch (vars.second[i].type)
		{
		case Var::Type::Int:
		{
			int value = dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->get_text_value();
			*static_cast<int*>(vars.second[i].variable) = value;

			break;
		}
		case Var::Type::Float:
		{
			float value = dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->get_text_value();
			*static_cast<float*>(vars.second[i].variable) = value;

			break;
		}
		case Var::Type::Bool:
		{
			bool value = dynamic_cast<GUI::Checkbox*>(vars.first->get_element(vars.second[i].name))->is_checked();

			*static_cast<bool*>(vars.second[i].variable) = value;

			break;
		}
		case Var::Type::Dropdown:
		{
			int value = dynamic_cast<GUI::DropDownList*>(vars.first->get_element(vars.second[i].name))->get_selected_index();
			*static_cast<int*>(vars.second[i].variable) = value;

			break;
		}
		case Var::Type::Vector2f:
		{

			float x_value = dynamic_cast<GUI::InputBox*>(vars.first->get_element("x_" + vars.second[i].name))->get_text_value();
			float y_value = dynamic_cast<GUI::InputBox*>(vars.first->get_element("y_" + vars.second[i].name))->get_text_value();

			*static_cast<Vector2f*>(vars.second[i].variable) = Vector2f(x_value, y_value);
			break;
		}
		case Var::Type::Vector2i:
		{
			int x_value = (int)dynamic_cast<GUI::InputBox*>(vars.first->get_element("ix_" + vars.second[i].name))->get_text_value();
			int y_value = (int)dynamic_cast<GUI::InputBox*>(vars.first->get_element("iy_" + vars.second[i].name))->get_text_value();

			*static_cast<Vector2i*>(vars.second[i].variable) = Vector2i(x_value, y_value);

			break;
		}
		case Var::Type::FloatConvex:
		{

			FloatConvex poly = *static_cast<FloatConvex*>(vars.second[i].variable);
			std::vector<Vector2f> model = poly.get_model();

			for (std::size_t i = 0; i < model.size(); i++)
			{
				float x_value = dynamic_cast<GUI::InputBox*>(vars.first->get_element(std::to_string(i) + "_x_" + vars.second[i].name))->get_text_value();
				float y_value = dynamic_cast<GUI::InputBox*>(vars.first->get_element(std::to_string(i) + "_y_" + vars.second[i].name))->get_text_value();

				poly.setPoint(i, Vector2f(x_value, y_value));
			}

			*static_cast<FloatConvex*>(vars.second[i].variable) = poly;

			break;
		}
		case Var::Type::Header:
		{
			break;
		}
		case Var::Type::String:
		{
			std::string value = dynamic_cast<GUI::InputBox*>(vars.first->get_element(vars.second[i].name))->get_text();
			*static_cast<std::string*>(vars.second[i].variable) = value;
			break;
		}
		}

	}
}



}