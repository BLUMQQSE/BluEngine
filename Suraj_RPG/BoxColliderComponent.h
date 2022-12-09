#pragma once
#include "Component.h"
#include "Math.h"
namespace bm98
{

class RigidbodyComponent;

enum class CollisionDetection
{
	DISCRETE,
	CONTINUOUS
};

class BoxColliderComponent : public Component
{
public:
	BoxColliderComponent();
	BoxColliderComponent(sf::Sprite& sprite, float offset_x,
		float offset_y, float width, float height, bool trigger,
		CollisionDetection collision_check_type = CollisionDetection::DISCRETE);
	virtual ~BoxColliderComponent();

	virtual void init() override;
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


	Vector2f get_offset();

	

protected:

private:
	sf::Sprite* sprite;
	sf::RectangleShape hitbox;
	CollisionDetection collision_check_type;

	//sf::CircleShape circle;
	float width, height;
	float offsetX, offsetY;
	bool trigger;

	RigidbodyComponent* rb;

};
}
