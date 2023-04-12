#pragma once
#include "EventSystem.h"
#include "globals.h"
#include "Math.h"
#include "Component.h"
#include "IRenderable.h"
namespace bm98
{
class Tile;
class TlemapColliderComponent;

static const std::string tiles_file_path = "Resources/Images/Tiles/";

using namespace core;
class TilemapComponent : public Component, public IRenderable,
	public Listener
{
public:
	TilemapComponent();
	TilemapComponent(Vector2i position, float grid_size,
		unsigned width, unsigned height);
	virtual ~TilemapComponent();

	virtual void init() override;
	virtual void awake() override;
	virtual void update() override;
	virtual void late_update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	virtual void on_destroy() override;

	/// <returns>Size of each Tile within the map.</returns>
	float& grid_size();
	sf::Texture* tile_sheet(std::string key);

	/// <summary>
	/// Adds all tiles currently selected to the map. If part of selection is not within
	/// map bounds, those tiles will not be added.
	/// </summary>
	void add_tiles(const int x, const int y, Sorting::Layer layer,
		const sf::IntRect texture_rect, TileNS::Type tile_type = TileNS::Type::DEFAULT,
		bool collision = false, bool animated_sprite = false, float animation_timer = 300.0f);
	/// <summary> Removes all tiles currently selected from the map. </summary>
	void remove_tiles(const int x, const int y, const Sorting::Layer layer,
		const sf::IntRect texture_rect, bool animated_sprite = false);

	void highlight_layer(Sorting::Layer layer);
	/// <summary> Returns physical layer for layer. </summary>
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

	/// <summary> Returns all Tiles in the map. </summary>
	std::vector<std::vector<std::vector<Tile*> > > get_tiles();
	/// <summary> Returns all Tiles with a collider attached. </summary>
	std::vector<Tile*> get_collidable_tiles();

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;

private:
	Vector2i position;
	float grid_size_f;
	unsigned grid_size_u;
	unsigned int layers;
	Vector2i max_size = Vector2i(1, 1);
	FloatConvex outline;
	std::vector<std::vector<std::vector<Tile*> > > map;

	bool updated_this_frame = false;
	// All Tiles which need to be updated
	std::vector<Tile*> map_updateables;
	// All Tiles which possess a collider
	std::vector<Tile*> map_collidables;

	std::vector<sf::Sprite> render_sprites;
	std::vector<sf::RenderTexture*> render_textures;
	std::vector<Sorting::Layer> render_layers;
	std::unordered_map<int, PhysicsNS::Layer> physical_layers;


	std::string file_path = "";
	
	std::map<std::string, sf::Texture> tile_sheets;
	std::string current_tileset_key;
	std::vector<std::string> tileset_keys;


	Sorting::Layer editor_active_layer;

	/// <summary>
	/// Updates textures of tilemap. Called after any alterations occur to the tilemap.
	/// </summary>
	void update_tilemap_changes();
	/// <summary>
	/// Loads tile sheets used by the tilemap from tiles_file_path and loads them into
	/// the tile_sheets map.
	/// </summary>
	void load_tile_sheets();
	
	/// <summary> Removes all Tiles from tilemap. </summary>
	void clear_map();
	/// <summary> Creates empty map of Tile* of max_size.x * max_size.y * layers. </summary>
	void create_empty_map();

	/// <summary> Adjusts size of tilemap and handles any deletion of Tiles necessary. </summary>
	void resize_map(unsigned width, unsigned height);


	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}