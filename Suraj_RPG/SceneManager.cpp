#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FileManager.h"
#include "GameObject.h"

namespace bm98::core
{
std::string SceneManager::scenes_file_path = "Data/Scenes/";
std::vector<GameObject*> SceneManager::objects_to_add;
Scene* SceneManager::active_scene = nullptr;
//GameState* SceneManager::game_state = nullptr;

std::string SceneManager::get_default_scene()
{
	return "default.json";
}


std::string SceneManager::get_active_scene_name()
{
	if (active_scene)
	{
		return active_scene->get_name();
	}

	return get_default_scene();
}

void SceneManager::init(Scene* scene)
{
	//game_state = g_s;
	active_scene = scene;
}

void SceneManager::destroy()
{
	active_scene = nullptr;
}

void SceneManager::load_scene(std::string scene_name)
{
	std::cout << "load_scene(" << scene_name << ")\n";
	if (active_scene->get_name() != scene_name)
	{
		save_scene();
	}
	active_scene->clear_scene();


	Json::Value obj = FileManager::load_from_file(FileManager::get_save_name() + scenes_file_path + scene_name);

	active_scene->set_name(scene_name);

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

void SceneManager::save_scene(bool save_everything)
{
	std::cout << "save_scene("<<active_scene->get_name()<<")\n";

	FileManager::save_to_file_styled(active_scene->serialize_undestroyed_objects(), FileManager::get_save_name() + scenes_file_path + "dont_destroy_on_load_objects.json");
	FileManager::save_to_file_styled(active_scene->serialize_destroyed_objects(), FileManager::get_save_name() + scenes_file_path + active_scene->get_name());

	if (save_everything)
		active_scene->clear_scene(true);
}

void SceneManager::save_scene(Scene* scene)
{
	FileManager::save_to_file_styled(scene->serialize_json(), FileManager::get_save_name() + scenes_file_path + scene->get_name());
}

void SceneManager::clear_active_scene()
{
	active_scene->clear_scene(true);
}

void SceneManager::save_scene_prefab(Scene* scene)
{
	FileManager::save_to_file_styled(scene->serialize_json(), scenes_file_path + scene->get_name());

}

void SceneManager::instantiate_gameobject(GameObject* game_object)
{
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

std::vector<GameObject*> SceneManager::get_objects_in_scene()
{
	return active_scene->get_objects();
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