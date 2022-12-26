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

RigidbodyComponent::RigidbodyComponent(sf::Sprite& sprite, float max_velocity,
	float acceleration, float deceleration)
	:sprite(&sprite), max_velocity(max_velocity), acceleration(acceleration),
	deceleration(deceleration)
{
	unhalt();
}

RigidbodyComponent::~RigidbodyComponent()
{

}

void RigidbodyComponent::init()
{
	sprite = &game_object->get_component<SpriteComponent>().get_sprite();
	max_velocity = 200;
	acceleration = 15;
	deceleration = 50;
	unhalt();
}

void RigidbodyComponent::update()
{
	//update states

	update_movement_state();
	update_orientation();
	//Final movement
	sprite->move(velocity * Time::delta_time());
	//TODO: here we can update gameobject transform to reflect this position
	game_object->transform.position = sprite->getPosition();
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

	return obj;
}

void RigidbodyComponent::unserialize_json(Json::Value obj)
{
	max_velocity = obj["max-velocity"].asFloat();
	acceleration = obj["acceleration"].asFloat();
	deceleration = obj["deceleration"].asFloat();
	current_orientation = Global::string_to_orientation(obj["orientation"].asString());
}

#pragma endregion


const sf::Vector2f RigidbodyComponent::get_velocity() const
{
	return velocity;
}

const float RigidbodyComponent::get_max_velocity() const
{
	return max_velocity;
}

void RigidbodyComponent::set_velocity(sf::Vector2f velocity)
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

void RigidbodyComponent::halt_right()
{
	halted_right = true;
}

void RigidbodyComponent::halt_left()
{
	halted_left = true;
}

void RigidbodyComponent::halt_up()
{
	halted_up = true;
}

void RigidbodyComponent::halt_down()
{
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
		velocity.x += acceleration * dir_x * 100;
	if ((!halted_down && dir_y > 0) || (!halted_up && dir_y < 0))
		velocity.y += acceleration * dir_y * 100;

	if(velocity.x > 0)
		if (velocity.x > max_velocity)
			velocity.x = max_velocity;
	if(velocity.x < 0)
		if (velocity.x < -max_velocity)
			velocity.x = -max_velocity;
	
	if(velocity.y > 0)
		if (velocity.y > max_velocity)
			velocity.y = max_velocity; 
	if(velocity.y < 0)
		if (velocity.y < -max_velocity)
			velocity.y = -max_velocity;
}

void RigidbodyComponent::apply_deceleration()
{
	if (velocity.x > 0.f)
	{
		velocity.x -= deceleration;
		if (velocity.x < 0.f)
			velocity.x = 0.f;

	}
	else if (velocity.x < 0.f)
	{
		velocity.x += deceleration;
		if (velocity.x > 0.f)
			velocity.x = 0.f;

	}
	if (velocity.y > 0.f)
	{
		velocity.y -= deceleration;
		if (velocity.y < 0.f)
			velocity.y = 0.f;

	}
	else if (velocity.y < 0.f)
	{
		velocity.y += deceleration;
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
	}
	else if (velocity.x < 0.f && velocity.y == 0.f)
	{
		current_orientation = Orientation::LEFT;
	}
	else if (velocity.y > 0.f)
	{
		current_orientation = Orientation::DOWN;
	}
	else
	{
		current_orientation = Orientation::UP;
	}
}

void RigidbodyComponent::update_movement_state()
{
	if (velocity.x == 0.f && velocity.y == 0.f)
		current_movement_state = MovementState::IDLE;
	else
		current_movement_state = MovementState::WALKING;
}

void RigidbodyComponent::normalize_velocity()
{
	// Currently does normalize the velocity,
	// However causing weird movement when decelerating
	// (decelerates in diagonal direction then continues straight)
	float v1 = velocity.x;
	float v2 = velocity.y;

	if (velocity.x != 0 && velocity.y != 0)
	{
		velocity.x = velocity.x / std::sqrt(std::pow(velocity.x, 2) +
			std::pow(velocity.y, 2));
		velocity.x *= std::abs(v1);
		velocity.y = velocity.y / std::sqrt(std::pow(velocity.x, 2) +
			std::pow(velocity.y, 2));
		velocity.y *= std::abs(v2);
	}
}

}