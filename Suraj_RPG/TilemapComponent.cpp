#include "pch.h"
#include "TilemapComponent.h"
#include <filesystem>
#include "core/Debug.h"
#include "core/Renderer.h"
#include "AnimatedSpriteComponent.h"
#include "tiles/Tile.h"
#include "core/FileManager.h"
#include "GameObject.h"
#include "core/ResourceManager.h"
namespace bm98
{

std::map<std::string, sf::Texture> TilemapComponent::tile_sheets;
std::vector<std::string> TilemapComponent::tileset_keys;

using namespace core;
TilemapComponent::TilemapComponent()
{
	grid_size_u = UNIT_SIZE;

	grid_size_f = static_cast<float>(grid_size_u);
	this->max_size = Vector2i::Zero();

	this->layers = static_cast<int>(Sorting::Layer::UI);

	create_empty_map();

}

TilemapComponent::TilemapComponent(Vector2i position, float grid_size,
	unsigned width, unsigned height)
{
	this->position = position;
	this->grid_size_f = grid_size;
	this->grid_size_u = static_cast<unsigned>(this->grid_size_f);
	this->max_size.x = width + 1;
	this->max_size.y = height + 1;

	this->layers = static_cast<int>(Sorting::Layer::UI);
	create_empty_map();

}

TilemapComponent::~TilemapComponent()
{
	clear_map();
}

void TilemapComponent::init()
{
	EventSystem::Instance()->subscribe(EventID::ANIMATION_FRAME_CHANGE, this);
	
	current_tileset_key = tile_sheets.begin()->first;

	if (position.x == INFINITY)
	{
		position = sf::Vector2i(
			game_object->get_world_position().x, 
			game_object->get_world_position().y
		);
	}
	

	// Setting physical layers for each sorting layer to default
	for (int i = 0; i < static_cast<int>(Sorting::Layer::UI); i++)
	{
		physical_layers[(Sorting::Layer)i] = PhysicsNS::Layer::DEFAULT;
	}
	this->map_updateables.reserve(this->max_size.x * this->max_size.y * this->layers);



	update_tilemap_changes();

	this->outline = FloatConvex::Rectangle(
		Vector2f(position.x, position.y),
		Vector2f(max_size.x * UNIT_SIZE, max_size.y * UNIT_SIZE)
	);

	this->outline.setOutlineThickness(2.f);
	this->outline.setFillColor(sf::Color::Transparent);
	this->outline.setOutlineColor(sf::Color::White);

}

void TilemapComponent::awake()
{
	// Set rendering layer and z order of the outline

	set_sorting_layer(Sorting::Layer::UI);
	set_z_order(0);
	Renderer::Instance()->add(Renderer::RenderObject(&outline, this));
}

void TilemapComponent::update()
{
	updated_this_frame = false;
	for (auto& x : this->map_updateables)
	{
		x->update();
	}
}

void TilemapComponent::late_update()
{

}

// Soon to be obsolete
void TilemapComponent::add_to_buffer(sf::View* view)
{
	set_view(view);
}

void TilemapComponent::on_draw_gizmos()
{

}

void TilemapComponent::on_destroy()
{
	Renderer::Instance()->remove(&outline);
	for (int i = 0; i < render_sprites.size(); i++)
	{
		Renderer::Instance()->remove(&render_sprites[i]);
		if (render_textures[i])
			delete render_textures[i];
	}
	render_textures.clear();
	render_sprites.clear();
	renderable_info.clear();

	map_updateables.clear();

	clear_map();

}

float& TilemapComponent::grid_size()
{
	return grid_size_f;
}

sf::Texture* TilemapComponent::tile_sheet(std::string key)
{
	return &tile_sheets.at(key);
}

void TilemapComponent::add_tiles(const int x, const int y, Sorting::Layer layer, const sf::IntRect texture_rect,
	TileNS::Type tile_type, bool collision, bool animated_sprite, float animation_timer)
{
;	if (animated_sprite)
	{
		if (x < 0 || y < 0 || x >= max_size.x || y >= max_size.y)
			return;

		if (map[x][y][static_cast<int>(layer)])
			delete map[x][y][static_cast<int>(layer)];

		map[x][y][static_cast<int>(layer)] = new Tile(position.x, position.y, x, y, grid_size_f, layer, physical_layers[layer]);

		map[x][y][static_cast<int>(layer)]->add_animated_sprite_component(current_tileset_key, &tile_sheets.at(current_tileset_key), texture_rect, animation_timer);
		map[x][y][static_cast<int>(layer)]->set_collision(collision);
		map[x][y][static_cast<int>(layer)]->set_type(tile_type);

		update_tilemap_changes();
		return;
	}

	//if (x + static_cast<int>(texture_rect.width / 32) >= max_size.x ||
	//	y + static_cast<int>(texture_rect.height / 32) >= max_size.y)
	//	return;

	sf::IntRect sub_rect;
	sub_rect.width = grid_size_u;
	sub_rect.height = grid_size_u;

	int xx = x;
	int yy = y;
	for (int w = 0; w < texture_rect.width; w += grid_size_u)
	{
		sub_rect.left = w + texture_rect.left;

		if (xx < 0 || yy < 0 || xx >= max_size.x || yy >= max_size.y)
		{
			xx++;
			yy++;
			continue;
		}
		xx++;
		yy++;

		for (int h = 0; h < texture_rect.height; h += grid_size_u)
		{
			sub_rect.top = h + texture_rect.top;

			if (map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)])
				delete map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)];

			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)] =
				new Tile(position.x, position.y, x + static_cast<int>(w) / 32.f, y + static_cast<int>(h) / 32.f, grid_size_f, layer,
					physical_layers[layer]);

			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)]->set_texture(
				current_tileset_key, &tile_sheets.at(current_tileset_key), sub_rect);
			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)]->set_collision(collision);
			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)]->set_type(tile_type);
		}
	}
	update_tilemap_changes();
}

