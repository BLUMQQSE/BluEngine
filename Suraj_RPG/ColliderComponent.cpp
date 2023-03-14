#include "pch.h"
#include "ColliderComponent.h"
#include "GameObject.h"
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
	//return collider_bounds.intersects(collider);
	return false;
}

#pragma region IData

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

#pragma endregion

std::vector<Editor::SerializedVar> ColliderComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;

	variables.push_back(Editor::SerializedVar("trigger", static_cast<void*>(&trigger), Editor::VarType::Bool));
	variables.push_back(Editor::SerializedVar("X offset", static_cast<void*>(&offsetX), 
		Editor::VarType::Float));
	variables.push_back(Editor::SerializedVar("Y offset", static_cast<void*>(&offsetY),
		Editor::VarType::Float));
	variables.push_back(Editor::SerializedVar("collision_check_type", &collision_check_type,
		Editor::VarType::Dropdown, collisiondetection_to_vector()));


	return variables;
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

const bool ColliderComponent::is_trigger() const
{
	return trigger;
}

const bool ColliderComponent::is_active()
{
	return active;
}


}