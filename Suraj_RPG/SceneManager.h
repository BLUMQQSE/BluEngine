#pragma once

namespace bm98
{
class GameObject;
class Scene;
}

namespace bm98::core
{
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	static void init(Scene* active_scene);

	static Scene* active_scene;

	static void update();

	static void load_scene(std::string scene_name);
	static void save_scene();

	/// <summary>
	/// Instantiates game objects into the active scene.
	/// </summary>
	/// <param name="on_next_update">If true, delays adding the game object until the next scene. This should be set when
	/// instantiating game objects while in a game object constructor to avoid memory access issues.</param>
	static void instantiate_gameobject(GameObject* game_object, bool on_next_update = false);
	static void destroy_gameobject(GameObject* game_object);


private:

	static std::string scenes_file_path;

	static std::vector<GameObject*> objects_to_add;

};

}