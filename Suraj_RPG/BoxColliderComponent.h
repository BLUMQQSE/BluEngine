#pragma once
namespace bm98
{
enum class CollisionDetection
{
	DISCRETE,
	CONTINUOUS
};

class BoxColliderComponent
{
public:

	BoxColliderComponent(sf::Sprite& sprite, float offset_x,
		float offset_y, float width, float height, bool trigger,
		CollisionDetection collision_check_type = CollisionDetection::DISCRETE);
	virtual ~BoxColliderComponent();

	bool check_intersect(const sf::FloatRect& frect);
	bool check_outer_intersect(const sf::FloatRect& frect);
	const sf::FloatRect get_bounds() const;
	const sf::FloatRect get_outer_bounds() const;
	const bool is_trigger() const;

	const bool is_active();
	void set_active(const bool& active);


	sf::Vector2f get_offset();

	void update();
	void fixed_update();
	void render(sf::RenderTarget& target);
	void add_to_buffer(sf::View* view = nullptr);

protected:


private:
	sf::Sprite& sprite;
	sf::RectangleShape hitbox;
	CollisionDetection collision_check_type;

	//sf::CircleShape circle;
	float offsetX, offsetY;
	bool trigger;
	bool active;

};
}
