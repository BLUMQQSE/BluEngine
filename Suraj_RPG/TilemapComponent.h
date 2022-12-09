#pragma once
#include "globals.h"

#include "Component.h"
namespace bm98
{
class Tile;

static const std::string tiles_file_path = "Resources/Images/Tiles/";

class TilemapComponent : public Component
{

public:
	TilemapComponent();
	TilemapComponent(int position_x, int position_y, float grid_size,
		unsigned width, unsigned height);
	virtual ~TilemapComponent();

	float& grid_size();
	sf::Texture* tile_sheet(std::string key);

	virtual void update() override;
	virtual void late_update() override;
	virtual void fixed_update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	void add_tiles(const unsigned x, const unsigned y, SortingLayer layer,
		const sf::IntRect texture_rect, TileType tile_type = TileType::DEFAULT,
		bool collision = false, bool animated_sprite = false, float animation_timer = 30.f);
	void remove_tiles(const unsigned x, const unsigned y, const SortingLayer layer,
		const sf::IntRect texture_rect, bool animated_sprite = false);

	void highlight_layer(SortingLayer layer);

	std::vector<std::string> get_tileset_keys();
	sf::Texture* get_texture();
	void set_texture(std::string key);

	void save_to_json(std::string file_path);
	void load_from_json(std::string file_path);
	/// <summary>
	/// Sets origin of tilemap.
	/// </summary>
	void set_position(sf::Vector2f pos);


	std::vector<Tile*> get_collidable_tiles();

	// Inherited via IData
	virtual Json::Value serialize_json() override;

	virtual void unserialize_json(Json::Value obj) override;

private:
	sf::Vector2i position;
	float grid_size_f;
	unsigned grid_size_u;
	unsigned int layers;
	sf::Vector2u max_size;
	sf::RectangleShape outline;
	std::vector<std::vector<std::vector<Tile*> > > map;
	std::vector<Tile*> map_renderables;
	std::vector<Tile*> map_updateables;
	// tile sheet will become a map
	// can hold different texture sheets for choosing stuff
	//sf::Texture* tile_sheet;

	std::map<std::string, sf::Texture> tile_sheets;
	std::string current_tileset_key;
	std::vector<std::string> tileset_keys;

	/*
	may be better to use vector<sf::Texture> tile_sheets;
	and sf::Texture* current_tile_sheet
	//this is an issue for saving and loading a map link for string

	but how can i save a reference to each loaded file
	*/

	SortingLayer editor_active_layer;

	void update_tilemap_changes();
	void load_tile_sheets();
	void clear();

};

}