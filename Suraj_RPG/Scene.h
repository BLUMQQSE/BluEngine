#pragma once
#include "IData.h"
namespace bm98
{
class GameObject;
class TilemapComponent;

class Scene : IData
{
public:
	Scene();
	Scene(std::string name);
	virtual ~Scene();

	void update();
	void late_update();
	void fixed_update();
	void render(sf::View* view = nullptr);

	std::string get_name();
	std::vector<GameObject*> get_objects();
	std::vector<GameObject*> get_dont_destroy_objects();

	void set_name(std::string name);

	void insert_gameobject(GameObject* go, bool initialize = true);
	void remove_gameobject(GameObject* go);

	void clear_scene(bool remove_everything = false);


	// Inherited via IData
	virtual Json::Value serialize_json() override;
	Json::Value serialize_destroyed_objects();
	virtual void unserialize_json(Json::Value obj) override;


private:
	std::string name;

	std::vector<GameObject*> objects_in_scene;


	void init_tilemap();
	void insert_sort();


};
}