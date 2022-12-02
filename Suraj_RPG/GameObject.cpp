#include "pch.h"
#include "GameObject.h"
#include "Time.h"
#include "Renderer.h"
#include "Debug.h"
#include "Collisions.h"
namespace bm98
{
using namespace core;
#pragma region PUBLIC

GameObject::GameObject()
{
	init_variables();
}

GameObject::~GameObject()
{
	delete box_collider_component;
	delete rigidbody_component;
	delete animation_component;
	delete child_animation_component;
	delete animated_sprite_component;
	delete render_object;
}

void GameObject::init_object()
{
	
}

void GameObject::update()
{
	if (!active)
		return;
	if (rigidbody_component)
	{
		rigidbody_component->update();
		transform.position = sprite.getPosition();
	}
	if (box_collider_component)
		box_collider_component->update();
	if (animated_sprite_component)
		animated_sprite_component->update();
	if (child_animation_component)
		child_animation_component->update();

	if (parent)
		sprite.setPosition(parent->transform.position + transform.local_position);
}

void GameObject::late_update()
{
	if (!active)
		return;
}

void GameObject::fixed_update()
{
	if (!active)
		return;
	if (rigidbody_component)
		rigidbody_component->fixed_update();
	if (box_collider_component)
		box_collider_component->fixed_update(); //this needs be after rigid updates so we get next frames bounds, rather than this frames	
	
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
	if(box_collider_component)
		box_collider_component->add_to_buffer(view);
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

void GameObject::add_box_collider_component(sf::Sprite& sprite, float offset_x,
	float offset_y, float width, float height, bool trigger)
{
	box_collider_component = new BoxColliderComponent(sprite, offset_x, offset_y, 
		width, height, trigger);
}

void GameObject::add_rigidbody_component(const float maxVelocity,
	const float acceleration, const float deceleration)
{
	rigidbody_component = new RigidbodyComponent(sprite, maxVelocity,
		acceleration, deceleration);
}

void GameObject::add_animation_component()
{
	animation_component = new AnimationComponent(sprite, texture_sheet);
}

void GameObject::add_child_animation_component()
{
	if (!parent)
		return;
	if (!parent->get_animation_component())
		return;
	
	child_animation_component = new ChildAnimationComponent(sprite, texture_sheet, *parent->get_animation_component());
}

void GameObject::add_animated_sprite_component(sf::Texture& texture_sheet, sf::IntRect animation_rect,
	float animation_timer, int frame_width, int frame_height)
{
	animated_sprite_component = new AnimatedSpriteComponent(sprite, texture_sheet, animation_rect, 
		animation_timer, frame_width, frame_height);
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
	transform.position = sf::Vector2f(x, y);
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


BoxColliderComponent* GameObject::get_box_collider_component()
{
	return box_collider_component;
}

RigidbodyComponent* GameObject::get_rigidbody_component()
{
	return rigidbody_component;
}

AnimationComponent* GameObject::get_animation_component()
{
	return animation_component;
}

ChildAnimationComponent* GameObject::get_child_animation_component()
{
	return child_animation_component;
}

AnimatedSpriteComponent* GameObject::get_animated_sprite_component()
{
	return animated_sprite_component;
}

#pragma endregion

#pragma region PRIVATE

void GameObject::init_variables()
{
	box_collider_component = nullptr;
	rigidbody_component = nullptr;
	animation_component = nullptr;
	animated_sprite_component = nullptr;
	active = true;
}

#pragma endregion
}