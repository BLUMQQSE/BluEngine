#include "pch.h"
#include "GameObject.h"
#include "Time.h"
#include "Renderer.h"
#include "Debug.h"
#include "Collisions.h"
#include "SceneManager.h"

#include "TilemapComponent.h"
#include "ChildAnimationComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSpriteComponent.h"
#include "AnimationComponent.h"
#include "PlayerController.h"
#include "ButtonComponent.h"
#include "DontDestroyOnLoad.h"
#include "SceneChange.h"
#include "CameraComponent.h"
#include "AudioSource.h"
#include "Inventory.h"

namespace bm98
{
using namespace core;
GameObject::GameObject()
{
	unique_runtime_id = get_unique_id();
	init_variables();
}

GameObject::~GameObject()
{
	for (auto& c : components)
	{
		delete c;
	}
	components.clear();
}

void GameObject::init()
{
	init_components();
}

void GameObject::awake()
{

	set_position(transform.position.x, transform.position.y);
	
	awake_components();

}

void GameObject::start()
{
	initialized = true;
	start_components();
}

void GameObject::update()
{
	for (auto& c_t_r : components_to_remove)
	{
		components.erase(std::find(components.begin(), components.end(), c_t_r));
		delete c_t_r;
	}
	components_to_remove.clear();
	
	if (!active)
		return;
	for(std::size_t i = 0; i != components.size(); i++)
		components[i]->update();
	

	//if(!parent)
		//transform.position = sprite.getPosition();
	
	if (parent)
		transform.position = parent->transform.position + transform.local_position;
	
	//sprite.setPosition(transform.position);
	
}

void GameObject::late_update()
{
	if (!active)
		return;
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->late_update();
}

void GameObject::fixed_update()
{
	if (!active)
		return;
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->fixed_update();
}

void GameObject::on_collision_enter(Collision info)
{
	if (parent)
		parent->on_collision_enter(info);
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->on_collision_enter(info);
}

void GameObject::on_collision_stay(Collision info)
{
	if (parent)
		parent->on_collision_stay(info);
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->on_collision_stay(info);
}

void GameObject::on_collision_exit(Collision info)
{
	if (parent)
		parent->on_collision_exit(info);
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->on_collision_exit(info);
}

void GameObject::on_trigger_enter(Collider info)
{
	if (parent)
		parent->on_trigger_enter(info);
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->on_trigger_enter(info);
}

void GameObject::on_trigger_stay(Collider info)
{
	if (parent)
		parent->on_trigger_stay(info);
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->on_trigger_stay(info);
}

void GameObject::on_trigger_exit(Collider info)
{
	if (parent)
		parent->on_trigger_exit(info);
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->on_trigger_exit(info);
}

void GameObject::add_to_buffer(sf::View* view)
{
	if (!active)
	{
		return;
	}
	for (std::size_t i = 0; i != components.size(); i++)
		components[i]->add_to_buffer(view);

	for (auto& ch : children)
		ch->add_to_buffer(view);
	
}

const bool& GameObject::is_active()
{
	return active;
}

void GameObject::set_active(bool active)
{
	this->active = active;
	for (std::size_t i = 0; i < components.size(); i++)
		components[i]->set_active(active);
	for (auto& ch : children)
		ch->set_active(active);
}

void GameObject::set_render(bool render)
{
	for (auto& c : components)
	{
		if (dynamic_cast<IRenderable*>(c))
			dynamic_cast<IRenderable*>(c)->set_render(render);
	}
}

void GameObject::set_parent(GameObject* parent)
{
	this->parent = parent;
}

void GameObject::add_child(GameObject* child)
{
	children.push_back(child);
}

void GameObject::remove_child(GameObject* child)
{
	children.erase(std::find(children.begin(), children.end(), child));
}

void GameObject::set_position(const float x, const float y)
{
	transform.position = Vector2f(x, y);
	if (has_component<SpriteComponent>())
		get_component<SpriteComponent>().set_position(x, y);
}

void GameObject::set_local_position(const float x, const float y)
{
	transform.local_position = Vector2f(x, y);
}

const size_t GameObject::get_unique_runtime_id() const
{
	return unique_runtime_id;
}

const GameObject::Info& GameObject::get_info() const
{
	return info;
}

const GameObject::Transform& GameObject::get_transform() const
{
	return transform;
}

const sf::Vector2f GameObject::get_center() const
{
	if (has_component<SpriteComponent>())
	{
		sf::FloatRect sc = get_component<SpriteComponent>().get_sprite().getGlobalBounds();
		return sf::Vector2f(
			sc.left + sc.width / 2,
			sc.top + sc.height / 2
		);
	}
	if (has_component<BoxColliderComponent>())
	{
		sf::FloatRect bc = get_component<BoxColliderComponent>().get_bounds();
		return sf::Vector2f(
			bc.left + bc.width / 2,
			bc.top + bc.height / 2
		);
	}
	return get_transform().position + get_transform().local_position;
}

GameObject* GameObject::get_parent()
{
	return parent;
}

GameObject* GameObject::get_greatest_ancestor()
{
	GameObject* o = parent;
	while (o->get_parent())
	{
		o = o->get_parent();
	}
	return o;
}

std::vector<GameObject*> GameObject::get_children()
{
	return children;
}

std::vector<GameObject*> GameObject::get_all_posterity()
{
	std::vector<GameObject*> all_children = children;
	for (std::size_t i = 0; i != children.size(); i++)
	{
		std::vector<GameObject*> posterity = children[i]->get_all_posterity();
		all_children.insert(all_children.end(), posterity.begin(), posterity.end());
		
	}
	return all_children;
}

const bool GameObject::check_for_child(GameObject* game_object) const
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i] == game_object)
			return true;

	return false;
}

