#pragma once
#include "Component.h"
#include "Gui.h"
#include "EventSystem.h"
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

	void toggle_editor(EditorPanel panel_to_toggle);
	void update();
	virtual void update_sfml(sf::Event sfEvent);

	// Inherited via Listener
	virtual void handle_event(core::Event* event) override;


private:
	GUI::Panel* heir_panel;
	GUI::Panel* inspec_panel;

	sf::View* heir_view;
	sf::View* inspec_view;

	bool heir_active = true;
	bool inspec_active = true;


	bool gameobject_held = false;

	std::unordered_map<GUI::Button*, GameObject*> objects_in_scene_map;

	/// <summary> Map connecting each Component Panel to its variables stored. </summary>
	//std::unordered_map < GUI::Panel*, std::vector<Editor::SerializedVar>> variables_in_components;
	std::vector<std::pair<GUI::Panel*,
		std::vector<Editor::SerializedVar>>> variables_in_components;

	GameObject* selected_gameobject;

	//std::unordered_map<GUI::Panel*, Component*> component_panels;

	void create_heir_panel();
	void update_heir_panel();

	void create_inspec_panel();
	void update_inspec_panel();


	GUI::Panel* create_component_panel(float pos_y, float width, std::string component_name,
		std::vector<Editor::SerializedVar> vars);

	void update_component_panel(std::pair<GUI::Panel*, std::vector<Editor::SerializedVar>> vars);

};

}