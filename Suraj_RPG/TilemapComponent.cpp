#include "pch.h"
#include "TilemapComponent.h"
#include <filesystem>
#include "Debug.h"
#include "Renderer.h"
#include "Tile.h"
#include "FileManager.h"
#include "GameObject.h"
namespace bm98
{
using namespace core;
TilemapComponent::TilemapComponent()
{
}

TilemapComponent::TilemapComponent(int position_x, int position_y, float grid_size,
	unsigned width, unsigned height)
{
	this->position = sf::Vector2i(position_x, position_y);
	this->grid_size_f = grid_size;
	this->grid_size_u = static_cast<unsigned>(this->grid_size_f);
	this->max_size.x = width;
	this->max_size.y = height;
	load_tile_sheets();
	this->layers = ((int)SortingLayer::UI);

	this->outline.setPosition(position_x, position_y);
	this->outline.setSize(sf::Vector2f(width * grid_size, height * grid_size));
	this->outline.setOutlineThickness(1.f);
	this->outline.setFillColor(sf::Color::Transparent);
	this->outline.setOutlineColor(sf::Color::White);

	this->map.reserve(this->max_size.x);
	this->map_renderables.reserve(this->max_size.x * this->max_size.y * this->layers);

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
				this->map[x][y].push_back(new Tile(position_x, position_y, x, y, grid_size_f, (SortingLayer)z));
			}
		}
	}

	update_tilemap_changes();
}

TilemapComponent::~TilemapComponent()
{
	clear();
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
	layers = ((int)SortingLayer::UI);
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
	Renderer::add(Renderer::RenderObject(&outline, SortingLayer::BACKGROUND, 0, view));
	for (auto& x : this->map_renderables)
	{
		x->add_to_buffer(view);
	}
}

void TilemapComponent::add_tiles(const unsigned x, const unsigned y, SortingLayer layer, const sf::IntRect texture_rect, TileType tile_type,
	bool collision, bool animated_sprite, float animation_timer)
{
	if (animated_sprite)
	{
		if (x >= max_size.x || y >= max_size.y)
			return;
		//map[x][y][(int)layer]->set_texture(current_tileset_key, 
			//&tile_sheets.at(current_tileset_key), texture_rect);
		map[x][y][static_cast<int>(layer)]->add_animated_sprite_component(current_tileset_key, &tile_sheets.at(current_tileset_key), texture_rect, animation_timer);
		map[x][y][static_cast<int>(layer)]->set_empty(false);
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

			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_texture(
				current_tileset_key, &tile_sheets.at(current_tileset_key), sub_rect);
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_empty(false);
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_collision(collision);
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_type(tile_type);
		}
	}
	update_tilemap_changes();
}

void TilemapComponent::remove_tiles(const unsigned x, const unsigned y, const SortingLayer layer,
	const sf::IntRect texture_rect, bool animated_sprite)
{
	if (animated_sprite)
	{
		if (x >= max_size.x || y >= max_size.y)
			return;
		map[x][y][static_cast<int>(layer)]->remove_texture();
		map[x][y][static_cast<int>(layer)]->set_empty(true);
		map[x][y][static_cast<int>(layer)]->set_collision(false);
		map[x][y][static_cast<int>(layer)]->set_type(TileType::DEFAULT);
		map[x][y][static_cast<int>(layer)]->remove_animated_sprite_component();
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
			sf::IntRect sub_rect;

			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h)/32][static_cast<int>(layer)]->remove_texture();
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_empty(true);
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_collision(false);
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->set_type(TileType::DEFAULT);
			map[x + static_cast<int>(w) / 32][y + static_cast<int>(h) / 32][static_cast<int>(layer)]->remove_animated_sprite_component();
		}
	}
	update_tilemap_changes();
}

void TilemapComponent::highlight_layer(SortingLayer layer)
{
	editor_active_layer = layer;
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
	FileManager::save_to_file_styled(serialize_json(), file_path);
}

void TilemapComponent::load_from_json(std::string file_path)
{
	clear();
	load_tile_sheets();
	layers = ((int)SortingLayer::UI);
	Json::Value obj = FileManager::load_from_file(file_path, true);
	unserialize_json(obj);

}

void TilemapComponent::set_position(sf::Vector2f pos)
{
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
	map_renderables.clear();
	map_updateables.clear();
	for (auto& x : this->map)
	{
		for (auto& y : x)
		{
			for (auto& z : y)
			{
				if (!z->is_empty())
				{

					if (z->get_layer() == editor_active_layer)
						z->highlight(true);
					else
						z->highlight(false);

					map_renderables.push_back(z);
				}
				if (z->get_animated_sprite_component())
				{
					map_updateables.push_back(z);
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
	std::cout << "unserialize tilemap";
	load_tile_sheets();
	layers = ((int)SortingLayer::UI);
	position = sf::Vector2i(obj["position.x"].asInt64(), obj["position.y"].asInt64());
	max_size.x = obj["max-size.x"].asUInt64();
	max_size.y = obj["max-size.y"].asUInt64();
	grid_size_u = obj["grid-size"].asUInt64();
	file_path = obj["file-path"].asString();
	grid_size_f = static_cast<float>(grid_size_u);

	this->map_renderables.clear();
	this->map_updateables.clear();

	this->map_renderables.reserve(this->max_size.x * this->max_size.y * this->layers);

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
				map[x][y].reserve(this->layers);
				this->map[x][y].push_back(new Tile(std::floorf(game_object->get_transform().position.x / grid_size_f), 
					std::floorf(game_object->get_transform().position.y / grid_size_f), x, y, grid_size_f, (SortingLayer)z));
			}
		}
	}

	for (Json::Value tile : obj["tiles"])
	{
		int x = tile["grid.x"].asInt64();
		int y = tile["grid.y"].asInt64();
		int z = static_cast<int>(Global::string_to_layer(tile["layer"].asString()));
		sf::IntRect texture_rect;
		texture_rect.left = tile["texture_rect.left"].asInt64();
		texture_rect.width = tile["texture_rect.width"].asInt64();
		texture_rect.top = tile["texture_rect.top"].asInt64();
		texture_rect.height = tile["texture_rect.height"].asInt64();

		bool collision = tile["collision"].asBool();
		TileType type = Global::string_to_tiletype(tile["tile-type"].asString());
		std::string texture_source = tile["texture-source"].asString();
		float animation_timer = tile["animation-timer"].asFloat();

		if (tile["animated-sprite"].asBool())
			map[x][y][z]->add_animated_sprite_component(texture_source, &tile_sheets.at(texture_source),
				texture_rect, animation_timer);
		else
			map[x][y][z]->set_texture(texture_source, &tile_sheets.at(texture_source), texture_rect);

		map[x][y][z]->set_collision((bool)collision);
		map[x][y][z]->set_type(type);
		map[x][y][z]->set_empty(false);


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

void TilemapComponent::clear()
{
	for (size_t x = 0; x < this->max_size.x; x++)
	{
		for (size_t y = 0; y < this->max_size.y; y++)
		{
			for (size_t z = 0; z < this->layers; z++)
			{
				delete map[x][y][z];
				map[x][y][z] = nullptr;
			}
			map[x][y].clear();

		}
		map[x].clear();
	}
	map.clear();

}
}