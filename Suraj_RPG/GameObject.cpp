#include "pch.h"
#include "GameObject.h"
#include "Time.h"
#include "Renderer.h"
#include "Debug.h"
#include "Collisions.h"
#include "ChildAnimationComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
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

void GameObject::update()
{
	if (!active)
		return;
	for (auto& c : components)
	{
		c->update();
	}

	if(!parent)
		transform.position = sprite.getPosition();
	if (parent)
		sprite.setPosition(parent->transform.position + transform.local_position);

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
	
}

void GameObject::on_collision_stay(Collision info)
{
	if (parent)
		parent->on_collision_stay(info);
	
}

void GameObject::on_collision_exit(Collision info)
{
	if (parent)
		parent->on_collision_exit(info);
	
}

void GameObject::on_trigger_enter(Collider info)
{
	if (parent)
		parent->on_trigger_enter(info);
	
}

void GameObject::on_trigger_stay(Collider info)
{
	if (parent)
		parent->on_trigger_stay(info);
	
}

void GameObject::on_trigger_exit(Collider info)
{
	if (parent)
		parent->on_trigger_exit(info);
	
}

void GameObject::add_to_buffer(sf::View* view)
{
	if (!active)
		return;
	render_object->view = view;
	Renderer::add(get_render_object());
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

void GameObject::set_sprite_texture(sf::Texture& texture)
{
	sprite.setTexture(texture);
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
	sprite.setPosition(x, y);
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
	return obj;
}

void GameObject::unserialize_json(Json::Value obj)
{

}

#pragma endregion

void GameObject::init_variables()
{
	active = true;
}

}