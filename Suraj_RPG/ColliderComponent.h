#pragma once
#include "pch.h"
#include "Component.h"
#include "IRenderable.h"
#include "Math.h"
namespace bm98
{

class RigidbodyComponent;

#pragma region CollisionDetection

enum class CollisionDetection
{
	DISCRETE,
	CONTINUOUS
};

static CollisionDetection string_to_collisiondetection(std::string col)
{
	if (col == "DISCRETE")
		return CollisionDetection::DISCRETE;
	if (col == "CONTINUOUS")
		return CollisionDetection::CONTINUOUS;

	std::cout << "ERROR::Collider::string_to_collisiondetection\n";
	return CollisionDetection::DISCRETE;
}

static std::string collisiondetection_to_string(CollisionDetection col)
{
	switch (col)
	{
	case bm98::CollisionDetection::DISCRETE:
		return "DISCRETE";
	case bm98::CollisionDetection::CONTINUOUS:
		return "CONINUOUS";
	default:
		std::cout << "ERROR::Collider::collisiondetection_to_string\n";
		return "Discrete";
	}
}

static std::vector<std::string> collisiondetection_to_vector()
{
	return { "DISCRETE", "CONTINUOUS" };
}
#pragma endregion


class ColliderComponent :
    public Component , public IRenderable
{
public:
	ColliderComponent();
	virtual ~ColliderComponent();

	virtual bool intersects(const FloatConvex collider);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;

	virtual void set_active(bool active) override;

	FloatConvex get_collider_bounds();
	const bool is_trigger() const;
	const bool is_active();

protected:
	FloatConvex collider_bounds;

	bool trigger;
	float offsetX, offsetY;

	CollisionDetection collision_check_type;
	RigidbodyComponent* rb;
};

}