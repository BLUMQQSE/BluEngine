#include "pch.h"
#include "SceneEditor.h"
#include "globals.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Gui.h"
#include "Input.h"
#include "Debug.h"
#include "Scene.h"

#include "Component.h"
#include "SpriteComponent.h"
#include "TilemapComponent.h"
#include "AnimatedSpriteComponent.h"
#include "AnimationComponent.h"
#include "ChildAnimationComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"

namespace bm98
{

SceneEditor::SceneEditor(sf::Font* font)
	:font(*font)
{
	gameobject_editor = new Panel(200, 100, 420, 800);
	gameobject_selector = new Panel(1400, 200, 400, 600);
	gameobject_heirarchy = new Panel(0, 100, 200, 800);
	
	scene = new Scene();

	scene_name = new InputBox(2200, 50, 300, 50, 32, *font);

	gameobject_heirarchy->toggle_active();
	gameobject_editor->toggle_active();
	gameobject_selector->toggle_active();

	create_gameobject_selector();
	create_empty_gameobject_editor();

	for (auto& c : editor_panels)
		std::cout << c.first << "\n";
}

SceneEditor::~SceneEditor()
{
	delete gameobject_editor;
	delete gameobject_selector;
	delete gameobject_heirarchy;
	delete scene;
	delete scene_name;
	for (auto& b : editor_buttons)
		delete b.second;
}

void SceneEditor::update_sfml(sf::Event sfEvent)
{
	gameobject_editor->update_sfml(sfEvent);
	gameobject_selector->update_sfml(sfEvent);
	gameobject_heirarchy->update_sfml(sfEvent);
	scene_name->update_sfml(sfEvent);
}

void SceneEditor::update()
{	
	gameobject_editor->update();
	gameobject_editor->display_mouse_pos(font);
	gameobject_selector->update();
	gameobject_heirarchy->update();
	scene_name->update();

	if (Input::get_action_down("ENTER") || Input::get_mouse_down(Input::Mouse::LEFT))
	{
		update_editor();
		update_selector();
	}

	

	if (!gameobject_editor->is_active())
	{
		for (auto& o : scene->get_objects())
		{
			//if (o->is_pressed())
			//	selected_gameobject = o;
		}
	}
	else
	{
		// TODO: here we will check to change selected_component
	}
	
	

	//if (!selected_gameobject)
		//gameobject_editor->clear();
	for (auto& b : editor_buttons)
		b.second->update();

	if (editor_buttons.at("SAVE_SCENE")->is_pressed())
		save_scene();
	

	for (auto p : editor_panels)
		if(p.second->get_dropdown("delete"))
			if (p.second->get_dropdown("delete")->get_selected_index() == 1)
			{
				remove_component(p.first);
				break;
			}

	
}

void SceneEditor::render(sf::View* view)
{
	if (gameobject_editor->is_active())
		gameobject_editor->add_to_buffer();
	if (gameobject_selector->is_active())
		gameobject_selector->add_to_buffer();
	for (auto& b : editor_buttons)
		b.second->add_to_buffer();

	scene->render(view);
	scene_name->add_to_buffer();
}

void SceneEditor::remove_component(std::string key)
{
	// need to shuffle all panels after panel[key] up to its position

	// iterate reverse over editor_panel_order, use their key 
	for (auto iter = editor_panel_order.rbegin(); iter != editor_panel_order.rend(); ++iter)
	{
		if (iter->second == key)
			break;
		
		editor_panels.at(iter->second)->set_position(editor_panels.at(iter->second)->get_position().x,
			editor_panels.at(iter->second)->get_position().y - editor_panels.at(std::next(iter)->second)->get_height());
	}

	if (key == typeid(SpriteComponent).name())
		selected_gameobject->remove_component<SpriteComponent>(&selected_gameobject->get_component<SpriteComponent>());


	editor_panels.at(key)->clear();
	gameobject_editor->remove_element(key);
	editor_panels.erase(key);
	
}

#pragma region Create Panels

void SceneEditor::create_sprite_component_panel()
{
	if (selected_gameobject->has_component<SpriteComponent>())
		return;
	
	selected_gameobject->add_component<SpriteComponent>();

	float y = get_next_y();
	editor_panels[typeid(SpriteComponent).name()] = new Panel(0, y, 420, 120);
	Label* name = new Label(0, 0, 20, font, "SpriteComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });
	Label* label = new Label(0, 30, 18, font, "Sprite file path:");
	InputBox* input = new InputBox(130, 30, 250, 20, 16, font);

	Label* layer = new Label(0, 60, 18, font, "Sorting Layer:");
	DropDownList* dropdown = new DropDownList(250, 60, 150, 20, font, Global::layers_to_vector(), 0, 12);
	Label* order = new Label(0, 90, 18, font, "Order in the layer:");
	InputBox* z = new InputBox(300, 90, 40, 20, 16, font);

	editor_panels.at(typeid(SpriteComponent).name())->add_element("delete", del);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("name", name);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("sprite", label);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("sprite_file_path", input);

	editor_panels.at(typeid(SpriteComponent).name())->add_element("layer", layer);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("layer_drop", dropdown);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("order", order);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("order_input", z);

	gameobject_editor->add_element(typeid(SpriteComponent).name(), editor_panels.at(typeid(SpriteComponent).name()));
	editor_panel_order.push_back({ editor_panel_order.size(), typeid(SpriteComponent).name() });
	
}

void SceneEditor::create_boxcollider_component_panel()
{
	if (selected_gameobject->has_component<BoxColliderComponent>())
		return;
	selected_gameobject->add_component<BoxColliderComponent>();

	float y = get_next_y();
	editor_panels[typeid(BoxColliderComponent).name()] = new Panel(0, y, 420, 210);
	Label* name = new Label(0, 0, 20, font, "BoxColliderComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });

	Label* width = new Label(0, 30, 18, font, "width:");
	InputBox* i_width = new InputBox(330, 30, 60, 20, 16, font);
	Label* height = new Label(0, 60, 18, font, "height:");
	InputBox* i_height = new InputBox(330, 60, 60, 20, 16, font);
	Label* x_off = new Label(0, 90, 18, font, "offsetX:");
	InputBox* i_x = new InputBox(330, 90, 60, 20, 16, font);
	Label* y_off = new Label(0, 120, 18, font, "offsetY:");
	InputBox* i_y = new InputBox(330, 120, 60, 20, 16, font);

	Label* trigger = new Label(0, 150, 18, font, "trigger:");
	Checkbox* trig_check = new Checkbox(330, 150, 10);
	Label* coll = new Label(0, 180, 18, font, "collision type:");
	DropDownList* coll_drop = new DropDownList(330, 180, 40, 20, font, collisiondetection_to_vector());

	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("delete", del);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("name", name);

	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("width", width);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("width_input", i_width);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("height", height);	
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("height_input", i_height);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("x_offset", x_off);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("x_offset_input", i_x);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("y_offset", y_off);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("y_offset_input", i_y);

	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("trigger", trigger);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("trigger_check", trig_check);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("collision_type", coll);
	editor_panels.at(typeid(BoxColliderComponent).name())->add_element("collision_type_drop", coll_drop);

	gameobject_editor->add_element(typeid(BoxColliderComponent).name(), editor_panels.at(typeid(BoxColliderComponent).name()));
	editor_panel_order.push_back({ editor_panel_order.size(), typeid(BoxColliderComponent).name() });

}



#pragma endregion

#pragma region Update Panels

void SceneEditor::update_sprite_component_panel()
{
	if (editor_panels.count(typeid(SpriteComponent).name()) == 0)
		return;

	selected_gameobject->get_component<SpriteComponent>().set_sprite(
		editor_panels.at(
		typeid(SpriteComponent).name())->get_inputbox("sprite_file_path")->get_text());
	selected_gameobject->get_component<SpriteComponent>().set_layer(
		Global::string_to_layer(editor_panels.at(typeid(SpriteComponent).name())->get_dropdown("layer_drop")->get_selected_button()->get_text()));
	selected_gameobject->get_component<SpriteComponent>().set_z_order(
		static_cast<int>(editor_panels.at(typeid(SpriteComponent).name())->get_inputbox("order_input")->get_text_value()));
		
}

void SceneEditor::update_boxcollider_component_panel()
{
	if (editor_panels.count(typeid(BoxColliderComponent).name()) == 0)
		return;

	selected_gameobject->get_component<BoxColliderComponent>().set_trigger(
		editor_panels.at(typeid(BoxColliderComponent).name())->get_checkbox("trigger_check")->is_checked());

	selected_gameobject->get_component<BoxColliderComponent>().set_collision_detection(
		string_to_collisiondetection(editor_panels.at(typeid(BoxColliderComponent).name())->get_dropdown("collision_type_drop")->get_selected_button()->get_text()));
	
	selected_gameobject->get_component<BoxColliderComponent>().set_hitbox(
		selected_gameobject->get_transform().position.x,
		selected_gameobject->get_transform().position.y,
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("width_input")->get_text_value(),
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("height_input")->get_text_value(),
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("x_offset_input")->get_text_value(),
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("y_offset_input")->get_text_value()
		);
}

#pragma endregion



void SceneEditor::create_empty_gameobject_editor()
{
	ScrollView* sv = new ScrollView(0, 0, 420, 5000, false);
	gameobject_editor->add_scroll_view("scroll", sv);
#pragma region Info
	
	editor_panels["info_panel"] = new Panel(0, 0, 420, 80);

	DropDownList* del = new DropDownList(390, 5, 0, 0, font, { "" });

	//Label* name = new Label(100, 5, 24, font, "GameObject");
	InputBox* name = new InputBox(50, 5, 300, 30, 24, font);
	Label* tag = new Label(25, 40, 18, font, "Tag:");
	DropDownList* tag_list = new DropDownList(85, 40, 100, 30, font, Global::tags_to_vector());
	Label* layer = new Label(225, 40, 18, font, "Layer:");
	DropDownList* layer_list = new DropDownList(285, 40, 100, 30, font, Global::physics_layer_to_vector());

	editor_panels.at("info_panel")->add_element("name_input", name);
	editor_panels.at("info_panel")->add_element("tag", tag);
	editor_panels.at("info_panel")->add_element("tag_drop", tag_list);
	editor_panels.at("info_panel")->add_element("layer", layer);
	editor_panels.at("info_panel")->add_element("layer_drop", layer_list);
	editor_panels.at("info_panel")->add_element("delete", del);
	
#pragma endregion

#pragma region Transform

	editor_panels["transform_panel"] = new Panel(0, 80, 420, 85);

	del = new DropDownList(390, 5, 0, 0, font, {""});

	Label* position = new Label(0, 5, 18, font, "Position");

	Label* pos_x = new Label(180, 5, 18, font, "x:");
	Label* pos_y = new Label(300, 5, 18, font, "y:");
	InputBox* ipos_x = new InputBox(210, 8, 60, 20, 16, font);
	InputBox* ipos_y = new InputBox(330, 8, 60, 20, 16, font);

	Label* rotation = new Label(0, 30, 18, font, "Rotation");

	Label* rot_x = new Label(180, 30, 18, font, "x:");
	Label* rot_y = new Label(300, 30, 18, font, "y:");
	InputBox* irot_x = new InputBox(210, 33, 60, 20, 16, font);
	InputBox* irot_y = new InputBox(330, 33, 60, 20, 16, font);

	Label* scale = new Label(0, 55, 18, font, "Scale");

	Label* sca_x = new Label(180, 55, 18, font, "x:");
	Label* sca_y = new Label(300, 55, 18, font, "y:");
	InputBox* isca_x = new InputBox(210, 58, 60, 20, 16, font);
	InputBox* isca_y = new InputBox(330, 58, 60, 20, 16, font);

	editor_panels.at("transform_panel")->add_element("position", position);

	editor_panels.at("transform_panel")->add_element("pos_x", pos_x);
	editor_panels.at("transform_panel")->add_element("pos_y", pos_y);
	editor_panels.at("transform_panel")->add_element("pos_x_input", ipos_x);
	editor_panels.at("transform_panel")->add_element("pos_y_input", ipos_y);

	editor_panels.at("transform_panel")->add_element("rotation", rotation);

	editor_panels.at("transform_panel")->add_element("rot_x", rot_x);
	editor_panels.at("transform_panel")->add_element("rot_y", rot_y);
	editor_panels.at("transform_panel")->add_element("rot_x_input", irot_x);
	editor_panels.at("transform_panel")->add_element("rot_y_input", irot_y);

	editor_panels.at("transform_panel")->add_element("scale", scale);

	editor_panels.at("transform_panel")->add_element("scale_x", sca_x);
	editor_panels.at("transform_panel")->add_element("scale_y", sca_y);
	editor_panels.at("transform_panel")->add_element("scale_x_input", isca_x);
	editor_panels.at("transform_panel")->add_element("scale_y_input", isca_y);
	editor_panels.at("transform_panel")->add_element("delete", del);

#pragma endregion

	gameobject_editor->add_element("info_panel", editor_panels.at("info_panel"));
	editor_panel_order.push_back({ editor_panel_order.size(), "info_panel"});
	gameobject_editor->add_element("transform_panel", editor_panels.at("transform_panel"));
	editor_panel_order.push_back({ editor_panels.size(), "transform_panel"});


	editor_buttons["SAVE_SCENE"] = new Button(2510, 50, 100, 50, &font, "Save", 30, 
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));
	
	editor_buttons["ADD_COMPONENT"] = new Button(230, 840, 100, 50, &font, "Add", 30,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

}

float SceneEditor::get_next_y()
{
	float y = 0;
	for (auto& c : editor_panels)
		y += c.second->get_height();
	
	return y;
}

void SceneEditor::update_editor()
{
	//issue is here
	if (!selected_gameobject)
		return;

	selected_gameobject->info.name = gameobject_editor->get_panel("info_panel")->get_inputbox("name_input")->get_text();

	selected_gameobject->info.tag = Global::string_to_tag(
		gameobject_editor->get_panel("info_panel")->get_dropdown("tag_drop")->get_selected_button()->get_text());

	selected_gameobject->info.layer = Global::string_to_physics_layer(
		gameobject_editor->get_panel("info_panel")->get_dropdown("layer_drop")->get_selected_button()->get_text());

	selected_gameobject->set_position(
		gameobject_editor->get_panel("transform_panel")->get_inputbox("pos_x_input")->get_text_value(),
		gameobject_editor->get_panel("transform_panel")->get_inputbox("pos_y_input")->get_text_value()
		);
	selected_gameobject->transform.rotation = sf::Vector2f(
		gameobject_editor->get_panel("transform_panel")->get_inputbox("rot_x_input")->get_text_value(),
		gameobject_editor->get_panel("transform_panel")->get_inputbox("rot_y_input")->get_text_value()
	);
	selected_gameobject->transform.scale = sf::Vector2f(
		gameobject_editor->get_panel("transform_panel")->get_inputbox("scale_x_input")->get_text_value(),
		gameobject_editor->get_panel("transform_panel")->get_inputbox("scale_y_input")->get_text_value()
	);

	update_sprite_component_panel();
	update_boxcollider_component_panel();
}

void SceneEditor::update_selector()
{
	if (addable_gameobjects.at("empty_gameobject")->is_pressed())
	{
		selected_gameobject = new GameObject();
		scene->insert_gameobject(selected_gameobject);
	}
	
	
	if (!selected_gameobject)
		return;

	if (addable_components.at(typeid(SpriteComponent).name())->is_pressed())
		create_sprite_component_panel();
	if (addable_components.at(typeid(BoxColliderComponent).name())->is_pressed())
		create_boxcollider_component_panel();
	
}

void SceneEditor::create_gameobject_selector()
{
	std::cout << "HAWIKDNHAWNDIKAWD\n";
	addable_gameobjects["empty_gameobject"] = new Button(50, 10, 300, 60, &font, "Empty GameObject", 30,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));
	
	gameobject_selector->add_element("empty_gameobject", addable_gameobjects.at("empty_gameobject"));

	gameobject_selector->add_element("component_panel", new Panel(0, 200, 400, 400));
	ScrollView* sv = new ScrollView(0, 0, 400, 500, false);
	gameobject_selector->get_panel("component_panel")->add_scroll_view("scroll", sv);

	addable_components[typeid(SpriteComponent).name()] = new Button(50, 0, 150, 25, &font, "SpriteComponent", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	gameobject_selector->get_panel("component_panel")->add_element(typeid(SpriteComponent).name(), addable_components.at(typeid(SpriteComponent).name()));

	addable_components[typeid(BoxColliderComponent).name()] = new Button(50, 50, 150, 25, &font, "BoxColliderComponent", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	gameobject_selector->get_panel("component_panel")->add_element(typeid(BoxColliderComponent).name(), addable_components.at(typeid(BoxColliderComponent).name()));
	
	

}

void SceneEditor::save_scene()
{
	scene->set_name("Unnamed.json");
	if(scene_name->get_text() != "")
		scene->set_name(scene_name->get_text() + ".json");
	SceneManager::save_scene(scene);
}

}