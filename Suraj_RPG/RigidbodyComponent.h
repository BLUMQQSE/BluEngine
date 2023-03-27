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
	RigidbodyComponent(float max_velocity,
		float acceleration, float deceleration);
	virtual ~RigidbodyComponent();

	virtual void init() override;
	virtual void update() override;
	virtual void fixed_update() override;

	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;

	Vector2f get_velocity();
	const float get_max_velocity() const;
	const BodyType get_body_type() const;

	void set_velocity(Vector2f velocity);
	void set_max_velocity(float velocity);
	void set_acceleration(float acceleration);
	void set_deceleration(float deceleration);
	void set_body_type(BodyType body_type);

	//movement states

	const MovementState get_movement_state() const;
	const Orientation::Direction get_orientation() const;

	void set_orientation(Orientation::Direction dir);

	void apply_acceleration(const Vector2f dir, const bool ignore_max_velocty = false);
	void apply_velocity(const Vector2f velocity, const bool ignore_max_velocity = false);
	void move_towards(const Vector2f dest);

	void halt(Vector2f dir);

protected:
	void apply_deceleration();

	MovementState current_movement_state;
	Orientation::Direction current_orientation;
	BodyType body_type = BodyType::KINEMATIC;

private:
	float max_velocity;
	float acceleration;
	float deceleration;

	Vector2f velocity;

	/// <summary>
	/// Caps the velocity at max_velocity if it currently exceeds
	/// </summary>
	void cap_velocity();

	void update_orientation();
	void update_movement_state();


};
}
