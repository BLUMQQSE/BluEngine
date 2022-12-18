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
	static std::string get_save_name();
	static std::string get_active_scene_name();

	static void init(std::string save_name, Scene* active_scene);

	static Scene* active_scene;

	static void update();

	static void load_scene(std::string scene_name);
	/// <summary>
	/// Loads a scene prefab. Should only be used by editor state.
	/// </summary>
	/// <param name="scene_name"></param>
	static void load_scene_prefab(std::string scene_name);
	static void save_scene();
	static void save_scene(Scene* scene);
	/// <summary>
	/// Saves a scene prefab. Should only be used by editor state.
	/// </summary>
	static void save_scene_prefab(Scene* scene);

	/// <summary>
	/// Instantiates game objects into the active scene.
	/// </summary>
	/// <param name="on_next_update">If true, delays adding the game object until the next scene. This should be set when
	/// instantiating game objects while in a game object constructor to avoid memory access issues.</param>
	static void instantiate_gameobject(GameObject* game_object, bool on_next_update = false);
	static void instantiate_gameobject_on_load(GameObject* game_object);
	static void destroy_gameobject(GameObject* game_object);

	static GameObject* find(std::string name, GameObject* object_to_ignore);
	static GameObject* find_with_tag(Tag tag, GameObject* object_to_ignore);
	static std::vector<GameObject*> find_all_with_tag(Tag tag, GameObject* object_to_ignore);
	template <typename T> static  GameObject* find_of_type(GameObject* object_to_ignore);
	template <typename T> static std::vector<GameObject*> find_all_of_type(GameObject* object_to_ignore);
	//static void set_active_scene(std::string scene_name);

private:
	static std::string save_name;
	static std::string scenes_file_path;

	//static GameState* game_state;
	static std::vector<GameObject*> objects_to_add;

};


}