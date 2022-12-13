#pragma once
#include "Component.h"
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

	std::cout << "ERROR::BoxColliderComponent::string_to_collisiondetection\n";
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
		std::cout << "ERROR::BoxColliderComponent::collisiondetection_to_string\n";
		return "Discrete";
	}
}

static std::vector<std::string> collisiondetection_to_vector()
{
	return {"DISCRETE", "CONTINUOUS"};
}
#pragma endregion

class BoxColliderComponent : public Component
{
public:
	BoxColliderComponent();
	BoxColliderComponent(float offset_x,
		float offset_y, float width, float height, bool trigger,
		CollisionDetection collision_check_type = CollisionDetection::DISCRETE);
	virtual ~BoxColliderComponent();

	virtual void init() override;
	virtual void awake() override;
	virtual void update() override;
	virtual void fixed_update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	bool check_intersect(const sf::FloatRect& frect);
	bool check_outer_intersect(const sf::FloatRect& frect);
	const sf::FloatRect get_bounds() const;
	const sf::FloatRect get_outer_bounds() const;
	const bool is_trigger() const;
	const bool is_active();
	
	void set_active(const bool& active);
	void set_hitbox(float x, float y, float width, 
		float height, float offset_x, float offset_y);
	void set_collision_detection(CollisionDetection col);
	void set_trigger(bool trig);

	Vector2f get_offset();

	

protected:

private:
	//sf::Sprite* sprite;
	sf::RectangleShape hitbox;
	CollisionDetection collision_check_type;

	//sf::CircleShape circle;
	float width, height;
	float offsetX, offsetY;
	bool trigger;

	RigidbodyComponent* rb;

};
}
