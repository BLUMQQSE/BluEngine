#pragma once
#include "Component.h"
#include "Math.h"
namespace bm98
{

enum class MovementState
{
	IDLE,
	WALKING
};

class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent();
	RigidbodyComponent(sf::Sprite& sprite, float max_velocity,
		float acceleration, float deceleration);
	virtual ~RigidbodyComponent();

	virtual void init() override;
	virtual void update() override;
	virtual void fixed_update() override;

	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	const sf::Vector2f get_velocity() const;
	const float get_max_velocity() const;

	void set_velocity(sf::Vector2f velocity);
	void set_max_velocity(float velocity);
	void set_acceleration(float acceleration);
	void set_deceleration(float deceleration);

	void halt_right();
	void halt_left();
	void halt_up();
	void halt_down();
	void unhalt();
	//movement states

	const MovementState get_movement_state() const;
	const Orientation get_orientation() const;

	void apply_acceleration(const float dir_x, const float dir_y);

	

protected:
	void apply_deceleration();

	MovementState current_movement_state;
	Orientation current_orientation;

private:
	sf::Sprite* sprite;

	float max_velocity;
	float acceleration;
	float deceleration;

	Vector2f velocity;
	//std::vector<Contact> contacts;

	bool halted_up;
	bool halted_down;
	bool halted_left;
	bool halted_right;

	void update_orientation();
	void update_movement_state();
	void normalize_velocity();


};
}
