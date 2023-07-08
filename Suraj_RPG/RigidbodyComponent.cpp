#include "pch.h"
#include "RigidbodyComponent.h"
#include "core/Time.h"
#include "core/Renderer.h"
#include "GameObject.h"
#include "SpriteComponent.h"
namespace bm98
{
using namespace core;

RigidbodyComponent::RigidbodyComponent() {}

RigidbodyComponent::RigidbodyComponent(float max_velocity,
	float acceleration, float deceleration)
	:max_velocity(max_velocity), acceleration(acceleration),
	deceleration(deceleration) {}

RigidbodyComponent::~RigidbodyComponent() {}

void RigidbodyComponent::update()
{
	update_movement_state();
	update_orientation();
	Vector2f translation = ((external_velocity * external_dir) + (current_velocity * movement_dir))
		* UNIT_SIZE * Time::Instance()->delta_time();
	game_object->move(translation);
}

void RigidbodyComponent::fixed_update()
{
	apply_deceleration();
	started_external_movement_this_frame = false;
	started_movement_this_frame = false;
}

#pragma region IData

Json::Value RigidbodyComponent::serialize_json()
{
	Json::Value obj;

	obj["max-velocity"] = max_velocity;
	obj["acceleration"] = acceleration;
	obj["deceleration"] = deceleration;
	obj["mass"] = mass;
	obj["orientation"] = Orientation::ToString(current_orientation);
	obj["body-type"] = body_type_to_string(body_type);
	obj["freeze-x"] = freeze_x;
	obj["freeze-y"] = freeze_y;

	return obj;
}

void RigidbodyComponent::unserialize_json(Json::Value obj)
{
	max_velocity = obj["max-velocity"].asFloat();
	acceleration = obj["acceleration"].asFloat();
	deceleration = obj["deceleration"].asFloat();
	mass = obj["mass"].asFloat();
	current_orientation = Orientation::ToDirection(obj["orientation"].asString());
	switch (current_orientation)
	{
		case bm98::Orientation::Direction::UP:
			facing_dir = Vector2f(0, -1);
			break;
		case bm98::Orientation::Direction::DOWN:
			facing_dir = Vector2f(0, 1);
			break;
		case bm98::Orientation::Direction::LEFT:
			facing_dir = Vector2f(-1, 0);
			break;
		case bm98::Orientation::Direction::RIGHT:
			facing_dir = Vector2f(1, 0);
			break;
	}
	body_type = string_to_body_type(obj["body-type"].asString());
	freeze_x = obj["freeze-x"].asBool();
	freeze_y = obj["freeze-y"].asBool();
}

#pragma endregion

std::vector<Editor::SerializedVar> RigidbodyComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;
	//variables = Component::get_editor_values();
	
	

	//variables.push_back(Editor::SerializedVar("This is a header", nullptr, Editor::VarType::Header));
	variables.push_back(Editor::SerializedVar("movement_direction", static_cast<void*>(&movement_dir), Var::Type::Vector2f));
	variables.push_back(Editor::SerializedVar("velocity", static_cast<void*>(&current_velocity), Var::Type::Float));
	variables.push_back(Editor::SerializedVar("external_velocity", static_cast<void*>(&external_velocity), Var::Type::Float));
	variables.push_back(Editor::SerializedVar("max_velocity", static_cast<void*>(&max_velocity),
		Var::Type::Float));
	variables.push_back(Editor::SerializedVar("acceleration", static_cast<void*>(&acceleration),
		Var::Type::Float));
	variables.push_back(Editor::SerializedVar("deceleration", static_cast<void*>(&deceleration)
		, Var::Type::Float));
	variables.push_back(Editor::SerializedVar("body_type", static_cast<void*>(&body_type),
		Var::Type::Dropdown, body_type_to_vector()));
	
	variables.push_back(Editor::SerializedVar("freeze_x", static_cast<void*>(&freeze_x),
						Var::Type::Bool));
	variables.push_back(Editor::SerializedVar("freeze_y", static_cast<void*>(&freeze_y),
						Var::Type::Bool));
	return variables;
}

void RigidbodyComponent::apply_external_force(Vector2f force, const bool ignore_mass)
{
	if (!active)
		return;
	if (external_velocity == 0)
		started_external_movement_this_frame = true;

	if (freeze_x)
		force.x = 0;
	if (freeze_y)
		force.y = 0;

	if (!ignore_mass && mass != 0)
	{
		force /= mass;
	}
	Vector2f new_ext_dir = external_dir;
	new_ext_dir *= external_velocity;

	new_ext_dir += (force.get_normalized() * force.magnitude());

	external_velocity = new_ext_dir.magnitude();
	external_dir = new_ext_dir.get_normalized();

}

void RigidbodyComponent::move_towards(const Vector2f dest)
{
	move(dest - game_object->get_physical_center());
}

void RigidbodyComponent::move(Vector2f dir)
{
	if (!active)
		return;
	if(dir != Vector2f::Zero())
		facing_dir = dir;

	if (freeze_x)
		dir.x = 0;
	if (freeze_y)
		dir.y = 0;

	if (current_velocity == 0)
		started_movement_this_frame;

	Vector2f new_dir = movement_dir;
	new_dir *= current_velocity;
	
	new_dir += (dir.get_normalized() * acceleration);
	//float new_velocity = new_dir.magnitude();
	current_velocity = std::min(new_dir.magnitude(), max_velocity);
	// More Fluid Movement:
	// movement_dir = new_dir.get_normalized();
	// More Snappy Movement
	movement_dir = dir.get_normalized();
}

void RigidbodyComponent::halt(Vector2f dir)
{
	return;
	std::cout << dir.to_string() << "\n";
	// TODO: Need to remove and velocity which was going into the direction cut off
	if ((dir.x > 0 && movement_dir.x > 0) || (dir.x < 0 && movement_dir.x < 0))
	{
		// reduce velocity
		float vel = current_velocity * movement_dir.x;
		current_velocity -= vel;
		movement_dir.x = 0;
	}
	if ((dir.y > 0 && facing_dir.y > 0) || (dir.y < 0 && facing_dir.y < 0))
	{
		// reduce velocity
		float vel = current_velocity * movement_dir.y;
		current_velocity -= vel;
		movement_dir.y = 0;
	}
}

void RigidbodyComponent::apply_deceleration()
{
	if (external_velocity > 0 && current_velocity > 0)
	{
		if(!started_external_movement_this_frame)
			external_velocity = std::max(0.f, external_velocity - (deceleration / 2));
		if(!started_movement_this_frame)
			current_velocity = std::max(0.f, current_velocity - (deceleration / 2));
	}
	else if (external_velocity > 0 && !started_external_movement_this_frame)
		external_velocity = std::max(0.f, external_velocity - deceleration);
	else if (current_velocity > 0 && !started_external_movement_this_frame)
		current_velocity = std::max(0.f, current_velocity - deceleration);

}

void RigidbodyComponent::update_orientation()
{
	if (facing_dir.x > 0.f)
	{
		current_orientation = Orientation::Direction::RIGHT;
		return;
	}
	if (facing_dir.x < 0.f)
	{
		current_orientation = Orientation::Direction::LEFT;
		return;
	}
	if (facing_dir.y > 0.f)
	{
		current_orientation = Orientation::Direction::DOWN;
		return;
	}
	if (facing_dir.y < 0.f)
	{
		current_orientation = Orientation::Direction::UP;
		return;
	}
}

void RigidbodyComponent::update_movement_state()
{
	if (current_velocity == 0)
		current_movement_state = MovementState::IDLE;
	else
		current_movement_state = MovementState::WALKING;
}

}