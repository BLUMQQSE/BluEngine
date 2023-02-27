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

#pragma region BodyType
	enum class BodyType
	{
		KINEMATIC,
		DYNAMIC,
		STATIC
	};

	static std::vector<std::string> body_type_to_vector()
	{
		return { "KINEMATIC", "DYNAMIC", "STATIC"};
	}

	static std::string body_type_to_string(BodyType type)
	{
		switch (type)
		{
		case BodyType::KINEMATIC:
			return "KINEMATIC";
		case BodyType::DYNAMIC:
			return "DYNAMIC";
		case BodyType::STATIC:
			return "STATIC";
		default:
			std::cout << "ERROR::RIGIDBODY::body_type_to_string::UNDEFINED BODYTYPE: " << static_cast<int>(type) << "\n";
			return "null";
		}
	}

	static BodyType string_to_body_type(std::string type)
	{
		if (type == "KINEMATIC")
			return BodyType::KINEMATIC;
		if (type == "DYNAMIC")
			return BodyType::DYNAMIC;
		if (type == "STATIC")
			return BodyType::STATIC;

		std::cout << "ERROR::RIGIDBODY::string_to_body_type::UNDEFINED STRING: " << type << "\n";
		return BodyType::KINEMATIC;
	}

#pragma endregion

	RigidbodyComponent();
	RigidbodyComponent(sf::Sprite& sprite, float max_velocity,
		float acceleration, float deceleration);
	virtual ~RigidbodyComponent();

	virtual void init() override;
	virtual void update() override;
	virtual void fixed_update() override;

	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	const Vector2f get_velocity() const;
	const float get_max_velocity() const;
	const BodyType get_body_type() const;

	void set_velocity(Vector2f velocity);
	void set_max_velocity(float velocity);
	void set_acceleration(float acceleration);
	void set_deceleration(float deceleration);
	void set_body_type(BodyType body_type);

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
	BodyType body_type = BodyType::KINEMATIC;

private:
	sf::Sprite* sprite;

	float max_velocity;
	float acceleration;
	float deceleration;

	bool halted_up = false;
	bool halted_down = false;
	bool halted_left = false;
	bool halted_right = false;

	Vector2f velocity;

	void update_orientation();
	void update_movement_state();


};
}
