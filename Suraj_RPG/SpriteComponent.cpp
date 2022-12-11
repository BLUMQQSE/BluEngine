#include "pch.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "Debug.h"
namespace bm98
{



SpriteComponent::SpriteComponent()
{
	name = "SpriteComponent";
}

SpriteComponent::SpriteComponent(std::string file_path)
{
	name = "SpriteComponent";
	if (!this->texture_sheet.loadFromFile(sprite_path + file_path))
	{
		Debug::Log("ERROR::SpriteComponent::COULD NOT LOAD texture_sheet");
		return;
	};
	//file_path = "Data/player.json";

	sprite.setTexture(texture_sheet);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));

}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::init()
{

}

void SpriteComponent::update()
{
	if (game_object->get_parent())
		if (game_object->get_parent()->has_component<SpriteComponent>())
			sprite.setPosition(game_object->get_parent()->transform.position + game_object->transform.local_position);

}

Json::Value SpriteComponent::serialize_json()
{
	Json::Value obj;

	obj["name"] = name;
	obj["layer"] = Global::layer_to_string(layer);
	obj["z-order"] = z_order;
	obj["sprite-file-path"] = file_path;

	return obj;
}

void SpriteComponent::unserialize_json(Json::Value obj)
{
	file_path = obj["sprite-file-path"].asString();
	name = obj["name"].asString();
	layer = Global::string_to_layer(obj["layer"].asString());
	z_order = obj["z-order"].asInt64();

	std::cout << z_order << "z";

	file_path = obj["sprite-file-path"].asString();
	set_size(64, 64);
	set_sprite(file_path);
}

sf::Sprite& SpriteComponent::get_sprite()
{
    return sprite;
}

sf::Texture& SpriteComponent::get_texture_sheet()
{
	return texture_sheet;
}

SortingLayer SpriteComponent::get_layer()
{
	return layer;
}

int SpriteComponent::get_order()
{
	return z_order;
}

void SpriteComponent::set_sprite(std::string file_path)
{
	if (!this->texture_sheet.loadFromFile(sprite_path + file_path))
	{
		std::cout << file_path<<"\n";
		Debug::Log("ERROR::SpriteComponent::COULD NOT LOAD texture_sheet");
		return;
	};

	this->file_path = file_path;
	sprite.setTexture(texture_sheet);
	//sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));
	sprite.setTextureRect(
		sf::IntRect(sf::Vector2i(0, 0), 
		sf::Vector2i(size))
	);
}

void SpriteComponent::set_size(int size_x, int size_y)
{
	size.x = size_x;
	size.y = size_y;
}

void SpriteComponent::set_position(float x, float y)
{
	sprite.setPosition(x, y);
}

void SpriteComponent::set_layer(SortingLayer sorting_layer)
{
	this->layer = sorting_layer;
}

void SpriteComponent::set_z_order(int order)
{
	this->z_order = order;
}


}