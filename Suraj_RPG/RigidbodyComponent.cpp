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
		UNIT_SIZE * Time::delta_time();
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
	obj["orientation"] = Global::orientation_to_string(current_orientation);
	obj["body-type"] = body_type_to_string(body_type);

	return obj;
}

void RigidbodyComponent::unserialize_json(Json::Value obj)
{
	max_velocity = obj["max-velocity"].asFloat();
	acceleration = obj["acceleration"].asFloat();
	deceleration = obj["deceleration"].asFloat();
	current_orientation = Global::string_to_orientation(obj["orientation"].asString());
	body_type = string_to_body_type(obj["body-type"].asString());
}

#pragma endregion

std::vector<Editor::SerializedVar> RigidbodyComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> variables;
	
	//variables.push_back(Editor::SerializedVar("This is a header", nullptr, Editor::VarType::Header));

	variables.push_back(Editor::SerializedVar("velocity", static_cast<void*>(&velocity), Editor::VarType::Vector2f));
	variables.push_back(Editor::SerializedVar("max_velocity", static_cast<void*>(&max_velocity),
		Editor::VarType::Float));
	variables.push_back(Editor::SerializedVar("acceleration", static_cast<void*>(&acceleration),
		Editor::VarType::Float));
	variables.push_back(Editor::SerializedVar("deceleration", static_cast<void*>(&deceleration)
		, Editor::VarType::Float));
	variables.push_back(Editor::SerializedVar("body_type", static_cast<void*>(&body_type),
		Editor::VarType::Dropdown, body_type_to_vector()));

	return variables;
}


const Vector2f RigidbodyComponent::get_velocity() const
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

void RigidbodyComponent::halt_right()
{
	velocity.x = 0.f;
	halted_right = true;
}

void RigidbodyComponent::halt_left()
{
	velocity.x = 0.f;
	halted_left = true;
}

void RigidbodyComponent::halt_up()
{
	velocity.y = 0.f;
	halted_up = true;
}

void RigidbodyComponent::halt_down()
{
	velocity.y = 0.f;
	halted_down = true;
}

void RigidbodyComponent::unhalt()
{
	halted_down = false;
	halted_up = false;
	halted_left = false;
	halted_right = false;
}

const MovementState RigidbodyComponent::get_movement_state() const
{
	return current_movement_state;
}

const Orientation RigidbodyComponent::get_orientation() const
{
	return current_orientation;
}

void RigidbodyComponent::apply_acceleration(const float dir_x, const float dir_y)
{
	//Acceleration
	if ((!halted_right && dir_x > 0) || (!halted_left && dir_x < 0))
		velocity.x += (acceleration * UNIT_SIZE) * dir_x * 100;
	if ((!halted_down && dir_y > 0) || (!halted_up && dir_y < 0))
		velocity.y += (acceleration * UNIT_SIZE) * dir_y * 100;

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

void RigidbodyComponent::update_orientation()
{
	//if no movement occurred, no orientation change
	if (velocity.x == 0.f && velocity.y == 0.f)
		return;

	if (velocity.x > 0.f && velocity.y == 0.f)
	{
		current_orientation = Orientation::RIGHT;
		return;
	}
	if (velocity.x < 0.f && velocity.y == 0.f)
	{
		current_orientation = Orientation::LEFT;
		return;
	}
	if (velocity.y > 0.f)
	{
		current_orientation = Orientation::DOWN;
		return;
	}
	current_orientation = Orientation::UP;
	
}

void RigidbodyComponent::update_movement_state()
{
	if (velocity.x == 0.f && velocity.y == 0.f)
		current_movement_state = MovementState::IDLE;
	else
		current_movement_state = MovementState::WALKING;
}

}