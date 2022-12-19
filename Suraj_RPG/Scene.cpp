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
	SceneManager::save_scene();
	for (auto& o : objects_in_scene)
		delete o;
}

void Scene::update()
{
	std::cout << objects_in_scene.size() << "\n";
	for (auto& o : objects_in_scene)
		o->update();
	
}

void Scene::late_update()
{
	for (auto& o : objects_in_scene)
		o->late_update();
}

void Scene::fixed_update()
{

	for (auto& o : objects_in_scene)
		o->fixed_update();
}

void Scene::render(sf::View* view)
{
	for (auto& o : objects_in_scene)
		o->add_to_buffer(view);
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
	this->name = name;
}

void Scene::insert_gameobject(GameObject* go, bool initialize)
{
	objects_in_scene.push_back(go);
	Physics::add_to_physics(go);
	insert_sort();

	if (initialize)
	{
		go->init();
		go->awake();
		go->start();
	}
}

void Scene::remove_gameobject(GameObject* go)
{
	Physics::remove_from_physics(go);
	objects_in_scene.erase(std::remove(objects_in_scene.begin(), objects_in_scene.end(), go));
	delete go;
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
	std::vector<GameObject*> objects_to_remove;

	for (auto& o : objects_in_scene)
	{	
		bool dont_destroy = false;

		if (o->has_component<DontDestroyOnLoad>())
			dont_destroy = true;
		else
		{
			GameObject* obj = o;
			while (obj->get_parent())
			{
				obj = obj->get_parent();
				if (obj->has_component<DontDestroyOnLoad>())
				{
					dont_destroy = true;
					break;
				}
			}
		}

		if (!dont_destroy)
		{
			std::cout << "Removing: " << o->get_info().name << "\n";
			objects_to_remove.push_back(o);
			//remove_gameobject(o);
		}
		else
		{
			std::cout << "Not Removing: " << o->get_info().name << "\n";
		}
	}
	for (auto& otr : objects_to_remove)
		remove_gameobject(otr);
	

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