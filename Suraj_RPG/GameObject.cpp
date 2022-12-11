#include "pch.h"
#include "GameObject.h"
#include "Time.h"
#include "Renderer.h"
#include "Debug.h"
#include "Collisions.h"

#include "TilemapComponent.h"
#include "ChildAnimationComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSpriteComponent.h"
#include "AnimationComponent.h"
#include "PlayerController.h"

namespace bm98
{
using namespace core;
GameObject::GameObject()
{
	init_variables();
}

GameObject::~GameObject()
{
	for (auto& c : components)
	{
		c.reset();
	}
	delete render_object;
}

void GameObject::init_object()
{

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
	start_components();
}

void GameObject::update()
{
	if (!active)
		return;
	for (auto& c : components)
	{
		c->update();
	}

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
	for (auto& c : components)
	{
		c->late_update();
	}
}

void GameObject::fixed_update()
{
	if (!active)
		return;
	for (auto& c : components)
	{
		c->fixed_update();
	}

	/*
	if (rigidbody_component)
		rigidbody_component->fixed_update();
	if (box_collider_component)
		box_collider_component->fixed_update(); //this needs be after rigid updates so we get next frames bounds, rather than this frames	
	*/
}

void GameObject::on_collision_enter(Collision info)
{
	if (parent)
		parent->on_collision_enter(info);
	for (auto& c : components)
		c->on_collision_enter(info);
}

void GameObject::on_collision_stay(Collision info)
{
	if (parent)
		parent->on_collision_stay(info);
	for (auto& c : components)
		c->on_collision_stay(info);
}

void GameObject::on_collision_exit(Collision info)
{
	if (parent)
		parent->on_collision_exit(info);
	for (auto& c : components)
		c->on_collision_exit(info);
}

void GameObject::on_trigger_enter(Collider info)
{
	if (parent)
		parent->on_trigger_enter(info);
	for (auto& c : components)
		c->on_trigger_enter(info);
}

void GameObject::on_trigger_stay(Collider info)
{
	if (parent)
		parent->on_trigger_stay(info);
	for (auto& c : components)
		c->on_trigger_stay(info);
}

void GameObject::on_trigger_exit(Collider info)
{
	if (parent)
		parent->on_trigger_exit(info);
	for (auto& c : components)
		c->on_trigger_exit(info);
}

void GameObject::add_to_buffer(sf::View* view)
{
	if (!active)
		return;
	if (has_component<SpriteComponent>())
	{
		Renderer::add(
			Renderer::RenderObject(&get_component<SpriteComponent>().get_sprite(),
				get_component<SpriteComponent>().get_layer(),
				get_component<SpriteComponent>().get_order(),
				view
			));
	}
	else
	{
		//TODO: remove once components working correctly
		//render_object->view = view;
		//Renderer::add(get_render_object());
		//Renderer::add(Renderer::RenderObject(&sprite, SortingLayer::ACTOR, 0, view));
	}
	
	for (auto& c : components)
	{
		c->add_to_buffer(view);
	}
}

Renderer::RenderObject& GameObject::get_render_object()
{
	return *render_object;
}

const bool& GameObject::is_active()
{
	return active;
}

void GameObject::set_active(bool& active)
{
	this->active = active;
}

void GameObject::set_parent(GameObject* parent)
{
	this->parent = parent;
}

void GameObject::add_child(GameObject* child)
{
	children.insert(child);
}

void GameObject::set_position(const float x, const float y)
{
	transform.position = Vector2f(x, y);
	if (has_component<SpriteComponent>())
		get_component<SpriteComponent>().set_position(x, y);
}

const GameObject::Info& GameObject::get_info() const
{
	return info;
}

const GameObject::Transform& GameObject::get_transform() const
{
	return transform;
}

GameObject* GameObject::get_parent()
{
	return parent;
}

std::set<GameObject*> GameObject::get_children()
{
	return children;
}

const bool& GameObject::check_for_child(GameObject* game_object) const
{
	return children.find(game_object) != children.end();
}

#pragma region IDATA

Json::Value GameObject::serialize_json()
{
	Json::Value obj;
	obj["Name"] = info.name;
	obj["tag"] = Global::tag_to_string(info.tag);
	obj["physics-layer"] = Global::physics_layer_to_string(info.layer);
	obj["position-x"] = transform.position.x;
	obj["position-y"] = transform.position.y;
	obj["rotation-x"] = transform.rotation.x;
	obj["rotation-y"] = transform.rotation.y;
	obj["scale-x"] = transform.scale.x;
	obj["scale-y"] = transform.scale.y;
	for (auto& c : components)
	{
		Json::Value obj2;
		obj2["name"] = typeid(*c).name();
		obj2["value"] = c->serialize_json();
		obj["Components"].append(obj2);
	}
	std::cout << "ayo" << obj["Components"].size() << "\n";
	return obj;
}

void GameObject::unserialize_json(Json::Value obj)
{
	info.name = obj["Name"].asString();
	info.tag = Global::string_to_tag(obj["tag"].asString());
	info.layer = Global::string_to_physics_layer(obj["physics-layer"].asString());
	transform.position.x = obj["position-x"].asFloat();
	transform.position.y = obj["position-y"].asFloat();
	transform.rotation.x = obj["rotation-x"].asFloat();
	transform.rotation.y = obj["rotation-y"].asFloat();
	transform.scale.x = obj["scale-x"].asFloat();
	transform.scale.y = obj["scale-y"].asFloat();

	std::unordered_map<std::string, Json::Value> comps;

	for (Json::Value component : obj["Components"])
	{

		if (component["name"].asString() == typeid(TilemapComponent).name())
		{
			comps[typeid(TilemapComponent).name()] = component["value"];
			add_component<TilemapComponent>();
		}
		if (component["name"].asString() == typeid(SpriteComponent).name())
		{
			comps[typeid(SpriteComponent).name()] = component["value"];
			add_component<SpriteComponent>();
		}
		if (component["name"].asString() == typeid(AnimatedSpriteComponent).name())
		{
			comps[typeid(AnimatedSpriteComponent).name()] = component["value"];
			add_component<AnimatedSpriteComponent>();
		}
		if (component["name"].asString() == typeid(AnimationComponent).name())
		{
			comps[typeid(AnimationComponent).name()] = component["value"];
			add_component<AnimationComponent>();
		}
		if (component["name"].asString() == typeid(BoxColliderComponent).name())
		{
			comps[typeid(BoxColliderComponent).name()] = component["value"];
			add_component<BoxColliderComponent>();
		}
		if (component["name"].asString() == typeid(ChildAnimationComponent).name())
		{
			comps[typeid(ChildAnimationComponent).name()] = component["value"];
			add_component<ChildAnimationComponent>();
		}
		if (component["name"].asString() == typeid(RigidbodyComponent).name())
		{
			comps[typeid(RigidbodyComponent).name()] = component["value"];
			add_component<RigidbodyComponent>();
		}
		if (component["name"].asString() == typeid(PlayerController).name())
		{
			comps[typeid(PlayerController).name()] = component["value"];
			add_component<PlayerController>();
		}
	}

	init_components();

	for (auto& c : components)
	{
		c->unserialize_json(comps.at(typeid(*c).name()));
	}
	//for (auto& c : components)
	//	c->unserialize_json(obj["Components"][c->get_name()]);


	awake_components();
	start_components();

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