const bool GameObject::check_for_child(std::string name) const
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i]->get_info().name == name)
			return true;

	return false;
}

GameObject* GameObject::get_child(std::string name)
{
	for (std::size_t i = 0; i != children.size(); i++)
		if (children[i]->get_info().name == name)
			return children[i];
	return nullptr;
}

const bool GameObject::is_initialized() const
{
	return initialized;
}

#pragma region IDATA

Json::Value GameObject::serialize_json()
{
	Json::Value obj;

	obj["info"] = info.serialize_json();
	obj["transform"] = transform.serialize_json();

	for (auto& c : components)
	{
		Json::Value obj2;
		obj2["name"] = typeid(*c).name();
		
		obj2["value"] = c->serialize_json();
		obj["components"].append(obj2);
	}
	for (auto& o : children)
	{
		obj["children"].append(o->serialize_json());
	}

	return obj;
}

void GameObject::unserialize_json(Json::Value obj)
{
	info.unserialize_json(obj["info"]);
	transform.unserialize_json(obj["transform"]);

	std::unordered_map<std::string, Json::Value> comps_data;
	for (Json::Value component : obj["components"])
	{
		if (component["name"].asString() == typeid(TilemapComponent).name())
		{
			comps_data[typeid(TilemapComponent).name()] = component["value"];
			add_component<TilemapComponent>();
		}
		if (component["name"].asString() == typeid(SpriteComponent).name())
		{
			comps_data[typeid(SpriteComponent).name()] = component["value"];
			add_component<SpriteComponent>();
		}
		if (component["name"].asString() == typeid(AnimatedSpriteComponent).name())
		{
			comps_data[typeid(AnimatedSpriteComponent).name()] = component["value"];
			add_component<AnimatedSpriteComponent>();
		}
		if (component["name"].asString() == typeid(AnimationComponent).name())
		{
			comps_data[typeid(AnimationComponent).name()] = component["value"];
			add_component<AnimationComponent>();
		}
		if (component["name"].asString() == typeid(BoxColliderComponent).name())
		{
			comps_data[typeid(BoxColliderComponent).name()] = component["value"];
			add_component<BoxColliderComponent>();
		}
		if (component["name"].asString() == typeid(ChildAnimationComponent).name())
		{
			comps_data[typeid(ChildAnimationComponent).name()] = component["value"];
			add_component<ChildAnimationComponent>();
		}
		if (component["name"].asString() == typeid(RigidbodyComponent).name())
		{
			comps_data[typeid(RigidbodyComponent).name()] = component["value"];
			add_component<RigidbodyComponent>();
		}
		if (component["name"].asString() == typeid(PlayerController).name())
		{
			comps_data[typeid(PlayerController).name()] = component["value"];
			add_component<PlayerController>();
		}
		if (component["name"].asString() == typeid(ButtonComponent).name())
		{
			comps_data[typeid(ButtonComponent).name()] = component["value"];
			add_component<ButtonComponent>();
		}
		if (component["name"].asString() == typeid(DontDestroyOnLoad).name())
		{
			comps_data[typeid(DontDestroyOnLoad).name()] = component["value"];
			add_component<DontDestroyOnLoad>();
		}
		if (component["name"].asString() == typeid(SceneChange).name())
		{
			comps_data[typeid(SceneChange).name()] = component["value"];
			add_component<SceneChange>();
		}
		if (component["name"].asString() == typeid(CameraComponent).name())
		{
			comps_data[typeid(CameraComponent).name()] = component["value"];
			add_component<CameraComponent>();
		}
		if (component["name"].asString() == typeid(AudioSource).name())
		{
			comps_data[typeid(AudioSource).name()] = component["value"];
			add_component<AudioSource>();
		}
		if (component["name"].asString() == typeid(Inventory).name())
		{
			comps_data[typeid(Inventory).name()] = component["value"];
			add_component<Inventory>();
		}
	}

	for (auto& c : components)
	{
		c->unserialize_json(comps_data.at(typeid(*c).name()));
	}

	for (Json::Value child : obj["children"])
	{
		GameObject* go = new GameObject();
		go->set_parent(this);
		add_child(go);
		go->unserialize_json(child);
		SceneManager::instantiate_gameobject_on_load(go);
	}

}

#pragma endregion

void GameObject::init_components()
{
	for (auto& c : components)
		c->init();
}

void GameObject::awake_components()
{
	for (auto& c : components)
		c->awake();
}

void GameObject::start_components()
{
	for (auto& c : components)
		c->start();
}

void GameObject::init_variables()
{
	active = true;
}

}