#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FileManager.h"
#include "GameObject.h"

namespace bm98::core
{
std::string SceneManager::save_name = "GameState_Should_Change_This";
std::string SceneManager::scenes_file_path = "Data/Scenes/";
std::vector<GameObject*> SceneManager::objects_to_add;
Scene* SceneManager::active_scene = nullptr;
//GameState* SceneManager::game_state = nullptr;

std::string SceneManager::get_default_scene()
{
	return "default.json";
}

std::string SceneManager::get_save_name()
{
	return save_name;
}

std::string SceneManager::get_active_scene_name()
{
	if (active_scene)
		return active_scene->get_name();

	return get_default_scene();
}

void SceneManager::init(std::string s_n, Scene* scene)
{
	//game_state = g_s;
	active_scene = scene;
	save_name = s_n;
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
	// changing scene, save prior
	if (active_scene->get_name() != scene_name)
		save_scene();
	std::vector<GameObject*> dont_destroy_objects = active_scene->get_dont_destroy_objects();
	active_scene->clear_scene();

	Json::Value obj = FileManager::load_from_file(save_name + scenes_file_path + scene_name);
	
	active_scene->set_name(scene_name);
	for (auto& dd : dont_destroy_objects)
		active_scene->insert_gameobject(dd, false);

	active_scene->unserialize_json(obj);
}

void SceneManager::load_scene_prefab(std::string scene_name)
{
	//if (active_scene)
	//	save_scene();
	
	active_scene->clear_scene();
	active_scene->set_name(scene_name);

	Json::Value obj = FileManager::load_from_file(scenes_file_path + scene_name);
	active_scene->unserialize_json(obj);
}

void SceneManager::save_scene()
{
	FileManager::save_to_file_styled(active_scene->serialize_json(), save_name + scenes_file_path + active_scene->get_name());
}

void SceneManager::save_scene(Scene* scene)
{
	FileManager::save_to_file_styled(scene->serialize_json(), save_name + scenes_file_path + scene->get_name());
}

void SceneManager::save_scene_prefab(Scene* scene)
{
	FileManager::save_to_file_styled(scene->serialize_json(), scenes_file_path + scene->get_name());

}

void SceneManager::instantiate_gameobject(GameObject* game_object, bool on_next_update)
{
	if (on_next_update)
	{
		objects_to_add.push_back(game_object);
	}
	else
		active_scene->insert_gameobject(game_object);
}

void SceneManager::instantiate_gameobject_on_load(GameObject* game_object)
{
	active_scene->insert_gameobject(game_object, false);
}

void SceneManager::destroy_gameobject(GameObject* game_object)
{
	if (game_object == nullptr)
		return;
	active_scene->remove_gameobject(game_object);
}

GameObject* SceneManager::find(std::string name, GameObject* object_to_ignore)
{
	std::vector<GameObject*> objects = active_scene->get_objects();
	for (auto& o : objects)
		if (o->get_info().name == name)
			return o;

	return nullptr;
}

GameObject* SceneManager::find_with_tag(Tag tag, GameObject* object_to_ignore)
{
	std::vector<GameObject*> objects = active_scene->get_objects();
	for (auto& o : objects)
		if (o->get_info().tag == tag)
			return o;

	return nullptr;
}

std::vector<GameObject*> SceneManager::find_all_with_tag(Tag tag, GameObject* object_to_ignore)
{
	std::vector<GameObject*> objects;
	for (auto& o : active_scene->get_objects())
		if (o->get_info().tag == tag)
			objects.push_back(o);

	return objects;
}
template <typename T> static  GameObject* SceneManager::find_of_type(GameObject* object_to_ignore)
{
	std::vector<GameObject*> objects = active_scene->get_objects();
	for (auto& o : objects)
		if (o->has_component<T>())
			return o;
}
template <typename T> static std::vector<GameObject*> SceneManager::find_all_of_type(GameObject* object_to_ignore)
{
	std::vector<GameObject*> objects = active_scene->get_objects();
	objects.erase(
		std::remove_if(
			objects.begin(),
			objects.end(),
			[](GameObject* const& p) { return !p->has_component<T>(); }
		),
		objects.end()
	);
	return objects;
}
}