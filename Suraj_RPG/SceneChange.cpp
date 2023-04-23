#include "pch.h"
#include "SceneChange.h"
#include "Collisions.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "DontDestroyOnLoad.h"
#include "Interactor.h"
namespace bm98
{

void SceneChange::handle_instant_interaction()
{
	if (current_interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		if (destination.scene_name != SceneManager::Instance()->get_active_scene_name())
		{
			EventSystem::Instance()->push_event(EventID::SCRIPTS_LOAD_SCENE, &destination,
				Caller(Caller::Name::SCENE_CHANGE, (void*)this));
		
		}
		else
			current_interactor->get_game_object()->set_world_position(destination.position);
	}
}

#pragma region IData

Json::Value SceneChange::serialize_json()
{
	Json::Value obj;
	obj[typeid(IInteractable).name()] = IInteractable::serialize_json();

	obj["destination-position"] = destination.position.serialize_json();
	obj["destination-scene"] = destination.scene_name;

	return obj;
}

void SceneChange::unserialize_json(Json::Value obj)
{
	IInteractable::unserialize_json(obj[typeid(IInteractable).name()]);
	destination.position.unserialize_json(obj["destination-position"]);
	destination.scene_name = obj["destination-scene"].asString();
}

std::vector<Editor::SerializedVar> SceneChange::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;

	values = IInteractable::get_editor_values();

	values.push_back(Editor::SerializedVar("destination_scene", (void*)&destination.scene_name, Var::Type::String));
	values.push_back(Editor::SerializedVar("destination_position", (void*)&destination.position, Var::Type::Vector2f));

	return values;
}

#pragma endregion

}