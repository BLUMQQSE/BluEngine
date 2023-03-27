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
		current_interactor->get_game_object()->set_world_position(destination);
		if (destination_scene_name != SceneManager::Instance()->get_active_scene_name())
			EventSystem::Instance()->push_event(EventID::SCRIPTS_LOAD_SCENE, &destination_scene_name);
	}
}

#pragma region IData

Json::Value SceneChange::serialize_json()
{
	Json::Value obj;
	obj[typeid(IInteractable).name()] = IInteractable::serialize_json();

	obj["destination"] = destination.serialize_json();
	obj["destination-scene"] = destination_scene_name;

	return obj;
}

void SceneChange::unserialize_json(Json::Value obj)
{
	IInteractable::unserialize_json(obj[typeid(IInteractable).name()]);
	destination.unserialize_json(obj["destination"]);
	destination_scene_name = obj["destination-scene"].asString();
}

#pragma endregion

}