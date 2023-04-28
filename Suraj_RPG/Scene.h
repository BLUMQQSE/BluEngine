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
	void set_view(sf::View* view);

	std::string get_name();
	std::vector<GameObject*> get_objects();
	std::vector<GameObject*> get_dont_destroy_objects();

	void set_name(std::string name);

	void insert_gameobject(GameObject* go, bool initialize = true);
	void remove_gameobject(GameObject* go);

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
	std::string name;

	std::vector<GameObject*> objects_in_scene;
	std::vector<GameObject*> objects_to_add;
	std::vector<GameObject*> objects_to_remove;

	sf::View* scene_view;
	void insert_sort();



};
}