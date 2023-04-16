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
	set_sorting_layer(Sorting::Layer::UI, false);
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

	collider_bounds = FloatConvex::Polygon
	(
		Vector2f(game_object->get_world_position().x + offsetX,
			game_object->get_world_position().y + offsetY),
		{ {Vector2f(0, 0), Vector2f(0, height), Vector2f(width, height), Vector2f(width, 0)} }
	);

	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	if (trigger)
		collider_bounds.setOutlineColor(sf::Color(135, 235, 162, 150));
	else
		collider_bounds.setOutlineColor(sf::Color(255, 20, 20, 150));

}

BoxColliderComponent::~BoxColliderComponent()
{
}

void BoxColliderComponent::init()
{
	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	if (trigger)
		collider_bounds.setOutlineColor(sf::Color(135, 235, 162, 200));
	else
		collider_bounds.setOutlineColor(sf::Color(255, 20, 20, 200));

	collider_bounds.set_position(game_object->get_world_position());
}

void BoxColliderComponent::awake()
{	
	Renderer::Instance()->add(Renderer::RenderObject(&collider_bounds, this));
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
void BoxColliderComponent:: unserialize_json(Json::Value obj)
{
	ColliderComponent::unserialize_json(obj);

	width = obj["hitbox-width"].asFloat();
	height = obj["hitbox-height"].asFloat();

}

#pragma endregion

std::vector<Editor::SerializedVar> BoxColliderComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;

	variables = ColliderComponent::get_editor_values();

	variables.push_back(Editor::SerializedVar("width", & width, Var::Type::Float));
	variables.push_back(Editor::SerializedVar("height", &height, Var::Type::Float));


	return variables;
}

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
	collider_bounds = FloatConvex::Polygon
	(
		Vector2f(game_object->get_world_position().x + offsetX,
			game_object->get_world_position().y + offsetY),
		{ {Vector2f(0, 0), Vector2f(width, 0), Vector2f(width, height), Vector2f(0, height)} }
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