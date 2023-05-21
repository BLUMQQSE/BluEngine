#include "pch.h"
#include "core/Debug.h"
#include "Scene.h"
#include "core/Physics.h"
#include "GameObject.h"
#include "core/SceneManager.h"
#include "core/FileManager.h"
#include "DontDestroyOnLoad.h"
#include "CameraComponent.h"
#include "core/EventSystem.h"
namespace bm98
{
Scene::Scene()
{
}
Scene::Scene(std::string file_name)
	:file_name(file_name)
{
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_PARENT_CHANGE, this);
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_INSTANTIATE, this);
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_DESTROY, this);

	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_COMPONENT_ADDED_FLAG, this);
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_COMPONENT_REMOVED_FLAG, this);
}

Scene::~Scene()
{
	int size = objects_in_scene.size();
	for (auto& o : objects_in_scene)
	{
		o.reset();
		//o->on_destroy();
		//delete o;
	}
}

void Scene::init()
{
	for (auto& o : objects_in_scene)
	{
		if (o->has_component<CameraComponent>())
		{
			scene_view = &o->get_component<CameraComponent>().lock()->get_camera_view();
		}
	}

	for (std::size_t i = 0; i != objects_in_scene.size(); i++)
	{
		objects_in_scene[i]->add_to_buffer(scene_view);
	}
}

