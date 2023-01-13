#include "pch.h"
#include "Tile.h"
#include "AnimatedSpriteComponent.h"
#include "Renderer.h"
namespace bm98
{
Tile::Tile(int x_offset, int y_offset, int grid_x, int grid_y, float grid_size_f, SortingLayer layer)
{
	this->x_offset = x_offset;
	this->y_offset = y_offset;

	this->sprite.setPosition(std::floor(static_cast<float>(x_offset + grid_x) * grid_size_f), 
		std::floor(static_cast<float>(y_offset + grid_y) * grid_size_f));

	this->grid_x = grid_x;
	this->grid_y = grid_y;

	this->layer = layer;
	this->static_tile = true;
	this->collision = false;
	this->type = TileType::DEFAULT;
	animated_sprite_component = nullptr;

	set_render(false);
	set_sorting_layer(layer, false);
	set_z_order(-1, false);
}

Tile::~Tile()
{
	if (animated_sprite_component)
	{
		Renderer::remove(&sprite);
		delete animated_sprite_component;
	}
}

void Tile::update()
{
	if (animated_sprite_component)
		animated_sprite_component->update();
}

void Tile::late_update()
{
}

void Tile::fixed_update()
{
}

const bool Tile::is_static() const
{
	return static_tile;
}

const SortingLayer Tile::get_layer() const
{
	return layer;
}

const bool Tile::get_collision() const
{
	return collision;
}

const sf::FloatRect Tile::get_bounds() const
{
	return sprite.getGlobalBounds();
}

const AnimatedSpriteComponent* Tile::get_animated_sprite_component()
{
	return animated_sprite_component;
}

sf::Sprite& Tile::get_sprite()
{
	return sprite;
}

void Tile::set_texture(std::string source_key, const sf::Texture* texture, const sf::IntRect rect)
{
	texture_source = source_key;
	texture_rect = rect;
	sprite.setTexture(*texture);
	sprite.setTextureRect(rect);
}

void Tile::set_collision(bool collision)
{
	this->collision = collision;
}

void Tile::set_type(TileType tile_type)
{
	type = tile_type;
}

void Tile::set_position(Vector2i pos)
{
	x_offset = pos.x;
	y_offset = pos.y;

	this->sprite.setPosition(std::floor(static_cast<float>(x_offset + (grid_x * UNIT_SIZE))), 
		std::floor(static_cast<float>(y_offset + (grid_y * UNIT_SIZE))));
}

void Tile::add_animated_sprite_component(std::string source_key, sf::Texture* texture_sheet, sf::IntRect animation_rect, float animation_timer)
{
	remove_animated_sprite_component();
	texture_rect = animation_rect;
	texture_source = source_key;
	this->animation_timer = animation_timer;
	animation_rect.width -= UNIT_SIZE;
	animated_sprite_component = new AnimatedSpriteComponent(sprite, *texture_sheet, animation_rect,
		animation_timer, UNIT_SIZE, UNIT_SIZE);

	Renderer::add(Renderer::RenderObject(&sprite, this));
}

void Tile::remove_animated_sprite_component()
{
	if (animated_sprite_component)
	{
		Renderer::remove(&sprite);
		delete animated_sprite_component;
		animation_timer = 0;
	}
	animated_sprite_component = nullptr;
	
}

void Tile::remove_texture()
{
	texture_source = "NULL";
	//shape.setTexture(nullptr);
}

void Tile::highlight(bool highlight)
{
	//if (highlight)
		//shape.setOutlineColor(sf::Color::Yellow);
	//else
		//shape.setOutlineColor(sf::Color::Black);
}

void Tile::add_to_buffer(sf::View* view)
{
	set_view(view);
	
	if (collision)
		sprite.setColor(sf::Color(255, 0, 0, 100));
	else
		sprite.setColor(sf::Color::White);
		
}

Json::Value Tile::serialize_json()
{
	Json::Value obj;
	obj["grid.x"] = grid_x;
	obj["grid.y"] = grid_y;
	obj["layer"] = Global::layer_to_string(layer);
	obj["texture_rect.left"] = texture_rect.left;
	obj["texture_rect.width"] = texture_rect.width;
	obj["texture_rect.top"] = texture_rect.top;
	obj["texture_rect.height"] = texture_rect.height;
	obj["collision"] = collision;
	obj["tile-type"] = Global::tiletype_to_string(type);
	obj["texture-source"] = texture_source;
	obj["animated-sprite"] = (animated_sprite_component != nullptr);
	obj["animation-timer"] = animation_timer;

	return obj;
}

void Tile::unserialize_json(Json::Value obj)
{
	grid_x = obj["grid.x"].asInt64();
	grid_y = obj["grid.y"].asInt64();
	layer = Global::string_to_layer(obj["layer"].asString());

	texture_rect.left = obj["texture_rect.left"].asInt64();
	texture_rect.width = obj["texture_rect.width"].asInt64();
	texture_rect.top = obj["texture_rect.top"].asInt64();
	texture_rect.height = obj["texture_rect.height"].asInt64();

	collision = obj["collision"].asBool();
	type = Global::string_to_tiletype(obj["tile-type"].asString());
	texture_source = obj["texture-source"].asString();
	animation_timer = obj["animation-timer"].asFloat();

}
}
