#include "pch.h"
#include "Scene.h"
#include "Physics.h"
#include "GameObject.h"
#include "TilemapComponent.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "DontDestroyOnLoad.h"

namespace bm98
{
Scene::Scene()
{
}
Scene::Scene(std::string name)
	:name(name)
{
	//init_tilemap();
}

Scene::~Scene()
{
	int size = objects_in_scene.size();
	for (auto& o : objects_in_scene)
		delete o;
}

void Scene::update()
{

	//remove destroyed objects
	for (auto& o_t_r : objects_to_remove)
	{
		Physics::remove_from_physics(o_t_r);
		objects_in_scene.erase(std::find(objects_in_scene.begin(),
			objects_in_scene.end(), o_t_r));
		if (o_t_r->get_parent())
			o_t_r->get_parent()->remove_child(o_t_r);

		// destroy all posterity of object
		if (o_t_r->get_children().size() > 0)
		{
			std::vector<GameObject*> posterity = o_t_r->get_all_posterity();
			while (posterity.size() > 0)
			{
				Physics::remove_from_physics(posterity[0]);
				objects_in_scene.erase(std::find(objects_in_scene.begin(),
					objects_in_scene.end(), posterity[0]));
				delete posterity[0];
				posterity.erase(posterity.begin());
			}
		}
		delete o_t_r;
	}
	objects_to_remove.clear();

	// add new objects
	for (auto& o_t_a : objects_to_add)
		objects_in_scene.push_back(o_t_a);
	
	if (objects_to_add.size() > 0)
		insert_sort();
	objects_to_add.clear();

	// update objects
	for(std::size_t i = 0; i != objects_in_scene.size(); i++)
		objects_in_scene[i]->update();
	
}

void Scene::late_update()
{
	for (std::size_t i = 0; i != objects_in_scene.size(); i++)
		objects_in_scene[i]->late_update();
}

void Scene::fixed_update()
{
	for (std::size_t i = 0; i != objects_in_scene.size(); i++)
		objects_in_scene[i]->fixed_update();
}

void Scene::render(sf::View* view)
{
	for (std::size_t i = 0; i != objects_in_scene.size(); i++)
		objects_in_scene[i]->add_to_buffer(view);
}

std::string Scene::get_name()
{
	return name;
}

std::vector<GameObject*> Scene::get_objects()
{
	return objects_in_scene;
}

std::vector<GameObject*> Scene::get_dont_destroy_objects()
{
	std::vector<GameObject*> objs;
	for (auto& o : objects_in_scene)
		if (o->has_component<DontDestroyOnLoad>())
		{
			objs.push_back(o);
			for (auto& c : o->get_children())
				objs.push_back(c);
		}
	return objs;
}

void Scene::set_name(std::string name)
{
	std::cout << "setting scene name to: " << name << "\n";
	this->name = name;
}

void Scene::insert_gameobject(GameObject* go, bool initialize)
{
	Physics::add_to_physics(go);
	if (initialize)
	{
		objects_to_add.push_back(go);
		
		go->init();
		go->awake();
		go->start();
	}
	else
	{
		objects_in_scene.push_back(go);
		
		insert_sort();
	}
}

void Scene::remove_gameobject(GameObject* go)
{
	objects_to_remove.push_back(go);
}

void Scene::clear_scene(bool remove_everything)
{

	if (remove_everything)
	{
		for (auto& o : objects_in_scene)
			delete o;
		objects_in_scene.clear();
		return;
	}

	std::vector<GameObject*> objects;
	for (auto& o : objects_in_scene)
	{	
		//only need to get parents, there children will be removed later in update
		if (o->get_parent())
			continue;

		if (!o->has_component<DontDestroyOnLoad>())
		{
			objects.push_back(o);
		}
	}

	for (std::size_t i = 0; i != objects.size(); i++)
		remove_gameobject(objects[i]);
	

}

#pragma region IDATA

Json::Value Scene::serialize_json()
{
	Json::Value obj;

	obj["name"] = name;
	for (auto& o : objects_in_scene)
	{
		//only save parent, parent will save children
		if(!o->get_parent())
			obj["gameobjects"].append(o->serialize_json());
	}

	return obj;
}

Json::Value Scene::serialize_destroyed_objects()
{
	Json::Value obj;
	
	obj["name"] = name;
	for (auto& o : objects_in_scene)
	{
		//only save parent, parent will save children
		if (!o->get_parent() && !o->has_component<DontDestroyOnLoad>())
			obj["gameobjects"].append(o->serialize_json());
	}

	return obj;

}

void Scene::unserialize_json(Json::Value obj)
{
	name = obj["name"].asString();

	for (Json::Value game_object : obj["gameobjects"])
	{
		GameObject* go = new GameObject();
		go->unserialize_json(game_object);

		insert_gameobject(go, false);
	}

	for (auto& go : objects_in_scene)
		if(!go->is_initialized())
			go->init();
	for (auto& go : objects_in_scene)
		if (!go->is_initialized())
			go->awake();
	for (auto& go : objects_in_scene)
		if (!go->is_initialized())
			go->start();

}

#pragma endregion

void Scene::init_tilemap()
{
	GameObject* go = new GameObject();
	go->add_component<TilemapComponent>();
	go->get_component<TilemapComponent>().load_from_json("Data/Tilemaps/test.json");
	Physics::add_tiles_to_physics(go->get_component<TilemapComponent>().get_collidable_tiles());
	objects_in_scene.push_back(go);

	//Tilemap* map = new Tilemap();
	//map->load_from_json("Data/Tilemaps/test.json");
	//Physics::add_tiles_to_physics(map->get_collidable_tiles());
	//tilemaps_in_scene.push_back(map);
}

//actually works
void Scene::insert_sort()
{

	for (int k = 1; k < objects_in_scene.size(); k++)
	{
		GameObject* temp = objects_in_scene[k];
		int j = k - 1;
		while (j >= 0 && temp->get_parent() != objects_in_scene[j])
		{
			objects_in_scene[j + 1] = objects_in_scene[j];
			j = j - 1;
		}
		objects_in_scene[j + 1] = temp;
	}

}
}