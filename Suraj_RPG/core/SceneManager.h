#pragma once
#include "../globals.h"
#include "EventSystem.h"
#include "../gui/Gui.h"
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
	sf::View* get_active_scene_view();

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
	void instantiate_gameobject(std::shared_ptr<GameObject> game_object);
	void instantiate_gameobject_on_load(std::shared_ptr<GameObject> game_object);
	void destroy_gameobject(std::shared_ptr<GameObject> game_object);

	std::vector<std::weak_ptr<GameObject>> get_objects_in_scene();

	std::weak_ptr<GameObject> find(std::string name, std::shared_ptr<GameObject> object_to_ignore);
	std::weak_ptr<GameObject> find_with_id(unsigned long id, std::shared_ptr<GameObject> object_to_ignore);
	std::weak_ptr<GameObject> find_with_tag(Tags::Tag tag, std::shared_ptr<GameObject> object_to_ignore);
	std::vector<std::weak_ptr<GameObject>> find_all_with_tag(Tags::Tag tag, std::shared_ptr<GameObject> object_to_ignore);
	template <typename T> static  std::weak_ptr<GameObject> find_of_type(std::shared_ptr<GameObject> object_to_ignore);
	template <typename T> static std::vector<std::weak_ptr<GameObject>> find_all_of_type(std::shared_ptr<GameObject> object_to_ignore);
	//static void set_active_scene(std::string scene_name);

private:
	SceneManager();
	~SceneManager() { }
	SceneManager(const SceneManager& rhs)
	{

	}
	SceneManager& operator=(const SceneManager& rhs) {}

	std::string scenes_file_path = "Data/Scenes/";
	Scene* active_scene;

	GUI::Label* label;

	void destroy();
	/// <summary>
	/// Clears everything out of the active scene. Used when exiting gamestate to 
	/// ensure all memory gets removed.
	/// </summary>
	void clear_active_scene();

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	void init(Scene* active_scene);

	bool load_finished = false;
	Json::Value loaded_scene_data;

	static void LoadSceneInBackground(std::string scene_name);

	//std::pair<Vector2f, float> name_tag_var = std::make_pair(Vector2f(0, -5), 1);
	float fade_speed = 1;

};


}