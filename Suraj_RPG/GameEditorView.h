#pragma once
#include "Gui.h"
namespace bm98
{
class GameObject;
class Component;
class GameEditorView
{
public:

	enum class EditorPanel
	{
		ALL,
		HEIRARCHY,
		CONTEXT,
		INSPECTOR
	};

	GameEditorView();
	virtual ~GameEditorView();

	void toggle_editor(EditorPanel panel_to_toggle);
	void update();
	virtual void update_sfml(sf::Event sfEvent);

private:
	GUI::Panel* heir_panel;
	GUI::Panel* context_panel;
	GUI::Panel* inspector_panel;

	sf::View* heir_view;
	sf::View* inspector_view;

	bool all_active = true;
	bool heir_active = true;
	bool context_active = true;
	bool inspector_active = true;
	//context will use default view

	std::unordered_map<GUI::Button*, GameObject*> objects_in_scene_map;
	std::vector<GameObject*> objects_in_scene;
	GameObject* selected_gameobject;
	// components on selected gameobject
	std::vector<GUI::Panel*> components_panels;
	std::vector<Component*> components;

	void create_heir_panel();
	void create_context_panel();
	void create_inspector_panel();

};

}
