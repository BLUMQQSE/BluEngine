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
	:offsetX(offset_x), offsetY(-offset_y), trigger(trigger),
	collision_check_type(collision_check_type), width(width), height(height)
{
	active = true;
	hitbox.setPosition(game_object->get_transform().position.x + offsetX,
		game_object->get_transform().position.y + offsetY);
	hitbox.setSize(sf::Vector2f(width, height));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(2.f);
	hitbox.setOutlineColor(sf::Color::Blue);
}

BoxColliderComponent::~BoxColliderComponent()
{
	Renderer::remove(&hitbox);
}

void BoxColliderComponent::init()
{
	Renderer::add(Renderer::RenderObject(&hitbox, this));
}

void BoxColliderComponent::awake()
{	
	hitbox.setPosition(game_object->get_transform().position.x + offsetX,
		game_object->get_transform().position.y + offsetY);
	hitbox.setSize(sf::Vector2f(width, height));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(2.f);
	hitbox.setOutlineColor(sf::Color::Blue);
}

void BoxColliderComponent::update()
{
	hitbox.setPosition(game_object->get_transform().position.x + offsetX,
		game_object->get_transform().position.y + offsetY);
}

void BoxColliderComponent::fixed_update()
{

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

	obj["trigger"] = trigger;
	obj["active"] = active;
	obj["collision-check-type"] = collisiondetection_to_string(collision_check_type);
	obj["hitbox-width"] = hitbox.getSize().x;
	obj["hitbox-height"] = hitbox.getSize().y;
	obj["hitbox-offsetX"] = offsetX;
	obj["hitbox-offsetY"] = offsetY;

	return obj;
}
void BoxColliderComponent::unserialize_json(Json::Value obj)
{
	trigger = obj["trigger"].asBool();
	active = obj["active"].asBool();
	collision_check_type = string_to_collisiondetection(obj["collision-check-type"].asString());
	width = obj["hitbox-width"].asFloat();
	height = obj["hitbox-height"].asFloat();
	offsetX = obj["hitbox-offsetX"].asFloat();
	offsetY = obj["hitbox-offsetY"].asFloat();

}

#pragma endregion

bool BoxColliderComponent::check_intersect(const sf::FloatRect& frect)
{
	return hitbox.getGlobalBounds().intersects(frect);
}

bool BoxColliderComponent::check_outer_intersect(const sf::FloatRect& frect)
{
	return get_outer_bounds().intersects(frect);
}

const sf::FloatRect BoxColliderComponent::get_bounds() const
{
	return hitbox.getGlobalBounds();
}

const sf::FloatRect BoxColliderComponent::get_outer_bounds() const
{
	return sf::FloatRect(
		hitbox.getGlobalBounds().left - 1,
		hitbox.getGlobalBounds().top - 1,
		hitbox.getGlobalBounds().width + 2,
		hitbox.getGlobalBounds().height + 2
	);;
}

const bool BoxColliderComponent::is_trigger() const
{
	return trigger;
}

const bool BoxColliderComponent::is_active()
{
	return active;
}

void BoxColliderComponent::set_active(const bool& active)
{
	this->active = active;
}

void BoxColliderComponent::set_hitbox(float x, float y, float width, float height, float offset_x, float offset_y)
{
	active = true;
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(2.f);
	hitbox.setOutlineColor(sf::Color::Blue);

	this->offsetX = offset_x;
	this->offsetY = offset_y;
	this->width = width;
	this->height = height;
	hitbox.setPosition(sf::Vector2f(x + offset_x, y + offset_y));
	hitbox.setSize(sf::Vector2f(width, height));
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