void TilemapComponent::remove_tiles(const int x, const int y, const Sorting::Layer layer,
	const sf::IntRect texture_rect, bool animated_sprite)
{
	if (animated_sprite)
	{
		if (x >= max_size.x || y >= max_size.y)
			return;

		if (map[x][y][static_cast<int>(layer)])
			delete map[x][y][static_cast<int>(layer)];

		update_tilemap_changes();
		return;
	}

	if (x + static_cast<int>(texture_rect.width / 32) >= max_size.x ||
		y + static_cast<int>(texture_rect.height / 32) >= max_size.y)
		return;

	for (int w = 0; w < texture_rect.width; w += grid_size_u)
	{
		for (int h = 0; h < texture_rect.height; h += grid_size_u)
		{
			if (map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)])
			{
				delete map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)];
				map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)] = nullptr;
			}
		}
	}
	update_tilemap_changes();
}

void TilemapComponent::highlight_layer(Sorting::Layer layer)
{
	editor_active_layer = layer;
}

PhysicsNS::Layer TilemapComponent::get_layer(Sorting::Layer layer)
{
	return physical_layers.at(layer);
}

sf::Texture* TilemapComponent::get_texture()
{
	return &tile_sheets.at(current_tileset_key);
}

void TilemapComponent::set_texture(std::string key)
{
	current_tileset_key = key;
}

void TilemapComponent::save_to_json(std::string file_path)
{
	FileManager::Instance()->save_to_file_styled(serialize_json(), file_path);
}

void TilemapComponent::load_from_json(std::string file_path)
{
	Json::Value obj = FileManager::Instance()->load_from_file(file_path);
	unserialize_json(obj);
}

void TilemapComponent::set_world_position(Vector2f pos)
{
	this->position = sf::Vector2i(std::floor(pos.x), std::floor(pos.y));
	outline.set_position(pos);

	for (auto& r_s : render_sprites)
	{
		r_s.setPosition(position.x, position.y);
	}
}

std::vector<std::vector<std::vector<Tile*>>> TilemapComponent::get_tiles()
{
	return map;
}

std::vector<Tile*> TilemapComponent::get_collidable_tiles()
{
	std::vector<Tile*> col_tiles;

	for (auto& c : map_collidables)
	{
		col_tiles.push_back(c);
	}

	return col_tiles;
}

