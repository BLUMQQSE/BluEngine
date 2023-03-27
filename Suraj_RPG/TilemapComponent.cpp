#include "pch.h"
#include "TilemapComponent.h"
#include <filesystem>
#include "Debug.h"
#include "Renderer.h"
#include "Tile.h"
#include "FileManager.h"
#include "GameObject.h"
#include "ResourceManager.h"
namespace bm98
{
using namespace core;
TilemapComponent::TilemapComponent()
{

}

TilemapComponent::TilemapComponent(int position_x, int position_y, float grid_size,
	unsigned width, unsigned height)
{
	this->position = Vector2i(position_x, position_y);
	this->grid_size_f = grid_size;
	this->grid_size_u = static_cast<unsigned>(this->grid_size_f);
	this->max_size.x = width + 1;
	this->max_size.y = height + 1;
	load_tile_sheets();
	this->layers = static_cast<int>(Sorting::Layer::UI);
	for (int i = 0; i < static_cast<int>(Sorting::Layer::UI); i++)
	{
		physical_layers[i] = PhysicsNS::Layer::DEFAULT;
	}
	outline = FloatConvex::Polygon(Vector2f(0,0),
	{Vector2f(0,0), Vector2f(0, height * grid_size), Vector2f(width * grid_size, height * grid_size), Vector2f(width * grid_size, 0)});
	
	//this->outline.setPosition(position_x, position_y);
	//this->outline.setSize(sf::Vector2f(width * grid_size, height * grid_size));
	this->outline.setOutlineThickness(2.f);
	this->outline.setFillColor(sf::Color::Transparent);
	this->outline.setOutlineColor(sf::Color::White);

	this->map.reserve(this->max_size.x);
	this->map_renderables.reserve(this->max_size.x * this->max_size.y * this->layers);

	create_empty_map();
	
	set_sorting_layer(Sorting::Layer::UI);
	set_z_order(0);
	Renderer::Instance()->add(Renderer::RenderObject(&outline, this));

	update_tilemap_changes();
}

TilemapComponent::~TilemapComponent()
{
	Renderer::Instance()->remove(&outline);
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

void TilemapComponent::init()
{
	load_tile_sheets();
	layers = ((int)Sorting::Layer::UI);
	
}

void TilemapComponent::update()
{
	for (auto& x : this->map_updateables)
		x->update();
}

void TilemapComponent::late_update()
{
}

void TilemapComponent::fixed_update()
{
}

void TilemapComponent::add_to_buffer(sf::View* view)
{
	set_view(view);
	for (auto& x : this->map_renderables)
	{
		x->add_to_buffer(view);
	}
}

void TilemapComponent::add_tiles(const unsigned x, const unsigned y, Sorting::Layer layer, const sf::IntRect texture_rect, TileNS::Type tile_type,
	bool collision, bool animated_sprite, float animation_timer)
{
	if (animated_sprite)
	{
		if (x >= max_size.x || y >= max_size.y)
			return;

		if (map[x][y][static_cast<int>(layer)])
			delete map[x][y][static_cast<int>(layer)];

		map[x][y][static_cast<int>(layer)] = new Tile(position.x, position.y, x, y, grid_size_f, layer, physical_layers[static_cast<int>(layer)]);
		
		map[x][y][static_cast<int>(layer)]->add_animated_sprite_component(current_tileset_key, &tile_sheets.at(current_tileset_key), texture_rect, animation_timer);
		map[x][y][static_cast<int>(layer)]->set_collision(collision);
		map[x][y][static_cast<int>(layer)]->set_type(tile_type);

		update_tilemap_changes();
		return;
	}

	if (x + static_cast<int>(texture_rect.width / 32) >= max_size.x ||
		y + static_cast<int>(texture_rect.height / 32) >= max_size.y)
		return;
	sf::IntRect sub_rect;
	sub_rect.width = grid_size_u;
	sub_rect.height = grid_size_u;
	for (int w = 0; w < texture_rect.width; w += grid_size_u)
	{
		sub_rect.left = w + texture_rect.left;
		for (int h = 0; h < texture_rect.height; h += grid_size_u)
		{
			sub_rect.top = h + texture_rect.top;
			
			if (map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)])
				delete map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)];

			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)] =
				new Tile(position.x, position.y, x + static_cast<int>(w) / 32.f, y + static_cast<int>(h) / 32.f, grid_size_f, layer, physical_layers[static_cast<int>(layer)]);

			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)]->set_texture(
				current_tileset_key, &tile_sheets.at(current_tileset_key), sub_rect);
			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)]->set_collision(collision);
			map[x + static_cast<int>(w) / 32.f][y + static_cast<int>(h) / 32.f][static_cast<int>(layer)]->set_type(tile_type);
		}
	}
	update_tilemap_changes();
}

