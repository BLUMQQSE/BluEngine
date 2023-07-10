#include "pch.h"
#include "BoxColliderComponent.h"
#include "core/Renderer.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
namespace bm98
{
using namespace core;

BoxColliderComponent::BoxColliderComponent()
{
}

BoxColliderComponent::BoxColliderComponent(float offset_x,
	float offset_y, float width, float height, bool trigger,
	CollisionDetection collision_check_type)
{
	this->offset.x = offset_x;
	this->offset.y = offset_y;
	offset = Vector2f(width, height);
	this->trigger = trigger;
	this->active = true;
	this->collision_check_type = collision_check_type;

	collider_bounds = FloatConvex::Polygon
	(
		Vector2f(game_object->get_world_position().x + offset.x,
			game_object->get_world_position().y + offset.y),
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

	//collider_bounds.set_position(game_object->get_world_position());
}

void BoxColliderComponent::awake()
{	
}

void BoxColliderComponent::update()
{
}

void BoxColliderComponent::add_to_buffer(sf::View* view)
{
}

void BoxColliderComponent::on_draw_gizmos()
{
	ColliderComponent::on_draw_gizmos();
	//Gizmo::fill_color = Gizmo::outline_color;
	//Gizmo::outline_color = Color::Transparent;
	//Gizmo::draw_line(collider_bounds.get_position(), collider_bounds.get_position() +
	//				 Vector2f(collider_bounds.getGlobalBounds().width - 2, collider_bounds.getGlobalBounds().height - 2));
}

#pragma region IData

Json::Value BoxColliderComponent::serialize_json()
{
	Json::Value obj;

	obj = ColliderComponent::serialize_json();

	obj["hitbox-width"] = size.x;
	obj["hitbox-height"] = size.y;

	return obj;
}
void BoxColliderComponent:: unserialize_json(Json::Value obj)
{
	ColliderComponent::unserialize_json(obj);

	size.x = obj["hitbox-width"].asFloat();
	size.y = obj["hitbox-height"].asFloat();

}

#pragma endregion

std::vector<Editor::SerializedVar> BoxColliderComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;

	variables = ColliderComponent::get_editor_values();

	variables.push_back(Editor::SerializedVar("size", (void*)&size, Var::Type::Vector2f));

	return variables;
}

void BoxColliderComponent::editor_update()
{
	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	if (trigger)
		collider_bounds.setOutlineColor(sf::Color(135, 235, 162, 200));
	else
		collider_bounds.setOutlineColor(sf::Color(255, 20, 20, 200));

	collider_bounds = FloatConvex::Rectangle(game_object->get_world_position() + offset, size);
	
	collider_bounds.setFillColor(sf::Color::Transparent);
	collider_bounds.setOutlineThickness(-1.f);
	if (trigger)
		collider_bounds.setOutlineColor(sf::Color(135, 235, 162, 150));
	else
		collider_bounds.setOutlineColor(sf::Color(255, 20, 20, 150));
	//collider_bounds.set_position(game_object->get_world_position());
}

bool BoxColliderComponent::check_intersect(const sf::FloatRect& frect)
{
	return false;
}

bool BoxColliderComponent::check_outer_intersect(const sf::FloatRect& frect)
{
	return false;
}

void BoxColliderComponent::set_hitbox(float width, float height, 
	float offset_x, float offset_y)
{
	active = true;
	this->offset.x = offset_x;
	this->offset.y = offset_y;
	this->size.x = width;
	this->size.y = height;
	collider_bounds = FloatConvex::Polygon
	(
		Vector2f(game_object->get_world_position().x + offset.x,
			game_object->get_world_position().y + offset.y),
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
	return offset;
}

const bool BoxColliderComponent::blocks_light()
{
	return true;
}

}