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
	name = "BoxColliderComponent";
}

BoxColliderComponent::BoxColliderComponent(sf::Sprite& sprite, float offset_x,
	float offset_y, float width, float height, bool trigger,
	CollisionDetection collision_check_type)
	:sprite(&sprite), offsetX(offset_x), offsetY(-offset_y), trigger(trigger),
	collision_check_type(collision_check_type), width(width), height(height)
{
	name = "BoxColliderComponent";
	active = true;
	hitbox.setPosition(sprite.getPosition().x + offset_x, sprite.getPosition().y + offset_y);
	hitbox.setSize(sf::Vector2f(width, height));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(2.f);
	hitbox.setOutlineColor(sf::Color::Blue);
}

void BoxColliderComponent::init()
{
	sprite = &game_object->get_component<SpriteComponent>().get_sprite();
	collision_check_type = CollisionDetection::DISCRETE;
	trigger = false;
	active = true;
	width = 32;
	height = 50;
	offsetY = 14;
	offsetX = 16;

	hitbox.setPosition(sprite->getPosition().x + offsetX, sprite->getPosition().y + offsetY);
	hitbox.setSize(sf::Vector2f(width, height));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(2.f);
	hitbox.setOutlineColor(sf::Color::Blue);
}

void BoxColliderComponent::update()
{
	hitbox.setPosition(this->sprite->getPosition().x + offsetX,
		this->sprite->getPosition().y + offsetY);
}

void BoxColliderComponent::fixed_update()
{

}

void BoxColliderComponent::add_to_buffer(sf::View* view)
{
	Renderer::add(Renderer::RenderObject(&hitbox, SortingLayer::UI, 0, view));
}

#pragma region IData

Json::Value BoxColliderComponent::serialize_json()
{
	Json::Value obj;

	obj["name"] = name;
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
	name = obj["name"].asString();
	trigger = obj["trigger"].asBool();
	active = obj["active"].asBool();
	collision_check_type = string_to_collisiondetection(obj["collision-check-type"].asString());
	width = obj["hitbox-width"].asFloat();
	height = obj["hitbox-height"].asFloat();
	offsetX = obj["hitbox-offsetX"].asFloat();
	offsetY = obj["hitbox-offsetY"].asFloat();

}

#pragma endregion

BoxColliderComponent::~BoxColliderComponent()
{
}

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