#pragma once
#include "IData.h"
#include "core/EventSystem.h"
namespace bm98
{
class GameObject;
class TilemapComponent;

class Scene : IData, public core::Listener
{

public:
	Scene();
	Scene(std::string name);
	virtual ~Scene();

	void init();
	void scene_change();

	void update();
	void late_update();
	void fixed_update();
	void render(sf::View * view = nullptr);

	sf::View* get_view() { return scene_view; }
	void set_view(sf::View* view) { this->scene_view = view; }

	std::string get_file_name() { return file_name; }
	std::string get_scene_name() { return scene_name; }

	std::vector<std::weak_ptr<GameObject>> get_objects() 
	{
		std::vector<std::weak_ptr<GameObject>> result(objects_in_scene.size());
		for (int i = 0; i < objects_in_scene.size(); i++)
			result[i] = objects_in_scene[i];
		return result; 
	}
	std::vector<std::weak_ptr<GameObject>> get_dont_destroy_objects();

	void set_file_name(std::string name) { this->file_name = name; }
	void set_scene_name(std::string name) { this->scene_name = name; }

	void insert_gameobject(std::shared_ptr<GameObject> go, bool initialize = true);
	void remove_gameobject(std::shared_ptr<GameObject> go);

	void clear_scene(bool remove_everything = false);


	// Inherited via IData
	virtual Json::Value serialize_json() override;
	/// <summary>
	/// Serializes gameobjects not containing DontDestroyOnLoad.
	/// </summary>
	Json::Value serialize_destroyed_objects();
	/// <summary>
	/// Serializes gameobject containing DontDestroyOnLoad.
	/// </summary>
	/// <returns></returns>
	Json::Value serialize_undestroyed_objects();
	virtual void unserialize_json(Json::Value obj) override;

	// Inherited via Listener
	virtual void handle_event(core::Event* event) override;

private:
	std::string file_name;
	std::string scene_name;

	std::vector<std::shared_ptr<GameObject>> objects_in_scene;
	std::vector<std::shared_ptr<GameObject>> objects_to_add;
	std::vector<std::shared_ptr<GameObject>> objects_to_remove;

	sf::View* scene_view;
	void insert_sort();



};
}