void TilemapComponent::update_tilemap_changes()
{
	for (int i = 0; i < render_sprites.size(); i++)
	{
		Renderer::Instance()->remove(&render_sprites[i]);
		if (render_textures[i])
			delete render_textures[i];
	}
	render_textures.clear();
	render_sprites.clear();

	renderable_info.resize(this->layers);
	for (int i = 0; i < this->layers; i++)
	{
		renderable_info[i].set_render(true);
		renderable_info[i].set_render_depth(0);
		renderable_info[i].set_y_pos(position.y);
		renderable_info[i].set_sorting_layer(static_cast<Sorting::Layer>(i));
		renderable_info[i].set_z_order(-2);
		renderable_info[i].set_view(get_view());
	}

	for (int i = 0; i < this->layers; i++)
	{	
		render_textures.push_back(new sf::RenderTexture());

		render_textures[i]->create(grid_size_u * max_size.x, grid_size_u * max_size.y);

		for (size_t x = 0; x < this->max_size.x; x++)
		{
			for (size_t y = 0; y < this->max_size.y; y++)
			{

				if (this->map[x][y][i])
				{
					render_textures[i]->draw(this->map[x][y][i]->get_sprite());
				}
			}
		}

		render_textures[i]->display();
		sf::Sprite sprite(render_textures[i]->getTexture());
		sprite.setPosition(position.x, position.y);
		render_sprites.push_back(sprite);

	}

	// Set Render info of each render sprite
	// Z order is set negative to prevent gameobject on level 0 from passing under the layers
	for (int x = 0; x < render_sprites.size(); x++)
	{
		//set_z_order(-2, false);
		//Renderer::Instance()->add(Renderer::RenderObject(&render_sprites[x], get_render(), render_layers[x], get_z_order(), get_view_pointer()));
		Renderer::Instance()->add(Renderer::RenderObject(&render_sprites[x], &renderable_info[x]));
	}

	map_updateables.clear();

	for (auto& x : this->map)
	{
		for (auto& y : x)
		{
			for (auto& z : y)
			{
				if (z)
					if (z->get_animated_sprite_component())
						map_updateables.push_back(z);
			}
		}
	}

	updated_this_frame = true;
}

Json::Value TilemapComponent::serialize_json()
{
	Json::Value obj;

	obj["position.x"] = position.x;
	obj["position.y"] = position.y;
	obj["max-size.x"] = max_size.x;
	obj["max-size.y"] = max_size.y;
	obj["grid-size"] = grid_size_u;
	obj["file-path"] = file_path;

	for (auto& layer : physical_layers)
	{
		Json::Value obj2;
		obj2["layer"] = Sorting::ToString(layer.first);
		obj2["physical-layer"] = PhysicsNS::ToString(layer.second);
		obj["physical-layers"].append(obj2);
	}

	for (int x = 0; x < map.size(); x++)
		for (int y = 0; y < map[x].size(); y++)
			for (int z = 0; z < map[x][y].size(); z++)
			{
				if (map[x][y][z])
				{
					obj["tiles"].append(map[x][y][z]->serialize_json());
				}
			}

	return obj;
}

