#pragma once
#include "ColliderComponent.h"
namespace bm98
{

class BoxColliderComponent : public ColliderComponent
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
	
	
	
	void set_hitbox(float width, float height, float offset_x, float offset_y);
	void set_collision_detection(CollisionDetection col);
	void set_trigger(bool trig);

	Vector2f get_offset();

	

protected:

private:
	//sf::Sprite* sprite;
	sf::View* view = nullptr;

	//sf::CircleShape circle;
	float width, height;

};
}
