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
	gameobject_editor = new Panel(400, 100, 420, 800);
	gameobject_selector = new Panel(1400, 200, 400, 600);
	gameobject_heirarchy = new Panel(200, 100, 200, 800);
	
	scene = new Scene();

	scene_name = new InputBox(2200, 50, 300, 50, 32, *font);

	gameobject_heirarchy->toggle_active();
	gameobject_editor->toggle_active();
	gameobject_selector->toggle_active();

	create_gameobject_selector();
	create_empty_gameobject_editor();

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
	for (auto& gis : gameobjects_in_scene)
		delete gis.first;
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

	
	update_editor();
	update_selector();
	

	update_heirarchy();

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
	Renderer::refresh();

	if (gameobject_editor->is_active())
		gameobject_editor->add_to_buffer();
	if (gameobject_selector->is_active())
		gameobject_selector->add_to_buffer();
	if (gameobject_heirarchy->is_active())
		gameobject_heirarchy->add_to_buffer();
	for (auto& b : editor_buttons)
		b.second->add_to_buffer();

	scene->render(view);
	scene_name->add_to_buffer();
}

void SceneEditor::remove_component(std::string key)
{
	// moving all panels underneath removed component up
	for (auto iter = editor_panel_order.rbegin(); iter != editor_panel_order.rend(); ++iter)
	{
		if (iter->second == key)
		{
			break;
		}
		editor_panels.at(iter->second)->set_position(editor_panels.at(iter->second)->get_position().x,
			editor_panels.at(iter->second)->get_position().y - editor_panels.at(std::next(iter)->second)->get_height());
	}

	if (key == typeid(SpriteComponent).name())
		selected_gameobject->remove_component<SpriteComponent>(&selected_gameobject->get_component<SpriteComponent>());
	if (key == typeid(BoxColliderComponent).name())
		selected_gameobject->remove_component<BoxColliderComponent>(&selected_gameobject->get_component<BoxColliderComponent>());
	if (key == typeid(AnimationComponent).name())
		selected_gameobject->remove_component<AnimationComponent>(&selected_gameobject->get_component<AnimationComponent>());
	if (key == typeid(ChildAnimationComponent).name())
		selected_gameobject->remove_component<ChildAnimationComponent>(&selected_gameobject->get_component<ChildAnimationComponent>());
	if (key == typeid(AnimatedSpriteComponent).name())
		selected_gameobject->remove_component<AnimatedSpriteComponent>(&selected_gameobject->get_component<AnimatedSpriteComponent>());


	editor_panels.at(key)->clear();
	gameobject_editor->remove_element(key);
	editor_panels.erase(key);
}

void SceneEditor::create_gameobject()	
{
	selected_gameobject = new GameObject();

	float y = get_next_gameobject_y();

	// TODO: Add gameobject as to heirarchy
	Button* b = new Button(0, y, 100, 30, &font, "Gameobject", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));
	gameobjects_in_scene[b] = { {b, selected_gameobject} };
		
	// need to figure out how to name this
	gameobject_heirarchy->add_element(std::to_string(selected_gameobject->get_unique_runtime_id()), b);

	update_heirarchy_positions();
	clear_editor();
	populate_editor();

	scene->insert_gameobject(selected_gameobject);
}

#pragma region Create Panels

void SceneEditor::create_info_panel()
{
	editor_panels["info_panel"] = new Panel(0, 0, 420, 80);

	DropDownList* del = new DropDownList(390, 5, 0, 0, font, { "" });

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

	gameobject_editor->add_element("info_panel", editor_panels.at("info_panel"));
	editor_panel_order.push_back({ editor_panel_order.size(), "info_panel" });

	if (selected_gameobject)
	{
		name->set_text(selected_gameobject->get_info().name);
		tag_list->set_selected_index(static_cast<int>(selected_gameobject->get_info().tag));
		layer_list->set_selected_index(static_cast<int>(selected_gameobject->get_info().layer));
	}

}

