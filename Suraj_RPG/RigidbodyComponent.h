#pragma once
namespace bm98
{
enum class MovementState
{
	IDLE,
	WALKING
};

enum class Orientation
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class RigidbodyComponent
{
public:
	RigidbodyComponent(sf::Sprite& sprite, float max_velocity,
		float acceleration, float deceleration);
	virtual ~RigidbodyComponent();

	const sf::Vector2f get_velocity() const;
	const float get_max_velocity() const;

	void set_velocity(sf::Vector2f velocity);
	void halt_right();
	void halt_left();
	void halt_up();
	void halt_down();
	void unhalt();
	//movement states

	const MovementState get_movement_state() const;
	const Orientation get_orientation() const;

	void apply_acceleration(const float dir_x, const float dir_y);
	void update();
	void fixed_update();

protected:
	void apply_deceleration();

	MovementState current_movement_state;
	Orientation current_orientation;

private:
	sf::Sprite& sprite;

	float max_velocity;
	float acceleration;
	float deceleration;

	sf::Vector2f velocity;
	sf::Vector2f position;
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
