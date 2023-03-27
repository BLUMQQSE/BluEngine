#pragma once
#include "globals.h"
#include "Math.h"
#include "Component.h"
#include "IRenderable.h"
namespace bm98
{
class Tile;
class TlemapColliderComponent;

static const std::string tiles_file_path = "Resources/Images/Tiles/";

class TilemapComponent : public Component, public IRenderable
{

public:
	TilemapComponent();
	TilemapComponent(int position_x, int position_y, float grid_size,
		unsigned width, unsigned height);
	virtual ~TilemapComponent();

	float& grid_size();
	sf::Texture* tile_sheet(std::string key);

	virtual void init() override;
	virtual void update() override;
	virtual void late_update() override;
	virtual void fixed_update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	void add_tiles(const unsigned x, const unsigned y, Sorting::Layer layer,
		const sf::IntRect texture_rect, TileNS::Type tile_type = TileNS::Type::DEFAULT,
		bool collision = false, bool animated_sprite = false, float animation_timer = 30.f);
	void remove_tiles(const unsigned x, const unsigned y, const Sorting::Layer layer,
		const sf::IntRect texture_rect, bool animated_sprite = false);

	void highlight_layer(Sorting::Layer layer);
	/// <summary>
	/// Returns physical layer for layer.
	/// </summary>
	PhysicsNS::Layer get_layer(Sorting::Layer layer);

	std::vector<std::string> get_tileset_keys();
	sf::Texture* get_texture();
	void set_texture(std::string key);

	void save_to_json(std::string file_path);
	void load_from_json(std::string file_path = "");
	/// <summary>
	/// Sets origin of tilemap.
	/// </summary>
	void set_position(Vector2i pos);

	std::vector<std::vector<std::vector<Tile*> > > get_tiles();
	std::vector<Tile*> get_collidable_tiles();

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

private:
	Vector2i position;
	float grid_size_f;
	unsigned grid_size_u;
	unsigned int layers;
	sf::Vector2u max_size;
	FloatConvex outline;
	std::vector<std::vector<std::vector<Tile*> > > map;
	std::vector<Tile*> map_renderables;

	std::vector<sf::Sprite> render_sprites;
	std::vector<sf::RenderTexture*> render_textures;
	std::vector<Sorting::Layer> render_layers;
	std::unordered_map<int, PhysicsNS::Layer> physical_layers;

	std::vector<Tile*> map_updateables;

	std::string file_path = "";
	
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

	Sorting::Layer editor_active_layer;

	void update_tilemap_changes();
	void load_tile_sheets();
	
	void clear_map();
	void create_empty_map();


	//TilemapColliderComponent* tilemap_collider;
};

}