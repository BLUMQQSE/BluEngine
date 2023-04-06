#pragma once
#include "globals.h"
#include "EventSystem.h"
namespace bm98
{
class GameState;
class GameObject;
class Scene;
}

namespace bm98::core
{
class SceneManager : public Listener
{
public:
	// Returns instance of the SceneManager
	static SceneManager* Instance()
	{
		static SceneManager instance;
		return &instance;
	}

	std::string get_default_scene();
	std::string get_active_scene_name();

	void load_scene(std::string scene_name);
	/// <summary>
	/// Loads a scene prefab. Should only be used by editor state.
	/// </summary>
	/// <param name="scene_name"></param>
	void load_scene_prefab(std::string scene_name);
	/// <summary>
	/// Saves scene to json file.
	/// </summary>
	/// <param name="save_everything">If true, will save DontDestroyOnLoad objects and delete all objects in scene.
	///  This is defaulted to false, and should only be true on exiting gamestate.</param>
	void save_scene(bool save_everything = false);
	void save_scene(Scene* scene);
	/// <summary>
	/// Saves a scene prefab. Should only be used by editor state.
	/// </summary>
	void save_scene_prefab();

	/// <summary>
	/// Instantiates game objects into the active scene.
	/// </summary>
	void instantiate_gameobject(GameObject* game_object);
	void instantiate_gameobject_on_load(GameObject* game_object);
	void destroy_gameobject(GameObject* game_object);

	std::vector<GameObject*> get_objects_in_scene();

	GameObject* find(std::string name, GameObject* object_to_ignore);
	GameObject* find_with_tag(Tags::Tag tag, GameObject* object_to_ignore);
	std::vector<GameObject*> find_all_with_tag(Tags::Tag tag, GameObject* object_to_ignore);
	template <typename T> static  GameObject* find_of_type(GameObject* object_to_ignore);
	template <typename T> static std::vector<GameObject*> find_all_of_type(GameObject* object_to_ignore);
	//static void set_active_scene(std::string scene_name);

private:
	SceneManager();
	~SceneManager() { }
	SceneManager(const SceneManager& rhs)
	{

	}
	SceneManager& operator=(const SceneManager& rhs) {}



	std::string scenes_file_path = "Data/Scenes/";

	void destroy();
	/// <summary>
	/// Clears everything out of the active scene. Used when exiting gamestate to 
	/// ensure all memory gets removed.
	/// </summary>
	void clear_active_scene();

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	void init(Scene* active_scene);

	Scene* active_scene;

};


}