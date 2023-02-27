#include "pch.h"
#include "SceneChange.h"
#include "Collisions.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "DontDestroyOnLoad.h"
namespace bm98
{
SceneChange::SceneChange()
{
}
SceneChange::~SceneChange()
{
}
void SceneChange::on_trigger_enter(Collider info)
{


	std::cout << info.game_object->get_info().name << " entered my trigger\n";
	
	/*
	if (info.game_object->get_info().tag == Tag::PLAYER)
	{
		info.game_object->set_position(destination.x, destination.y);
		std::cout << "destination : " << destination_scene_name<<"\n";
		if (destination_scene_name != SceneManager::get_active_scene_name())
			SceneManager::load_scene(destination_scene_name);
	}
	*/
}

void SceneChange::on_trigger_stay(Collider info)
{
	std::cout << info.game_object->get_info().name << " is in my trigger\n";
}

void SceneChange::on_trigger_exit(Collider info)
{
	std::cout << info.game_object->get_info().name << " exited my trigger\n";
}

Json::Value SceneChange::serialize_json()
{
	Json::Value obj;

	obj["destination"] = destination.serialize_json();
	obj["destination-scene"] = destination_scene_name;

	return obj;
}

void SceneChange::unserialize_json(Json::Value obj)
{
	destination.unserialize_json(obj["destination"]);
	destination_scene_name = obj["destination-scene"].asString();
}

}