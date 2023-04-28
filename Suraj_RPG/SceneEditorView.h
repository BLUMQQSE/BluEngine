#pragma once
#include "Component.h"
#include "Gui.h"
#include "TextureSelector.h"
#include "core/EventSystem.h"
#include "Timer.h"

namespace bm98
{
class GameObject;
class Component;

class SceneEditorView : public core::Listener
{
public:

	enum class EditorPanel
	{
		ALL,
		HEIRARCHY,
		CONTEXT,
		INSPECTOR
	};

	SceneEditorView();
	virtual ~SceneEditorView();

	void init();

	const bool in_bound() const;

	void toggle_editor(EditorPanel panel_to_toggle);
	void update();
	virtual void update_sfml(sf::Event sfEvent);
	virtual void add_to_buffer(sf::View* view);

	// Inherited via Listener
	virtual void handle_event(core::Event* event) override;

	GameObject* get_selected_gameobject() { return selected_gameobject; }

private:
	GUI::Panel* heir_panel;
	GUI::Panel* inspec_panel;
	GUI::Panel* scene_editor_panel;

	GUI::InputBox* file_name;
	GUI::InputBox* component_name;

	GUI::Button* load_file;
	GUI::Button* save_file;
	GUI::Button* add_gameobject;
	GUI::Button* remove_gameobject;
	GUI::Button* add_component;
	GUI::Button* remove_component;

	sf::View* heir_view;
	sf::View* inspec_view;

	bool heir_active = true;
	bool inspec_active = true;
	bool scene_editor_active = true;

	bool gameobject_held = false;

	std::unordered_map<GUI::Button*, GameObject*> objects_in_scene_map;

	/// <summary> Map connecting each Component Panel to its variables stored. </summary>
	//std::unordered_map < GUI::Panel*, std::vector<Editor::SerializedVar>> variables_in_components;
	std::vector<std::pair<GUI::Panel*,
		std::vector<Editor::SerializedVar>>> variables_in_components;

	GameObject* selected_gameobject;
	GUI::TransformMover* transform_mover;

	//GUI::TextureSelector* texture_selector;
	std::unique_ptr<GUI::TextureSelector> texture_selector;

	void set_selected_gameobject(GameObject* object);

	void create_heir_panel();
	void update_heir_panel();

	void clear_inspec_panel();
	void create_inspec_panel();
	void update_inspec_panel();

	void create_scene_editor_panel();
	void update_scene_editor_panel();


	GUI::Panel* create_component_panel(float pos_y, float width, std::string component_name,
		std::vector<Editor::SerializedVar> vars);

	void update_component_panel(std::pair<GUI::Panel*, std::vector<Editor::SerializedVar>> vars);

};

}