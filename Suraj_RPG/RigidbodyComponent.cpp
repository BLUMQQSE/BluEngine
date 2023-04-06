#include "pch.h"
#include "RigidbodyComponent.h"
#include "Time.h"
#include "GameObject.h"
#include "SpriteComponent.h"
namespace bm98
{
using namespace core;

RigidbodyComponent::RigidbodyComponent()
{
}

RigidbodyComponent::RigidbodyComponent(float max_velocity,
	float acceleration, float deceleration)
	:max_velocity(max_velocity), acceleration(acceleration),
	deceleration(deceleration)
{

}

RigidbodyComponent::~RigidbodyComponent()
{

}

void RigidbodyComponent::init()
{

}

void RigidbodyComponent::update()
{
	//update states

	update_movement_state();
	update_orientation();
	// this could be converted to:
	Vector2f translation = velocity.get_normalized() * max_velocity * 
		UNIT_SIZE * Time::Instance()->delta_time();
	game_object->move(translation);
	
}

void RigidbodyComponent::fixed_update()
{
	apply_deceleration();
}
#pragma region IData

Json::Value RigidbodyComponent::serialize_json()
{
	Json::Value obj;

	obj["max-velocity"] = max_velocity;
	obj["acceleration"] = acceleration;
	obj["deceleration"] = deceleration;
	obj["orientation"] = Orientation::ToString(current_orientation);
	obj["body-type"] = body_type_to_string(body_type);

	return obj;
}

void RigidbodyComponent::unserialize_json(Json::Value obj)
{
	max_velocity = obj["max-velocity"].asFloat();
	acceleration = obj["acceleration"].asFloat();
	deceleration = obj["deceleration"].asFloat();
	current_orientation = Orientation::ToDirection(obj["orientation"].asString());
	body_type = string_to_body_type(obj["body-type"].asString());
}

#pragma endregion

std::vector<Editor::SerializedVar> RigidbodyComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;
	
	//variables.push_back(Editor::SerializedVar("This is a header", nullptr, Editor::VarType::Header));

	variables.push_back(Editor::SerializedVar("velocity", static_cast<void*>(&velocity), Var::Type::Vector2f));
	variables.push_back(Editor::SerializedVar("max_velocity", static_cast<void*>(&max_velocity),
		Var::Type::Float));
	variables.push_back(Editor::SerializedVar("acceleration", static_cast<void*>(&acceleration),
		Var::Type::Float));
	variables.push_back(Editor::SerializedVar("deceleration", static_cast<void*>(&deceleration)
		, Var::Type::Float));
	variables.push_back(Editor::SerializedVar("body_type", static_cast<void*>(&body_type),
		Var::Type::Dropdown, body_type_to_vector()));

	return variables;
}


Vector2f RigidbodyComponent::get_velocity()
{
	return velocity;
}

const float RigidbodyComponent::get_max_velocity() const
{
	return max_velocity;
}

const RigidbodyComponent::BodyType RigidbodyComponent::get_body_type() const
{
	return body_type;
}

void RigidbodyComponent::set_velocity(Vector2f velocity)
{
	this->velocity = velocity;
}

void RigidbodyComponent::set_max_velocity(float velocity)
{
	this->max_velocity = velocity;
}

void RigidbodyComponent::set_acceleration(float acceleration)
{
	this->acceleration = acceleration;
}

void RigidbodyComponent::set_deceleration(float deceleration)
{
	this->deceleration = deceleration;
}

void RigidbodyComponent::set_body_type(BodyType body_type)
{
	this->body_type = body_type;
}

const MovementState RigidbodyComponent::get_movement_state() const
{
	return current_movement_state;
}

const Orientation::Direction RigidbodyComponent::get_orientation() const
{
	return current_orientation;
}

void RigidbodyComponent::set_orientation(Orientation::Direction dir)
{
	current_orientation = dir;
}

void RigidbodyComponent::apply_acceleration(const Vector2f dir, const bool ignore_max_velocty)
{
	//Acceleration
	velocity.x += (acceleration * UNIT_SIZE) * dir.x * 100;
	velocity.y += (acceleration * UNIT_SIZE) * dir.y * 100;

	if (ignore_max_velocty)
		return;

	cap_velocity();
}

void RigidbodyComponent::apply_velocity(const Vector2f velocity, const bool ignore_max_velocity)
{
	this->velocity += velocity;

	if (ignore_max_velocity)
		return;
	
	cap_velocity();
}

void RigidbodyComponent::move_towards(const Vector2f dest)
{
	apply_acceleration(dest - game_object->get_world_position());
}

void RigidbodyComponent::halt(Vector2f dir)
{
	if ((dir.x > 0 && velocity.x > 0) || dir.x < 0 && velocity.x < 0)
		velocity.x = 0;
	if ((dir.y > 0 && velocity.y > 0) || dir.y < 0 && velocity.y < 0)
		velocity.y = 0;

}

void RigidbodyComponent::apply_deceleration()
{
	if (velocity.x > 0.f)
	{
		velocity.x -= deceleration * UNIT_SIZE;
		if (velocity.x < 0.f)
			velocity.x = 0.f;

	}
	else if (velocity.x < 0.f)
	{
		velocity.x += deceleration * UNIT_SIZE;
		if (velocity.x > 0.f)
			velocity.x = 0.f;

	}
	if (velocity.y > 0.f)
	{
		velocity.y -= deceleration * UNIT_SIZE;
		if (velocity.y < 0.f)
			velocity.y = 0.f;

	}
	else if (velocity.y < 0.f)
	{
		velocity.y += deceleration * UNIT_SIZE;
		if (velocity.y > 0.f)
			velocity.y = 0.f;
	}

}

void RigidbodyComponent::cap_velocity()
{
	if (velocity.x > 0)
		if (velocity.x > max_velocity * UNIT_SIZE)
			velocity.x = max_velocity * UNIT_SIZE;
	if (velocity.x < 0)
		if (velocity.x < -max_velocity * UNIT_SIZE)
			velocity.x = -max_velocity * UNIT_SIZE;

	if (velocity.y > 0)
		if (velocity.y > max_velocity * UNIT_SIZE)
			velocity.y = max_velocity * UNIT_SIZE;
	if (velocity.y < 0)
		if (velocity.y < -max_velocity * UNIT_SIZE)
			velocity.y = -max_velocity * UNIT_SIZE;
}

void RigidbodyComponent::update_orientation()
{
	//if no movement occurred, no orientation change
	if (velocity.x == 0.f && velocity.y == 0.f)
		return;

	if (velocity.x > 0.f && velocity.y == 0.f)
	{
		current_orientation = Orientation::Direction::RIGHT;
		return;
	}
	if (velocity.x < 0.f && velocity.y == 0.f)
	{
		current_orientation = Orientation::Direction::LEFT;
		return;
	}
	if (velocity.y > 0.f)
	{
		current_orientation = Orientation::Direction::DOWN;
		return;
	}
	current_orientation = Orientation::Direction::UP;
	
}

void RigidbodyComponent::update_movement_state()
{
	if (velocity.x == 0.f && velocity.y == 0.f)
		current_movement_state = MovementState::IDLE;
	else
		current_movement_state = MovementState::WALKING;
}

}