void TilemapComponent::unserialize_json(Json::Value obj)
{
	//load_tile_sheets();
	layers = ((int)Sorting::Layer::UI);
	position = sf::Vector2i(obj["position.x"].asInt64(), obj["position.y"].asInt64());

	max_size.x = obj["max-size.x"].asUInt64();
	max_size.y = obj["max-size.y"].asUInt64();
	grid_size_u = obj["grid-size"].asUInt64();
	file_path = obj["file-path"].asString();

	grid_size_f = static_cast<float>(grid_size_u);

	for (auto layer : obj["physical-layers"])
	{
		physical_layers[Sorting::ToLayer(layer["layer"].asString())] = PhysicsNS::ToLayer(layer["physical-layer"].asString());
	}

	this->map_updateables.clear();
	this->map_collidables.clear();

	// Reserve a good amount of space, but `n't need to save space for a pointer for every layers
	this->map_updateables.reserve(this->max_size.x * this->max_size.y);
	this->map_collidables.reserve(this->max_size.x * this->max_size.y);

	// Ensure any potential prior loaded map is removed
	clear_map();
	// Create empty map before loading in any tiles
	create_empty_map();

	for (Json::Value tile : obj["tiles"])
	{
		int x = tile["grid.x"].asInt64();
		int y = tile["grid.y"].asInt64();
		int z = static_cast<int>(Sorting::ToLayer(tile["layer"].asString()));
		sf::IntRect texture_rect;
		texture_rect.left = tile["texture-rect.left"].asInt64();
		texture_rect.width = tile["texture-rect.width"].asInt64();
		texture_rect.top = tile["texture-rect.top"].asInt64();
		texture_rect.height = tile["texture-rect.height"].asInt64();

		bool collision = tile["collision"].asBool();
		TileNS::Type type = TileNS::ToTiletype(tile["tile-type"].asString());
		std::string texture_source = tile["texture-source"].asString();
		float animation_timer = tile["animation-timer"].asFloat();

		map[x][y][z] = new Tile(position.x, position.y, x, y, grid_size_f, static_cast<Sorting::Layer>(z), physical_layers[(Sorting::Layer)z]);

		if (tile["animated-sprite"].asBool())
		{
			map[x][y][z]->add_animated_sprite_component(texture_source, &tile_sheets.at(texture_source),
				texture_rect, animation_timer);
			map[x][y][z]->unserialize_json(tile);
			map_updateables.push_back(map[x][y][z]);
		}
		else
			map[x][y][z]->set_texture(texture_source, &tile_sheets.at(texture_source), texture_rect);

		map[x][y][z]->set_collision(collision);
		if (collision)
			map_collidables.push_back(map[x][y][z]);

		map[x][y][z]->set_type(type);
	}

	update_tilemap_changes();
}

std::vector<Editor::SerializedVar> TilemapComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;
	values.push_back(Editor::SerializedVar("tilemap_size", &max_size, Var::Type::Vector2i));

	return values;
}

void TilemapComponent::editor_update()
{

	//======Safety Check For Editor==========================
	if (max_size.x == 0)
		max_size.x = 1;
	if (max_size.y == 0)
		max_size.y = 1;
	//======================================================
	if (max_size.x == map.size() && max_size.y == map[0].size())
	{
		return update_tilemap_changes();
	}
	if (position.x == INFINITY)
	{
		position = sf::Vector2i
		(
			game_object->get_world_position().x,
			game_object->get_world_position().y
		);
	}

	resize_map(max_size.x, max_size.y);


	update_tilemap_changes();

	
}

void TilemapComponent::set_physical_layer(Sorting::Layer layer, PhysicsNS::Layer physic)
{
	physical_layers[layer] = physic;
}

void TilemapComponent::LoadTileSheets()
{
	//tile_sheets.clear();
	
	//tileset_keys.clear();

	//int n = 0;
	for (const auto& entry : std::filesystem::directory_iterator(tiles_file_path))
	{
		std::string file_name = entry.path().string().substr(tiles_file_path.size(), entry.path().string().size() - 1);
		//if (n == 0)
			//current_tileset_key = file_name;
		//n++;

		if (!entry.is_directory())
		{
			if (!tile_sheets[file_name].loadFromFile(entry.path().string()))
			{
				Debug::Instance()->log("TILEMAP::FAILED TO LOAD TEXTURE", Debug::LogLevel::FAILURE);
			}
		}
	}

	for (auto f : tile_sheets)
		tileset_keys.push_back(f.first);

}

void TilemapComponent::clear_map()
{
	if (map.size() == 0 || map.size() == 0)
		return;
	for (size_t x = 0; x < this->max_size.x; x++)
	{
		for (size_t y = 0; y < this->max_size.y; y++)
		{
			for (size_t z = 0; z < this->layers; z++)
			{
				if (map[x][y][z])
					delete map[x][y][z];
				map[x][y][z] = nullptr;
			}
			map[x][y].clear();

		}
		map[x].clear();
	}
	map.clear();

}
void TilemapComponent::create_empty_map()
{
	this->map.resize(this->max_size.x);
	for (size_t x = 0; x < this->max_size.x; x++)
	{
		this->map[x] = std::vector< std::vector<Tile*>>();

		this->map[x].resize(this->max_size.y);
		for (size_t y = 0; y < this->max_size.y; y++)
		{
			this->map[x][y] = std::vector<Tile*>();
			this->map[x][y].resize(this->layers);
			for (size_t z = 0; z < this->layers; z++)
			{
				this->map[x][y][z] = nullptr;
			}
		}
	}
}

