#pragma once
#include "globals.h"
#include "IData.h"
#include "IObject.h"
namespace bm98
{
class AnimatedSpriteComponent;

class Tile : IData, IObject, public IRenderable
{
public:
	Tile(int x_offset, int y_offset, int grid_x, int grid_y,
		float grid_size_f, Sorting::Layer layer, PhysicsNS::Layer physical_layer);
	virtual ~Tile();


	virtual void update() override;
	virtual void late_update() override;
	virtual void fixed_update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	const int get_grid_x() { return grid_x; }
	const int get_grid_y() { return grid_y; }

	const bool is_static() const;
	const bool get_collision() const;
	FloatConvex get_collider();
	const PhysicsNS::Layer get_layer() const;
	const sf::FloatRect get_bounds() const;
	const AnimatedSpriteComponent* get_animated_sprite_component();
	sf::Sprite& get_sprite();

	/// <summary>
	/// Saves source file for texture, updates texture of tile with regard to rect
	///  location on texture sheet.
	/// </summary>
	void set_texture(std::string source_key, const sf::Texture* texture, const sf::IntRect rect);
	void set_collision(bool collision);
	void set_type(TileNS::Type tile_type);
	void set_position(Vector2i pos);

	void add_animated_sprite_component(std::string source_key, sf::Texture* texture,
		sf::IntRect animation_rect,
		float animation_timer);
	void remove_animated_sprite_component();

	void remove_texture();
	void highlight(bool highlight);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

protected:
	//sf::RectangleShape shape;
	sf::Sprite sprite;
	sf::IntRect texture_rect;

	int grid_x;
	int grid_y;

	int x_offset;
	int y_offset;

	FloatConvex collider;

	bool collision;
	bool static_tile;
	TileNS::Type type;
	Sorting::Layer layer;
	PhysicsNS::Layer physical_layer;
	std::string texture_source;
	AnimatedSpriteComponent* animated_sprite_component;
	
	float animation_timer;

};

}