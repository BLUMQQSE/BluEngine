#pragma once
#include "globals.h"
namespace bm98
{
class GameState;
class GameObject;
class Scene;
}

namespace bm98::core
{
class SceneManager
{
public:

	static std::string get_default_scene();
	static std::string get_active_scene_name();

	static void init(Scene* active_scene);
	static void destroy();

	static Scene* active_scene;

	static void load_scene(std::string scene_name);
	/// <summary>
	/// Loads a scene prefab. Should only be used by editor state.
	/// </summary>
	/// <param name="scene_name"></param>
	static void load_scene_prefab(std::string scene_name);
	/// <summary>
	/// Saves scene to json file.
	/// </summary>
	/// <param name="save_everything">If true, will save DontDestroyOnLoad objects and delete all objects in scene.
	///  This is defaulted to false, and should only be true on exiting gamestate.</param>
	static void save_scene(bool save_everything = false);
	static void save_scene(Scene* scene);
	/// <summary>
	/// Clears everything out of the active scene. Used when exiting gamestate to 
	/// ensure all memory gets removed.
	/// </summary>
	static void clear_active_scene();
	/// <summary>
	/// Saves a scene prefab. Should only be used by editor state.
	/// </summary>
	static void save_scene_prefab(Scene* scene);

	/// <summary>
	/// Instantiates game objects into the active scene.
	/// </summary>
	static void instantiate_gameobject(GameObject* game_object);
	static void instantiate_gameobject_on_load(GameObject* game_object);
	static void destroy_gameobject(GameObject* game_object);

	static GameObject* find(std::string name, GameObject* object_to_ignore);
	static GameObject* find_with_tag(Tag tag, GameObject* object_to_ignore);
	static std::vector<GameObject*> find_all_with_tag(Tag tag, GameObject* object_to_ignore);
	template <typename T> static  GameObject* find_of_type(GameObject* object_to_ignore);
	template <typename T> static std::vector<GameObject*> find_all_of_type(GameObject* object_to_ignore);
	//static void set_active_scene(std::string scene_name);

private:
	static std::string scenes_file_path;
	//static GameState* game_state;
	static std::vector<GameObject*> objects_to_add;

};


}