void SceneEditor::create_transform_panel()
{
	editor_panels["transform_panel"] = new Panel(0, 80, 420, 85);

	DropDownList* del = new DropDownList(390, 5, 0, 0, font, { "" });

	Label* position = new Label(5, 5, 18, font, "Position");

	Label* pos_x = new Label(180, 5, 18, font, "x:");
	Label* pos_y = new Label(300, 5, 18, font, "y:");
	InputBox* ipos_x = new InputBox(210, 8, 60, 20, 16, font);
	InputBox* ipos_y = new InputBox(330, 8, 60, 20, 16, font);

	Label* rotation = new Label(5, 30, 18, font, "Rotation");

	Label* rot_x = new Label(180, 30, 18, font, "x:");
	Label* rot_y = new Label(300, 30, 18, font, "y:");
	InputBox* irot_x = new InputBox(210, 33, 60, 20, 16, font);
	InputBox* irot_y = new InputBox(330, 33, 60, 20, 16, font);

	Label* scale = new Label(5, 55, 18, font, "Scale");

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

	gameobject_editor->add_element("transform_panel", editor_panels.at("transform_panel"));
	editor_panel_order.push_back({ editor_panels.size(), "transform_panel" });

	if (selected_gameobject)
	{
		std::stringstream i;

		//i << std::fixed << std::setprecision(2) << selected_gameobject->get_transform().position.x;
		ipos_x->set_text(i.str());
		i.str("");
		//i << std::fixed << std::setprecision(2) << selected_gameobject->get_transform().position.y;
		ipos_y->set_text(i.str());
		i.str("");
		//i << std::fixed << std::setprecision(2) << selected_gameobject->get_transform().rotation.x;

		irot_x->set_text(i.str());
		i.str("");
		//i << std::fixed << std::setprecision(2) << selected_gameobject->get_transform().rotation.y;
		irot_y->set_text(i.str());
		i.str("");
		//i << std::fixed << std::setprecision(2) << selected_gameobject->get_transform().scale.x;
		isca_x->set_text(i.str());
		i.str("");
		//i << std::fixed << std::setprecision(2) << selected_gameobject->get_transform().scale.y;
		isca_y->set_text(i.str());
	}
}