void Scene::update()
{
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

std::vector<std::weak_ptr<GameObject>> Scene::get_dont_destroy_objects()
{
	std::vector<std::weak_ptr<GameObject>> objs;
	for (auto& o : objects_in_scene)
		if (o->has_component<DontDestroyOnLoad>())
		{
			objs.push_back(o);
			for (auto& c : o->get_children())
				objs.push_back(std::shared_ptr<GameObject>(c));
		}
	return objs;
}

void Scene::insert_gameobject(std::shared_ptr<GameObject> go, bool initialize)
{
	go->add_to_buffer(scene_view);
	if (initialize)
	{
		std::vector<std::shared_ptr<GameObject>> objs;
		

		objs.push_back(go);
		
		
		std::vector<std::weak_ptr<GameObject>> posterity = go->get_all_posterity();
		objs.insert(objs.end(), posterity.begin(), posterity.end());
		
		for (std::size_t i = 0; i < objs.size(); i++)
			objects_in_scene.push_back(objs[i]);
		for (std::size_t i = 0; i < objs.size(); i++)
			objs[i]->init();
		for (std::size_t i = 0; i < objs.size(); i++)
			objs[i]->awake();
		for (std::size_t i = 0; i < objs.size(); i++)
			objs[i]->start();

		insert_sort();
	}
	else
	{
		// this occurs on loading objects from file, do not need posterity
		//Physics::add_to_physics(go);
		objects_in_scene.push_back(go);
		
		insert_sort();
	}
}

void Scene::remove_gameobject(std::shared_ptr<GameObject> go)
{
	// ISSUE
	//std::vector<std::shared_ptr<GameObject>>::iterator obj = std::find(objects_in_scene.begin(), objects_in_scene.end(), go.get());
	std::vector<std::shared_ptr<GameObject>>::iterator obj = std::find_if(objects_in_scene.begin(), objects_in_scene.end(),
																		  [go](std::shared_ptr<GameObject> const& i) { return i.get() == go.get(); });
	if (obj == objects_in_scene.end())
	{ 
		Debug::Instance()->core_log("ATTEMPTED TO REMOVE GAMEOBJECT WHICH DOESN'T EXIST", Debug::LogLevel::WARNING);
		return;
	}

	go->on_destroy();
	Physics::Instance()->remove_from_physics(go);
	
	
	objects_in_scene.erase(obj);	
	go->set_parent(nullptr);

	// destroy all posterity of object
	if (go->get_children().size() > 0)
	{
		std::vector<std::weak_ptr<GameObject>> posterity = go->get_all_posterity();
		while (posterity.size() > 0)
		{
			posterity[0].lock()->on_destroy();
			Physics::Instance()->remove_from_physics(posterity[0].lock());
			
			// ISSUE
			//objects_in_scene.erase(std::find(objects_in_scene.begin(),
				//objects_in_scene.end(), posterity[0]));

			std::shared_ptr<GameObject> s(posterity[0]);

			objects_in_scene.erase(std::find_if(objects_in_scene.begin(), objects_in_scene.end(),
								   [s](std::shared_ptr<GameObject> const& i) { return i.get() == s.get(); }));

			EventSystem::Instance()->push_event(EventID::SCENE_REMOVED_GAMEOBJECT_FLAG, posterity[0].lock().get());

			//delete posterity[0];
			posterity.erase(posterity.begin());
		}
	}

}

void Scene::clear_scene(bool remove_everything)
{
	std::vector<std::shared_ptr<GameObject>> objects;
	if (remove_everything)
	{
		objects = objects_in_scene;
	}
	else
	{
		for (auto& o : objects_in_scene)
		{
			//only need to get parents
			if (o->get_parent().lock())
				continue;
			if (!o->has_component<DontDestroyOnLoad>())
			{
				objects.push_back(o);
			}
		}
	}
	for (std::size_t i = 0; i != objects.size(); i++)
		remove_gameobject(objects[i]);
	

}

#pragma region IDATA

Json::Value Scene::serialize_json()
{
	Json::Value obj;

	obj["file-name"] = file_name;
	obj["scene-name"] = scene_name;
	for (auto& o : objects_in_scene)
	{
		//only save parent, parent will save children
		if(!o->get_parent().lock())
			obj["gameobjects"].append(o->serialize_json());
	}

	return obj;
}

Json::Value Scene::serialize_destroyed_objects()
{
	Json::Value obj;
	
	obj["file-name"] = file_name;
	obj["scene-name"] = scene_name;
	for (auto& o : objects_in_scene)
	{
		//only save parent, parent will save children
		if (!o->get_parent().lock() && !o->has_component<DontDestroyOnLoad>())
		{
			obj["gameobjects"].append(o->serialize_json());
	
		}
	}

	return obj;

}

Json::Value Scene::serialize_undestroyed_objects()
{
	Json::Value obj;

	obj["file-name"] = "dont_destroy_on_load_objects.json";

	for (auto& o : objects_in_scene)
	{
		//only save parent, parent will save children
		if (!o->get_parent().lock() && o->has_component<DontDestroyOnLoad>())
			obj["gameobjects"].append(o->serialize_json());
	}

	return obj;
}

void Scene::unserialize_json(Json::Value obj)
{
	file_name = obj["file-name"].asString();
	scene_name = obj["scene-name"].asString();

	for (Json::Value game_object : obj["gameobjects"])
	{
		std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
		
		go->unserialize_json(game_object);


		insert_gameobject(go, false);
	}

	init();

	for (auto& go : objects_in_scene)
		if (!go->is_initialized())
			go->init();

	for (auto& go : objects_in_scene)
		if (!go->is_initialized())
			go->awake();
	for (auto& go : objects_in_scene)
		if (!go->is_initialized())
			go->start();

}

#pragma endregion

void Scene::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::GAMEOBJECT_PARENT_CHANGE:
		insert_sort();
		EventSystem::Instance()->push_event(EventID::SCENE_GAMEOBJECT_ORDER_CHANGE_FLAG);
		break;
	case EventID::GAMEOBJECT_INSTANTIATE:
	{
		// here is the issue, at this point the shared pointer does not exist
		std::shared_ptr<GameObject> new_object(static_cast<GameObject*>(event->get_parameter()));	

		if (!new_object)
			return;

		insert_gameobject(new_object);

		EventSystem::Instance()->push_event(
			EventID::SCENE_ADDED_GAMEOBJECT_FLAG, static_cast<void*>(new_object.get()));

		break;
	}
	case EventID::GAMEOBJECT_DESTROY:
	{
		std::shared_ptr<GameObject>remove_object(static_cast<GameObject*>(event->get_parameter())->self());
		if (!remove_object)
			return;

		remove_gameobject(remove_object);

		EventSystem::Instance()->push_event(EventID::SCENE_REMOVED_GAMEOBJECT_FLAG);

		break;
	}
	case EventID::GAMEOBJECT_COMPONENT_ADDED_FLAG:
	{
		GameObject* obj = static_cast<GameObject*>(event->get_caller().pointer);
		Component* c = static_cast<Component*>(event->get_parameter());

		if (!obj || (!dynamic_cast<ColliderComponent*>(c) && !dynamic_cast<RigidbodyComponent*>(c)))
			return;

		Physics::Instance()->add_to_physics(obj->self());
		break;
	}
	case EventID::GAMEOBJECT_COMPONENT_REMOVED_FLAG:
	{
		if (in_editor)
			break;
		GameObject* obj = static_cast<GameObject*>(event->get_caller().pointer);
		Component* c = static_cast<Component*>(event->get_parameter());

		if (!obj || (!dynamic_cast<ColliderComponent*>(c) && !dynamic_cast<RigidbodyComponent*>(c)))
			return;
		// verify it has both, because as of right now neigther would've been removed
		if (!obj->has_component_of_type<ColliderComponent>() && !obj->has_component<RigidbodyComponent>())
			return;

		Physics::Instance()->remove_from_physics(obj->self());
		
		break;
	}
	}
}

void Scene::insert_sort()
{
	for (int k = 1; k < objects_in_scene.size(); k++)
	{
		std::shared_ptr<GameObject> temp = objects_in_scene[k];
		int j = k - 1;
		while (j >= 0 && temp->get_parent().lock().get() != objects_in_scene[j].get())
		{
			objects_in_scene[j + 1] = objects_in_scene[j];
			j = j - 1;
		}
		objects_in_scene[j + 1] = temp;
	}

}
}