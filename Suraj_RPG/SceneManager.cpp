#include "pch.h"
#include "SceneManager.h"
#include "json/json.h"
#include "Scene.h"
#include "FileManager.h"
#include "GameObject.h"

namespace bm98::core
{

std::string SceneManager::scenes_file_path = "Data/Scenes/";
std::vector<GameObject*> SceneManager::objects_to_add;
Scene* SceneManager::active_scene = nullptr;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::init(Scene* scene)
{
	active_scene = scene;
}

/// <summary>
/// TODO: This may need safety checks later to not load objects into next scene
/// </summary>
void SceneManager::update()
{
	for (auto& obj : objects_to_add)
		active_scene->insert_gameobject(obj);

	objects_to_add.clear();

}

void SceneManager::load_scene(std::string scene_name)
{
	if (active_scene)
		save_scene();
	Json::Value obj = FileManager::load_from_file(scenes_file_path + active_scene->get_name(), true);
	//active_scene->unserialize_json(obj);
}

void SceneManager::save_scene()
{
	FileManager::save_to_file_styled(active_scene->serialize_json(), scenes_file_path + active_scene->get_name());
}

void SceneManager::save_scene(Scene* scene)
{
	FileManager::save_to_file_styled(scene->serialize_json(), scenes_file_path + scene->get_name());
}

void SceneManager::instantiate_gameobject(GameObject* game_object, bool on_next_update)
{
	if (on_next_update)
	{
		game_object->update();
		game_object->late_update();
		objects_to_add.push_back(game_object);
	}
	else
		active_scene->insert_gameobject(game_object);
}

void SceneManager::destroy_gameobject(GameObject* game_object)
{
	if (game_object == nullptr)
		return;
	active_scene->remove_gameobject(game_object);
}

}