void SceneEditor::create_sprite_component_panel()
{
	SpriteComponent sc = selected_gameobject->get_component<SpriteComponent>();
	float y = get_next_component_y();
	editor_panels[typeid(SpriteComponent).name()] = new Panel(0, y, 420, 150);
	Label* name = new Label(5, 2, 20, font, "SpriteComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });
	Label* label = new Label(5, 30, 18, font, "Sprite file path:");
	InputBox* input = new InputBox(130, 30, 250, 20, 16, font);
	input->set_text(sc.get_file_path());

	Label* size = new Label(5, 60, 18, font, "Size");
	Label* size_x = new Label(180, 60, 18, font, "x:");
	InputBox* i_size_x = new InputBox(210, 60, 60, 20, 16, font);
	i_size_x->set_text(std::to_string(sc.get_size().x));
	Label* size_y = new Label(300, 60, 18, font, "y:");
	InputBox* i_size_y = new InputBox(330, 60, 60, 20, 16, font);
	i_size_y->set_text(std::to_string(sc.get_size().y));

	Label* layer = new Label(5, 90, 18, font, "Sorting Layer:");
	DropDownList* dropdown = new DropDownList(250, 90, 150, 20, font, Global::layers_to_vector(), 0, 12);
	dropdown->set_selected_index(static_cast<int>(sc.get_sorting_layer()));
	Label* order = new Label(5, 120, 18, font, "Order in layer:");
	InputBox* z = new InputBox(300, 120, 40, 20, 16, font);
	z->set_text(std::to_string(sc.get_z_order()));

	editor_panels.at(typeid(SpriteComponent).name())->add_element("delete", del);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("name", name);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("sprite", label);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("sprite_file_path", input);

	editor_panels.at(typeid(SpriteComponent).name())->add_element("size", size);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("size_x", size_x);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("size_y", size_y);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("size_x_input", i_size_x);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("size_y_input", i_size_y);

	editor_panels.at(typeid(SpriteComponent).name())->add_element("layer", layer);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("layer_drop", dropdown);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("order", order);
	editor_panels.at(typeid(SpriteComponent).name())->add_element("order_input", z);

	gameobject_editor->add_element(typeid(SpriteComponent).name(), editor_panels.at(typeid(SpriteComponent).name()));
	editor_panel_order.push_back({ editor_panel_order.size(), typeid(SpriteComponent).name() });
}

void SceneEditor::create_boxcollider_component_panel()
{
	float y = get_next_component_y();
	editor_panels[typeid(BoxColliderComponent).name()] = new Panel(0, y, 420, 210);
	Label* name = new Label(5, 2, 20, font, "BoxColliderComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });

	Label* width = new Label(5, 30, 18, font, "width:");
	InputBox* i_width = new InputBox(330, 30, 60, 20, 16, font);
	Label* height = new Label(5, 60, 18, font, "height:");
	InputBox* i_height = new InputBox(330, 60, 60, 20, 16, font);
	Label* x_off = new Label(5, 90, 18, font, "offsetX:");
	InputBox* i_x = new InputBox(330, 90, 60, 20, 16, font);
	Label* y_off = new Label(5, 120, 18, font, "offsetY:");
	InputBox* i_y = new InputBox(330, 120, 60, 20, 16, font);

	Label* trigger = new Label(5, 150, 18, font, "trigger:");
	Checkbox* trig_check = new Checkbox(330, 150, 10);
	Label* coll = new Label(5, 180, 18, font, "collision type:");
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

void SceneEditor::create_animation_component_panel()
{
	float y = get_next_component_y();
	editor_panels[typeid(AnimationComponent).name()] = new Panel(0, y, 420, 50);
	Label* name = new Label(5, 2, 20, font, "AnimationComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });


	editor_panels.at(typeid(AnimationComponent).name())->add_element("delete", del);
	editor_panels.at(typeid(AnimationComponent).name())->add_element("name", name);


	gameobject_editor->add_element(typeid(AnimationComponent).name(), editor_panels.at(typeid(AnimationComponent).name()));
	editor_panel_order.push_back({ editor_panel_order.size(), typeid(AnimationComponent).name() });
}

void SceneEditor::create_childanimation_component_panel()
{
	float y = get_next_component_y();
	editor_panels[typeid(ChildAnimationComponent).name()] = new Panel(0, y, 420, 50);
	Label* name = new Label(5, 2, 20, font, "ChildAnimationComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });


	editor_panels.at(typeid(ChildAnimationComponent).name())->add_element("delete", del);
	editor_panels.at(typeid(ChildAnimationComponent).name())->add_element("name", name);


	gameobject_editor->add_element(typeid(ChildAnimationComponent).name(), editor_panels.at(typeid(ChildAnimationComponent).name()));
	editor_panel_order.push_back({ editor_panel_order.size(), typeid(ChildAnimationComponent).name() });
}

void SceneEditor::create_animatedsprite_component_panel()
{
	float y = get_next_component_y();
	editor_panels[typeid(AnimatedSpriteComponent).name()] = new Panel(0, y, 420, 50);
	Label* name = new Label(5, 2, 20, font, "AnimatedSpriteComponent");
	DropDownList* del = new DropDownList(390, 5, 20, 20, font, { "-", "X" });


	editor_panels.at(typeid(AnimatedSpriteComponent).name())->add_element("delete", del);
	editor_panels.at(typeid(AnimatedSpriteComponent).name())->add_element("name", name);


	gameobject_editor->add_element(typeid(AnimatedSpriteComponent).name(), editor_panels.at(typeid(AnimatedSpriteComponent).name()));
	editor_panel_order.push_back({ editor_panel_order.size(), typeid(AnimatedSpriteComponent).name() });
}


#pragma endregion


#pragma region Update Panels

void SceneEditor::update_sprite_component_panel()
{
	if (editor_panels.count(typeid(SpriteComponent).name()) == 0)
		return;
	
	SpriteComponent* sc = &selected_gameobject->get_component<SpriteComponent>();

	sc->set_sprite(
		editor_panels.at(
		typeid(SpriteComponent).name())->get_inputbox("sprite_file_path")->get_text());
	sc->set_sorting_layer(
		Global::string_to_layer(editor_panels.at(typeid(SpriteComponent).name())->get_dropdown("layer_drop")->get_selected_button()->get_text()));
	sc->set_z_order(
		static_cast<int>(editor_panels.at(typeid(SpriteComponent).name())->get_inputbox("order_input")->get_text_value()));
	
	sc->set_size(static_cast<int>(editor_panels.at(typeid(SpriteComponent).name())->get_inputbox("size_x_input")->get_text_value()),
		static_cast<int>(editor_panels.at(typeid(SpriteComponent).name())->get_inputbox("size_y_input")->get_text_value()));
		
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
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("width_input")->get_text_value(),
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("height_input")->get_text_value(),
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("x_offset_input")->get_text_value(),
		editor_panels.at(typeid(BoxColliderComponent).name())->get_inputbox("y_offset_input")->get_text_value()
		);
}

void SceneEditor::update_animation_component_panel()
{
	if (editor_panels.count(typeid(AnimationComponent).name()) == 0)
		return;
}

void SceneEditor::update_childanimation_component_panel()
{
	if (editor_panels.count(typeid(ChildAnimationComponent).name()) == 0)
		return;
}

void SceneEditor::update_animatedsprite_component_panel()
{
	if (editor_panels.count(typeid(AnimatedSpriteComponent).name()) == 0)
		return;
}

#pragma endregion

void SceneEditor::update_info_panel()
{

	selected_gameobject->info.name = gameobject_editor->get_panel("info_panel")->get_inputbox("name_input")->get_text();

	selected_gameobject->info.tag = Global::string_to_tag(
		gameobject_editor->get_panel("info_panel")->get_dropdown("tag_drop")->get_selected_button()->get_text());

	selected_gameobject->info.layer = Global::string_to_physics_layer(
		gameobject_editor->get_panel("info_panel")->get_dropdown("layer_drop")->get_selected_button()->get_text());
}

void SceneEditor::update_transform_panel()
{
	float pos_x = gameobject_editor->get_panel("transform_panel")->get_inputbox("pos_x_input")->get_text_value();
	float pos_y = gameobject_editor->get_panel("transform_panel")->get_inputbox("pos_y_input")->get_text_value();

	selected_gameobject->set_local_position(
		Vector2f(
			gameobject_editor->get_panel("transform_panel")->get_inputbox("pos_x_input")->get_text_value(),
			gameobject_editor->get_panel("transform_panel")->get_inputbox("pos_y_input")->get_text_value()
		)
	);
	/*
	selected_gameobject->get_local_rotation(
		Vector2f(
		Vector2f(
		gameobject_editor->get_panel("transform_panel")->get_inputbox("rot_x_input")->get_text_value(),
		gameobject_editor->get_panel("transform_panel")->get_inputbox("rot_y_input")->get_text_value()
		)
	);
	selected_gameobject->transform.scale = sf::Vector2f(
		gameobject_editor->get_panel("transform_panel")->get_inputbox("scale_x_input")->get_text_value(),
		gameobject_editor->get_panel("transform_panel")->get_inputbox("scale_y_input")->get_text_value()
	);
	*/
}

void SceneEditor::clear_editor()
{
	for (auto& ep : editor_panels)
	{
		//STILL ISSUE HERE
		gameobject_editor->remove_element(ep.first);
		delete ep.second;
	}
	
	//editor_panels.clear();
}

void SceneEditor::populate_editor()
{
	
	create_info_panel();
	create_transform_panel();

	if (selected_gameobject->has_component<SpriteComponent>())
		create_sprite_component_panel();
	if (selected_gameobject->has_component<BoxColliderComponent>())
		create_boxcollider_component_panel();
	if (selected_gameobject->has_component<AnimatedSpriteComponent>())
		create_animatedsprite_component_panel();
	if (selected_gameobject->has_component<AnimationComponent>())
		create_animation_component_panel();
	if (selected_gameobject->has_component<ChildAnimationComponent>())
		create_childanimation_component_panel();
}

float SceneEditor::get_next_component_y()
{
	float y = 0;
	for (auto& c : editor_panels)
		y += c.second->get_height();
	
	return y;
}

float SceneEditor::get_next_gameobject_y()
{
	float y = 0;
	for (auto& g : gameobjects_in_scene)
		for(auto& b : g.second)
			y += b.first->get_height();
	return y;
}

void SceneEditor::update_editor()
{
	if (!selected_gameobject)
		return;

	update_info_panel();
	update_transform_panel();

	update_sprite_component_panel();
	update_boxcollider_component_panel();
	update_animation_component_panel();
	update_childanimation_component_panel();
	update_animatedsprite_component_panel();
}

void SceneEditor::update_selector()
{
	if (addable_gameobjects.at("empty_gameobject")->is_pressed())
	{
		create_gameobject();
	}


	if (!selected_gameobject)
		return;

	if (addable_components.at(typeid(SpriteComponent).name())->is_pressed())
	{
		std::cout << "sprite clicked";
		selected_gameobject->add_component<SpriteComponent>();
		create_sprite_component_panel();
	}
	if (addable_components.at(typeid(BoxColliderComponent).name())->is_pressed())
	{
		selected_gameobject->add_component<BoxColliderComponent>();
		create_boxcollider_component_panel();
	}
	if (addable_components.at(typeid(AnimationComponent).name())->is_pressed())
	{
		selected_gameobject->add_component<AnimationComponent>();
		create_animation_component_panel();
	}
	if (addable_components.at(typeid(ChildAnimationComponent).name())->is_pressed())
	{
		if (!selected_gameobject->get_parent())
			return;
		selected_gameobject->add_component<ChildAnimationComponent>();
		create_childanimation_component_panel();
	}
	if (addable_components.at(typeid(AnimatedSpriteComponent).name())->is_pressed())
	{
		selected_gameobject->add_component<AnimatedSpriteComponent>();
		create_animatedsprite_component_panel();
	}

}

void SceneEditor::update_heirarchy()
{
	if (gameobjects_in_scene.size() == 0)
		return;
	if (Input::get_mouse_down(Input::Mouse::LEFT))
	{
		for (auto& gis : gameobjects_in_scene)
		{
			if (gis.first->mouse_in_bounds())
			{
				gameobject_held = true;
				selected_button = gis.first;
			}
		}
	}
	if (Input::get_mouse_up(Input::Mouse::LEFT) && gameobject_held)
	{
		for (auto& gis : gameobjects_in_scene)
		{
			if (gis.first->mouse_in_bounds() &&
				gis.second.at(0).second != selected_gameobject)
			{
				if (selected_gameobject->get_parent())
				{
					//object already has parent, remove elements from said parent
					
					std::vector<std::pair<Button*, GameObject*>> objects_to_remove = gameobjects_in_scene.at(selected_button);

					for (auto& gis2 : gameobjects_in_scene)
					{
						if (gis2.second.at(0).second == selected_gameobject->get_parent())
						{
							gis2.second.at(0).second->remove_child(selected_gameobject);
							for (auto& otr : objects_to_remove)
							{
								std::vector<std::pair<Button*, GameObject*>>::iterator position =
									std::find(gis2.second.begin(), gis2.second.end(), otr);
								if (position != gis2.second.end())
									gis2.second.erase(position);
								else
									std::cout << "ERROR::FAILURE TO FIND ELEMENT IN MAP TO REMOVE\n";
							}
						}
					}
				}

				selected_gameobject->set_parent(gis.second.at(0).second);
				gis.second.at(0).second->add_child(selected_gameobject);
				

				std::vector<std::pair<Button*, GameObject*>> objects_to_move = gameobjects_in_scene.at(selected_button);
				//add elements of object to new parent
				for (auto& otm : objects_to_move)
				{
					gis.second.push_back(otm);
				}

				update_heirarchy_positions();
				
				return;
			}	
		}
		// we are putting into empty slot?
		if (gameobject_heirarchy->mouse_in_bounds())
		{
			// move data back to its own position
			if (selected_gameobject->get_parent())
			{
				//object already has parent, remove elements from said parent
				std::vector<std::pair<Button*, GameObject*>> objects_to_remove = gameobjects_in_scene.at(selected_button);

				for (auto& gis2 : gameobjects_in_scene)
				{
					if (gis2.second.at(0).second == selected_gameobject->get_parent())
					{
						gis2.second.at(0).second->remove_child(selected_gameobject);
						for (auto& otr : objects_to_remove)
						{
							std::vector<std::pair<Button*, GameObject*>>::iterator position =
								std::find(gis2.second.begin(), gis2.second.end(), otr);
							if (position != gis2.second.end())
								gis2.second.erase(position);
							else
								std::cout << "ERROR::FAILURE TO FIND ELEMENT IN MAP TO REMOVE\n";
						}
					}
				}
			}

			selected_gameobject->set_parent(nullptr);

			update_heirarchy_positions();

			return;
		}

		gameobject_held = false;
		selected_button = nullptr;
	}


	for (auto& gis : gameobjects_in_scene)
	{
		if (gis.second.at(0).second->get_parent())
			continue;
		for (auto& b : gis.second)
		{
			if (b.first->is_pressed())
			{
				clear_editor();
				selected_gameobject = b.second;
				populate_editor();
			}
			b.first->set_text(b.second->get_info().name);
		}
	}
}

void SceneEditor::update_heirarchy_positions()
{
	float y = 0;
	float x = 0;
	for (auto& gis : gameobjects_in_scene)
	{
		if (gis.second[0].second->get_parent())
			continue;
		for (auto& b : gis.second)
		{
			GameObject* g = b.second->get_parent();
			while (g)
			{
				x += 10;
				g = g->get_parent();
			}

			b.first->set_position(
				gameobject_heirarchy->get_position().x + x,
				gameobject_heirarchy->get_position().y + y);
			x += 5;
			y += b.first->get_height();
		}
		x = 0;
	}
}

void SceneEditor::create_empty_gameobject_editor()
{
	ScrollView* sv = new ScrollView(0, 0, 420, 5000, false);
	gameobject_editor->add_scroll_view("scroll", sv);

	create_info_panel();
	create_transform_panel();

	editor_buttons["SAVE_SCENE"] = new Button(2510, 50, 100, 50, &font, "Save", 30,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	editor_buttons["DELETE_OBJECT"] = new Button(230, 840, 100, 50, &font, "Delete", 30,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

}
void SceneEditor::create_gameobject_selector()
{
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

	gameobject_selector->get_panel("component_panel")->add_element(typeid(SpriteComponent).name(), 
		addable_components.at(typeid(SpriteComponent).name()));

	addable_components[typeid(BoxColliderComponent).name()] = new Button(50, 50, 150, 25, &font, "BoxColliderComponent", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	gameobject_selector->get_panel("component_panel")->add_element(typeid(BoxColliderComponent).name(), 
		addable_components.at(typeid(BoxColliderComponent).name()));
	
	addable_components[typeid(AnimationComponent).name()] = new Button(50, 100, 150, 25, &font, "AnimationComponent", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	gameobject_selector->get_panel("component_panel")->add_element(typeid(AnimationComponent).name(),
		addable_components.at(typeid(AnimationComponent).name()));

	addable_components[typeid(ChildAnimationComponent).name()] = new Button(50, 150, 150, 25, &font, "ChildAnimationComponent", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	gameobject_selector->get_panel("component_panel")->add_element(typeid(ChildAnimationComponent).name(), 
		addable_components.at(typeid(ChildAnimationComponent).name()));

	addable_components[typeid(AnimatedSpriteComponent).name()] = new Button(50, 200, 150, 25, &font, "AnimatedSpriteComponent", 20,
		sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(250, 250, 250, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50));

	gameobject_selector->get_panel("component_panel")->add_element(typeid(AnimatedSpriteComponent).name(),
		addable_components.at(typeid(AnimatedSpriteComponent).name()));

}

void SceneEditor::create_gameobject_heirarchy()
{
	gameobject_heirarchy->add_element("heirarchy_panel", new Panel(0, 200, 200, 400));
	ScrollView* sv = new ScrollView(0, 0, 200, 500, false);
	gameobject_heirarchy->get_panel("heirarchy_panel")->add_scroll_view("scroll", sv);

}

void SceneEditor::save_scene()
{
	scene->set_name("Unnamed.json");
	if(scene_name->get_text() != "")
		scene->set_name(scene_name->get_text() + ".json");
	SceneManager::save_scene_prefab(scene);
}

std::vector<std::pair<Button*, GameObject*>> SceneEditor::insert_sort(std::vector<std::pair<Button*, GameObject*>> vec)
{
	std::vector<std::pair<Button*, GameObject*>> objects = vec;

	for (int k = 1; k < objects.size(); k++)
	{
		std::pair<Button*, GameObject*> temp = objects[k];
		int j = k - 1;
		while (j >= 0 && temp.second->get_parent() != objects[j].second)
		{
			objects[j + 1] = objects[j];
			j = j - 1;
		}
		objects[j + 1] = temp;
	}
	return objects;

}

}