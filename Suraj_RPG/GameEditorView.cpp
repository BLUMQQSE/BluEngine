#include "pch.h"
#include "GameEditorView.h"
#include "SceneManager.h"
#include "GameObject.h"
namespace bm98
{
GameEditorView::GameEditorView()
{
	EventSystem::instance()->subscribe(EventID::SCENE_ADD_GAMEOBJECT, this);
	EventSystem::instance()->subscribe(EventID::SCENE_REMOVE_GAMEOBJECT, this);
}

GameEditorView::~GameEditorView()
{

}

void GameEditorView::toggle_editor(EditorPanel panel_to_toggle)
{
	switch (panel_to_toggle)
	{
	case bm98::GameEditorView::EditorPanel::ALL:
		all_active = !all_active;
		heir_active = all_active;
		context_active = all_active;
		inspector_active = all_active;
		break;
	case bm98::GameEditorView::EditorPanel::HEIRARCHY:
		heir_active = !heir_active;
		break;
	case bm98::GameEditorView::EditorPanel::CONTEXT:
		context_active = !context_active;
		break;
	case bm98::GameEditorView::EditorPanel::INSPECTOR:
		inspector_active = !inspector_active;
		break;
	}

	if (!heir_active || !context_active || !inspector_active)
		all_active = false;
	else
		all_active = true;

}

void GameEditorView::update()
{
	if(heir_active)
		heir_panel->update();
	if(context_active)
		context_panel->update();
	if(inspector_active)
		inspector_panel->update();


	/*
	ON EACH UPDATE:

		NEED TO CHECK GAMEOBJECTS IN SCENE ARE UNCHANGED
			LOOP THROUGH AND COMPARE POINTERS 
				IF CHANGE FOUND DELETE objects_in_scene AND RESET IT SET selected_gameobject TO NULL
	*/
	if (objects_in_scene.size() != core::SceneManager::get_objects_in_scene().size())
	{
		// need to update objects_in_scene
	}
	else
	{
		std::vector<GameObject*> get = core::SceneManager::get_objects_in_scene();
		for(int i = 0; i != objects_in_scene.size(); i++)
			if (objects_in_scene[i] != get[i])
			{
				// need to update objects_in_scene
			}
	}

	/*
		NEED TO CHECK COMPONENTS ON selected_gameobject
	*/
	if (selected_gameobject)
	{
		if (selected_gameobject->get_components().size() != components.size())
		{
			// need to update components
		}
		else
		{
			std::vector<Component*> get = selected_gameobject->get_components();
			for (int i = 0; i != components.size(); i++)
				if (components[i] != get[i])
				{
					// need to update components on selected object
				}
		}
	}

}

void GameEditorView::update_sfml(sf::Event sfEvent)
{
	if(heir_active)
		heir_panel->update_sfml(sfEvent);
	if(context_active)
		context_panel->update_sfml(sfEvent);
	if(inspector_active)
		inspector_panel->update_sfml(sfEvent);
}

void GameEditorView::handle_event(Event* event)
{
	EventID id = event->get_event_id();
	if (id == EventID::SCENE_ADD_GAMEOBJECT || id == EventID::SCENE_REMOVE_GAMEOBJECT)
	{
		
	}
}


void GameEditorView::create_heir_panel()
{
}

void GameEditorView::create_context_panel()
{
}

void GameEditorView::create_inspector_panel()
{
}

}