void TilemapComponent::remove_tiles(const unsigned x, const unsigned y, const Sorting::Layer layer,
	const sf::IntRect texture_rect, bool animated_sprite)
{
	if (animated_sprite)
	{
		if (x >= max_size.x || y >= max_size.y)
			return;

		if(map[x][y][static_cast<int>(layer)])
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
	return physical_layers.at(static_cast<int>(layer));
}

std::vector<std::string> TilemapComponent::get_tileset_keys()
{
	return tileset_keys;
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
	load_tile_sheets();
	Json::Value obj = FileManager::Instance()->load_from_file(file_path);
	unserialize_json(obj);

}

void TilemapComponent::set_position(Vector2i pos)
{
	if (position.equals(pos))
		return;

	this->position = pos;
	outline.set_position(Vector2f(pos.x, pos.y));

	for (auto& x : this->map)
		for (auto& y : x)
			for (auto& z : y)
				if(z)
					z->set_position(pos);
		
}

std::vector<std::vector<std::vector<Tile*>>> TilemapComponent::get_tiles()
{
	return map;
}

std::vector<Tile*> TilemapComponent::get_collidable_tiles()
{
	std::vector<Tile*> col_tiles;
	for (auto& r : map_renderables)
	{
		if (r->get_collision())
			col_tiles.push_back(r);
	}
	return col_tiles;
}

void TilemapComponent::update_tilemap_changes()
{
	// Render Textures
	for (int i = 0; i < render_sprites.size(); i++)
	{
		Renderer::Instance()->remove(&render_sprites[i]);
		if(render_textures[i])
			delete render_textures[i];
	}
	render_textures.clear();
	render_sprites.clear();
	render_layers.clear();
	
	for (int i = 0; i < static_cast<int>(Sorting::Layer::UI) - 1; i++)
	{
		render_layers.push_back(static_cast<Sorting::Layer>(i));
		render_textures.push_back(new sf::RenderTexture());

		render_textures[i]->create(grid_size_u * max_size.x, grid_size_u * max_size.y);
		
		for (size_t x = 0; x < this->max_size.x; x++)
		{
			for (size_t y = 0; y < this->max_size.y; y++)
			{
				if (this->map[x][y][i])
				{
					if (!this->map[x][y][i]->get_animated_sprite_component())
					{
						render_textures[i]->draw(this->map[x][y][i]->get_sprite());
					}
				}
			}
		}
		
		render_textures[i]->display();
		sf::Sprite sprite(render_textures[i]->getTexture());
		sprite.setPosition(position.x, position.y);

		render_sprites.push_back(sprite);

	}
	for (int x = 0; x < render_sprites.size(); x++)
	{
		set_z_order(-2, false);
		Renderer::Instance()->add(Renderer::RenderObject(&render_sprites[x], get_render(), render_layers[x], get_z_order(), get_view_pointer()));
	}


	//return;
	map_renderables.clear();
	map_updateables.clear();
	for (auto& x : this->map)
	{
		for (auto& y : x)
		{
			for (auto& z : y)
			{
				
				if (z)
				{
					if (z->get_animated_sprite_component())
					{
						map_renderables.push_back(z);
						map_updateables.push_back(z);
					}
				}
				
			}
		}
	}

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

	for (auto r : map_renderables)
	{
		obj["tiles"].append(r->serialize_json());
	}

	return obj;
}

void TilemapComponent::unserialize_json(Json::Value obj)
{
	load_tile_sheets();
	layers = ((int)Sorting::Layer::UI);
	position = sf::Vector2i(obj["position.x"].asInt64(), obj["position.y"].asInt64());
	max_size.x = obj["max-size.x"].asUInt64();
	max_size.y = obj["max-size.y"].asUInt64();
	grid_size_u = obj["grid-size"].asUInt64();
	file_path = obj["file-path"].asString();
	grid_size_f = static_cast<float>(grid_size_u);

	this->map_renderables.clear();
	this->map_updateables.clear();

	this->map_renderables.reserve(this->max_size.x * this->max_size.y * this->layers);

	clear_map();
	create_empty_map();

	for (Json::Value tile : obj["tiles"])
	{
		int x = tile["grid.x"].asInt64();
		int y = tile["grid.y"].asInt64();
		int z = static_cast<int>(Sorting::ToLayer(tile["layer"].asString()));
		sf::IntRect texture_rect;
		texture_rect.left = tile["texture_rect.left"].asInt64();
		texture_rect.width = tile["texture_rect.width"].asInt64();
		texture_rect.top = tile["texture_rect.top"].asInt64();
		texture_rect.height = tile["texture_rect.height"].asInt64();

		bool collision = tile["collision"].asBool();
		TileNS::Type type = TileNS::ToTiletype(tile["tile-type"].asString());
		std::string texture_source = tile["texture-source"].asString();
		float animation_timer = tile["animation-timer"].asFloat();

		map[x][y][z] = new Tile(position.x, position.y, x, y, grid_size_f, static_cast<Sorting::Layer>(z), physical_layers[z]);

		if (tile["animated-sprite"].asBool())
			map[x][y][z]->add_animated_sprite_component(texture_source, &tile_sheets.at(texture_source),
				texture_rect, animation_timer);
		else
			map[x][y][z]->set_texture(texture_source, &tile_sheets.at(texture_source), texture_rect);

		map[x][y][z]->set_collision((bool)collision);
		map[x][y][z]->set_type(type);
	}

	update_tilemap_changes();
}

void TilemapComponent::load_tile_sheets()
{
	tile_sheets.clear();
	tileset_keys.clear();

	int n = 0;
	for (const auto& entry : std::filesystem::directory_iterator(tiles_file_path))
	{
		std::string file_name = entry.path().string().substr(tiles_file_path.size(), entry.path().string().size() - 1);
		if (n == 0)
			current_tileset_key = file_name;
		n++;

		if (!entry.is_directory())
		{
			if (!tile_sheets[file_name].loadFromFile(entry.path().string()))
			{
				Debug::Log("ERROR::TILEMAP::FAILED TO LOAD TEXTURE");
			}
		}
	}

	for (auto f : tile_sheets)
		tileset_keys.push_back(f.first);

}

void TilemapComponent::clear_map()
{
	for (size_t x = 0; x < this->max_size.x; x++)
	{
		for (size_t y = 0; y < this->max_size.y; y++)
		{
			for (size_t z = 0; z < this->layers; z++)
			{
				if(map[x][y][z])
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
	this->map.reserve(this->max_size.x);
	for (size_t x = 0; x < this->max_size.x; x++)
	{
		this->map.push_back(std::vector< std::vector<Tile*> >());
		for (size_t y = 0; y < this->max_size.y; y++)
		{
			this->map[x].reserve(this->max_size.y);
			this->map[x].push_back(std::vector<Tile*>());
			for (size_t z = 0; z < this->layers; z++)
			{
				this->map[x][y].reserve(this->layers);
				this->map[x][y].push_back(nullptr);
			}
		}
	}
}

}