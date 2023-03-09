#include "pch.h"
#include "Scene.h"
#include "Physics.h"
#include "GameObject.h"
#include "TilemapComponent.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "DontDestroyOnLoad.h"
#include "CameraComponent.h"
#include "SpriteComponent.h"
#include "PlayerController.h"
#include "EventSystem.h"
namespace bm98
{
Scene::Scene()
{
}
Scene::Scene(std::string name)
	:name(name)
{

}

Scene::~Scene()
{
	int size = objects_in_scene.size();
	for (auto& o : objects_in_scene)
	{
		o->on_destroy();
		delete o;
	}
}

void Scene::init()
{

	for (auto& o : objects_in_scene)
		if (o->has_component<CameraComponent>())
			scene_view = &o->get_component<CameraComponent>().get_camera_view();
	
}

void Scene::update()
{

	// add new objects (add before remove in case adding a child to which parent will be
	// destroyed in same update
	for (auto& o_t_a : objects_to_add)
	{
		objects_in_scene.push_back(o_t_a);
		EventSystem::instance()->push_event(EventID::SCENE_ADD_GAMEOBJECT, o_t_a);
	}
	if (objects_to_add.size() > 0)
		insert_sort();
	
	objects_to_add.clear();

	//remove destroyed objects
	for (auto& o_t_r : objects_to_remove)
	{
		o_t_r->on_destroy();
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
				posterity[0]->on_destroy();
				Physics::remove_from_physics(posterity[0]);
				objects_in_scene.erase(std::find(objects_in_scene.begin(),
					objects_in_scene.end(), posterity[0]));

				EventSystem::instance()->push_event(EventID::SCENE_REMOVE_GAMEOBJECT, posterity[0]);
				
				delete posterity[0];
				posterity.erase(posterity.begin());
			}
		}

		EventSystem::instance()->push_event(EventID::SCENE_REMOVE_GAMEOBJECT, o_t_r);
		
		delete o_t_r;
	}
	objects_to_remove.clear();


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
	{
		objects_in_scene[i]->add_to_buffer(scene_view);
	}
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
	
	if (initialize)
	{
		
		std::vector<GameObject*> objs;
		

		objs.push_back(go);
		
		
		std::vector<GameObject*> posterity = go->get_all_posterity();
		objs.insert(objs.end(), posterity.begin(), posterity.end());
		

		for (std::size_t i = 0; i < objs.size(); i++)
			Physics::add_to_physics(objs[i]);

		for (std::size_t i = 0; i < objs.size(); i++)
			objects_to_add.push_back(objs[i]);
		for (std::size_t i = 0; i < objs.size(); i++)
			objs[i]->init();
		for (std::size_t i = 0; i < objs.size(); i++)
			objs[i]->awake();
		for (std::size_t i = 0; i < objs.size(); i++)
			objs[i]->start();
	}
	else
	{
		// this occurs on loading objects from file, do not need posterity
		Physics::add_to_physics(go);
		objects_in_scene.push_back(go);
		
		insert_sort();
	}
}

void Scene::remove_gameobject(GameObject* go)
{
	if (std::find(objects_in_scene.begin(), objects_in_scene.end(), go) == objects_in_scene.end())
	{
		std::cout << "ERROR::SCENE::REMOVE_GAMEOBJECT::OBJECT DOES NOT EXIST IN SCENE\n";
		return;
	}
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

Json::Value Scene::serialize_undestroyed_objects()
{
	Json::Value obj;

	obj["name"] = "dont_destroy_on_load_objects.json";
	for (auto& o : objects_in_scene)
	{
		//only save parent, parent will save children
		if (!o->get_parent() && o->has_component<DontDestroyOnLoad>())
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