#include "pch.h"
#include "Scene.h"
#include "Physics.h"
#include "GameObject.h"
#include "TilemapComponent.h"
#include "SceneManager.h"
#include "FileManager.h"

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
	SceneManager::save_scene();
	for (auto& o : objects_in_scene)
		delete o;
}

void Scene::update()
{
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

void Scene::set_name(std::string name)
{
	this->name = name;
}

void Scene::insert_gameobject(GameObject* go)
{
	objects_in_scene.push_back(go);
	Physics::add_to_physics(go);
	insert_sort();

	go->init();
	go->awake();
	go->start();
}

void Scene::remove_gameobject(GameObject* go)
{
	objects_in_scene.erase(std::remove(objects_in_scene.begin(), objects_in_scene.end(), go));
	delete go;
}

void Scene::load_scene(Json::Value obj)
{
}

#pragma region IDATA

Json::Value Scene::serialize_json()
{
	Json::Value obj;

	obj["name"] = name;
	for (auto& o : objects_in_scene)
	{
		std::cout << "object\n";
		//only save parent, parent will save children
		if(!o->get_parent())
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
		objects_in_scene.push_back(go);
	}

	for (auto& go : objects_in_scene)
		go->init();
	for (auto& go : objects_in_scene)
		go->awake();
	for (auto& go : objects_in_scene)
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