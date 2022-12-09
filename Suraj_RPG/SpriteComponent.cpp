#include "pch.h"
#include "SpriteComponent.h"
#include "GameObject.h"
namespace bm98
{



SpriteComponent::SpriteComponent()
{
}

SpriteComponent::SpriteComponent(std::string file_path)
{
	name = "SpriteComponent";
	if (!this->texture_sheet.loadFromFile(file_path))
	{
		throw("ERROR::SpriteComponent::COULD NOT LOAD texture_sheet");
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

	return obj;
}

void SpriteComponent::unserialize_json(Json::Value obj)
{
	name = obj["name"].asString();
}

sf::Sprite& SpriteComponent::get_sprite()
{
    return sprite;
}

sf::Texture& SpriteComponent::get_texture_sheet()
{
	return texture_sheet;
}


}