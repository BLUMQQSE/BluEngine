#include "pch.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "core/Renderer.h"
namespace bm98
{
ColliderComponent::ColliderComponent()
{
}

ColliderComponent::~ColliderComponent()
{
	//core::Renderer::Instance()->remove(&collider_bounds);
}

void ColliderComponent::init()
{
	collider_bounds.init();
}

void ColliderComponent::fixed_update()
{
	collider_bounds.set_position(Vector2f(game_object->get_world_position().x + offset.x,
								 game_object->get_world_position().y + offset.y));
}

void ColliderComponent::on_draw_gizmos()
{
	if (active)
	{
		if (!trigger)
			Gizmo::outline_color = Color::Orange;
		else
			Gizmo::outline_color = Color::LimeGreen;
		Gizmo::draw_convex(collider_bounds);
	}
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

	obj["hitbox-offsetX"] = offset.x;
	obj["hitbox-offsetY"] = offset.y;

	obj["collider-bounds"] = collider_bounds.serialize_json();

	return obj;
}

void ColliderComponent::unserialize_json(Json::Value obj)
{

	collision_check_type = string_to_collisiondetection(obj["collision-check-type"].asString());
	trigger = obj["trigger"].asBool();
	active = obj["active"].asBool();

	offset.x = obj["hitbox-offsetX"].asFloat();
	offset.y = obj["hitbox-offsetY"].asFloat();

	collider_bounds.unserialize_json(obj["collider-bounds"]);

}

#pragma endregion

std::vector<Editor::SerializedVar> ColliderComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;

	variables.push_back(Editor::SerializedVar("trigger", static_cast<void*>(&trigger), 
						Var::Type::Bool));
	variables.push_back(Editor::SerializedVar("offset", (void*)&offset, 
						Var::Type::Vector2f));
	variables.push_back(Editor::SerializedVar("collision_check_type", &collision_check_type,
		Var::Type::Dropdown, collisiondetection_to_vector()));


	return variables;
}

void ColliderComponent::editor_update()
{
	collider_bounds.init();
}

void ColliderComponent::set_active(bool active)
{
	Component::set_active(active);
}

void ColliderComponent::set_world_position(Vector2f pos)
{
	collider_bounds.set_position(pos + offset);
}

FloatConvex& ColliderComponent::get_collider_bounds()
{
	return collider_bounds;
}

Vector2f ColliderComponent::get_closest_point(Vector2f pos)
{
	return collider_bounds.get_closest_point(pos);
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