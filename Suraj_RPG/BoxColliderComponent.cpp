#include "pch.h"
#include "BoxColliderComponent.h"
#include "Renderer.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
namespace bm98
{
using namespace core;

BoxColliderComponent::BoxColliderComponent()
{
	set_sorting_layer(SortingLayer::UI, false);
}

BoxColliderComponent::BoxColliderComponent(float offset_x,
	float offset_y, float width, float height, bool trigger,
	CollisionDetection collision_check_type)
	: width(width), height(height)
{
	this->offsetX = offset_x;
	this->offsetY = offset_y;
	this->trigger = trigger;
	this->active = true;
	this->collision_check_type = collision_check_type;

	collider_bounds = FloatConvex::polygon
	(
		Vector2f(game_object->get_world_position().x + offsetX,
			game_object->get_world_position().y + offsetY),
		{ {Vector2f(0, 0), Vector2f(0, height), Vector2f(width, height), Vector2f(width, 0)} }
	);

	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	collider_bounds.setOutlineColor(sf::Color::Red);

}

BoxColliderComponent::~BoxColliderComponent()
{
	Renderer::remove(&collider_bounds);
}

void BoxColliderComponent::init()
{
	Renderer::add(Renderer::RenderObject(&collider_bounds, this));
}

void BoxColliderComponent::awake()
{	
	
	collider_bounds = FloatConvex::polygon
	(
		Vector2f(game_object->get_world_position().x + offsetX,
			game_object->get_world_position().y + offsetY),
		{ {Vector2f(0, 0), Vector2f(0, height), Vector2f(width, height), Vector2f(width, 0)} }
	);
	

	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	collider_bounds.setOutlineColor(sf::Color::Red);

}

void BoxColliderComponent::update()
{
	
}

void BoxColliderComponent::fixed_update()
{
	collider_bounds.set_position(Vector2f(game_object->get_world_position().x + offsetX,
		game_object->get_world_position().y + offsetY));
}

void BoxColliderComponent::add_to_buffer(sf::View* view)
{
	set_view(view);
	//Renderer::add(Renderer::RenderObject(&hitbox, render, SortingLayer::UI, 0, &view));
}

#pragma region IData

Json::Value BoxColliderComponent::serialize_json()
{
	Json::Value obj;

	obj = ColliderComponent::serialize_json();

	obj["hitbox-width"] = width;
	obj["hitbox-height"] = height;

	return obj;
}
void BoxColliderComponent::unserialize_json(Json::Value obj)
{
	ColliderComponent::unserialize_json(obj);

	width = obj["hitbox-width"].asFloat();
	height = obj["hitbox-height"].asFloat();

}

#pragma endregion

bool BoxColliderComponent::check_intersect(const sf::FloatRect& frect)
{
	return false;
}

bool BoxColliderComponent::check_outer_intersect(const sf::FloatRect& frect)
{
	return false;
}

const sf::FloatRect BoxColliderComponent::get_bounds() const
{
	return sf::FloatRect();
}

const sf::FloatRect BoxColliderComponent::get_outer_bounds() const
{
	return sf::FloatRect();
}

void BoxColliderComponent::set_hitbox(float width, float height, 
	float offset_x, float offset_y)
{
	active = true;
	this->offsetX = offset_x;
	this->offsetY = offset_y;
	this->width = width;
	this->height = height;
	collider_bounds = FloatConvex::polygon
	(
		Vector2f(game_object->get_world_position().x + offsetX,
			game_object->get_world_position().y + offsetY),
		{ {Vector2f(0, 0), Vector2f(0, height), Vector2f(width, height), Vector2f(width, 0)} }
	);

	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	collider_bounds.setOutlineColor(sf::Color::Red);
}

void BoxColliderComponent::set_collision_detection(CollisionDetection col)
{
	this->collision_check_type = col;
}

void BoxColliderComponent::set_trigger(bool trig)
{
	this->trigger = trig;
}

Vector2f BoxColliderComponent::get_offset()
{
	return Vector2f(offsetX, offsetY);
}

}