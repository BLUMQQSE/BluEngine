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

	virtual void update() override;
	virtual void fixed_update() override;

	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;

	//Vector2f get_velocity() { return velocity; }
	float get_velocity() { return current_velocity; }
	float get_external_velocity() { return external_velocity; }
	Vector2f get_movement_dir() { return movement_dir; }
	const float get_max_velocity() const { return max_velocity; }
	const BodyType get_body_type() const { return body_type; }
	const MovementState get_movement_state() const { return current_movement_state; }
	const Orientation::Direction get_orientation() const { return current_orientation; }

	void set_velocity(float velocity) { this->current_velocity = velocity; }
	void set_max_velocity(float velocity) { this->max_velocity = max_velocity; }
	void set_acceleration(float acceleration) { this->acceleration = acceleration; }
	void set_deceleration(float deceleration) { this->deceleration = deceleration; }
	void set_body_type(BodyType body_type) { this->body_type = body_type; }
	void set_orientation(Orientation::Direction dir) { this->current_orientation = dir; }

	void move(Vector2f dir);
	void move_towards(const Vector2f dest);
	 
	void apply_external_force(Vector2f force, const bool ignore_mass = false);

	

	void halt(Vector2f dir);

protected:
	void apply_deceleration();

	MovementState current_movement_state;
	Orientation::Direction current_orientation;
	BodyType body_type = BodyType::KINEMATIC;

private:
	float max_velocity;
	
	float current_velocity;
	Vector2f movement_dir;
	Vector2f facing_dir;

	float acceleration;
	float deceleration;
	float mass;

	bool freeze_x = false;
	bool freeze_y = false;

	float external_velocity;
	Vector2f external_dir;

	// ugly checks used to prevent decelerating right after accelerating
	bool started_movement_this_frame = false;
	bool started_external_movement_this_frame = false;

	/// <summary>
	/// Caps the velocity at max_velocity if it currently exceeds
	/// </summary>
	void cap_velocity();

	void update_orientation();
	void update_movement_state();


};
}