void TilemapComponent::resize_map(unsigned width, unsigned height)
{
	std::cout << "width: " << width << ", height: " << height << "\n";
	//empty array being initializes, so dont bother
	if (width == 0 || height == 0)
	{
		core::Debug::Instance()->log("Trying to set Tilemap to have a map of size zero", core::Debug::LogLevel::WARNING);
		return;
	}
	int x_last = this->map.size();
	int y_last = this->map[0].size();

	if (width > x_last)
	{
		this->map.resize(width);
		for (int x = x_last; x < this->map.size(); x++)
		{
			this->map[x] = std::vector< std::vector<Tile*>>();
			this->map[x].resize(this->map[0].size());
			
			for (int y = 0; y < this->map[x].size(); y++)
			{
				this->map[x][y] = std::vector<Tile*>();
				this->map[x][y].resize(this->layers);

				for (size_t z = 0; z < this->layers; z++)
				{
					this->map[x][y][z] = nullptr;
				}
			}
		}
	}
	else
	{
		for (int x = x_last-1; x >= width; x--)
		{
			for (int y = 0; y < map[x].size(); y++)
			{
				for (int z = 0; z < map[x][y].size(); z++)
				{
					if (map[x][y][z])
					{
						map_updateables.erase(std::remove(map_updateables.begin(), map_updateables.end(), map[x][y][z]), end(map_updateables));
						map_collidables.erase(std::remove(map_collidables.begin(), map_collidables.end(), map[x][y][z]), end(map_collidables));
						delete map[x][y][z];
					}
				}
				map[x][y].clear();
			}
			map[x].clear();
		}

		map.resize(width);

	}

	if (height > y_last)
	{
		for (int x = 0; x < this->map.size(); x++)
		{
			//this->map[x] = std::vector< std::vector<Tile*>>();
			this->map[x].resize(height);

			for (int y = y_last; y < height; y++)
			{
				this->map[x][y] = std::vector<Tile*>();
				this->map[x][y].resize(this->layers);

				for (size_t z = 0; z < this->layers; z++)
				{
					this->map[x][y][z] = nullptr;
				}
			}
		}
	}
	else
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = y_last-1; y >= height; y--)
			{
				for (int z = 0; z < map[x][y].size(); z++)
				{
					if (map[x][y][z])
					{
						map_updateables.erase(std::remove(map_updateables.begin(), map_updateables.end(), map[x][y][z]), end(map_updateables));
						map_collidables.erase(std::remove(map_collidables.begin(), map_collidables.end(), map[x][y][z]), end(map_collidables));
						delete map[x][y][z];
					}
				}
				map[x][y].clear();
			}
			map[x].resize(height);
		}


	}


	max_size = Vector2i(width, height);

	this->outline = FloatConvex::Rectangle(
		Vector2f(position.x, position.y),
		{ Vector2f(max_size.x * UNIT_SIZE, max_size.y * UNIT_SIZE) }
	);	
	this->outline.setOutlineThickness(2.f);
	this->outline.setFillColor(sf::Color::Transparent);
	this->outline.setOutlineColor(sf::Color::White);


	// Remove all map_updatables which are nullptr
	//map_updateables.erase(std::remove_if(map_updateables.begin(), map_updateables.end(),
	//	[](Tile* x) { return x == nullptr; }));
	
	// Remove all map_collidables which are nullptr
	//map_collidables.erase(std::remove_if(map_collidables.begin(), map_collidables.end(),
	//	[](Tile* x) { return x == nullptr; }));

}

void TilemapComponent::handle_event(Event* event)
{
	// Only want to update once every frame MAX
	if (updated_this_frame == true)
		return;

	switch (event->get_event_id())
	{
	case EventID::ANIMATION_FRAME_CHANGE:

		if (event->get_caller().name == Caller::Name::ANIMATED_SPRITE_COMPONENT)
		{
			update_tilemap_changes();
		}
		break;
	default:
		break;
	}

}

}