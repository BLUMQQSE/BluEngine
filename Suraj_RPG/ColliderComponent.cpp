#include "pch.h"
#include "ColliderComponent.h"
namespace bm98
{
ColliderComponent::ColliderComponent()
{
}

ColliderComponent::~ColliderComponent()
{
}

bool ColliderComponent::intersects(const FloatConvex collider)
{
	return collider_bounds.intersects(collider);
}

bool ColliderComponent::outer_intersects(const FloatConvex collider)
{
	return outer_collider_bounds.intersects(collider);
}

Json::Value ColliderComponent::serialize_json()
{
	Json::Value obj;
	

	obj["active"] = active;
	obj["collision-check-type"] = collisiondetection_to_string(collision_check_type);

	obj["trigger"] = trigger;

	obj["hitbox-offsetX"] = offsetX;
	obj["hitbox-offsetY"] = offsetY;

	obj["collider-bounds"] = collider_bounds.serialize_json();

	return obj;
}

void ColliderComponent::unserialize_json(Json::Value obj)
{

	collision_check_type = string_to_collisiondetection(obj["collision-check-type"].asString());
	trigger = obj["trigger"].asBool();
	active = obj["active"].asBool();

	offsetX = obj["hitbox-offsetX"].asFloat();
	offsetY = obj["hitbox-offsetY"].asFloat();

	collider_bounds.unserialize_json(obj["collider_bounds"]);
}

void ColliderComponent::set_active(bool active)
{
	Component::set_active(active);
	set_render(active);
}

FloatConvex ColliderComponent::get_collider_bounds()
{
	return collider_bounds;
}

FloatConvex ColliderComponent::get_outer_collider_bounds()
{
	return outer_collider_bounds;
}

const bool ColliderComponent::is_trigger() const
{
	return trigger;
}

const bool ColliderComponent::is_active()
{
	return active;
}


}