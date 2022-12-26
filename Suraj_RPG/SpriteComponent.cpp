#include "pch.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "Debug.h"
#include "ResourceManager.h"
namespace bm98
{

SpriteComponent::SpriteComponent()
{
	
}

SpriteComponent::SpriteComponent(std::string file_path)
{
	this->file_path = file_path;

}

SpriteComponent::~SpriteComponent()
{
	Renderer::remove(&sprite);
}

void SpriteComponent::init()
{
	
	//if (!this->texture_sheet.loadFromFile(sprite_path + file_path))
	//{
	//	Debug::Log("ERROR::SpriteComponent::COULD NOT LOAD texture_sheet");
	//	return;
	//};
	if (!ResourceManager::has_texture(file_path))
	{
		std::cout << "issue finding texut\n";
		return;
	}
	texture_sheet = ResourceManager::get_texture(file_path);

	sprite.setTexture(texture_sheet);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), size));
	Renderer::add(Renderer::RenderObject(&sprite, this));
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

	obj[typeid(IRenderable).name()] = IRenderable::serialize_json();

	//obj["layer"] = Global::layer_to_string(sorting_layer);
	//obj["z-order"] = z_order;
	obj["sprite-file-path"] = file_path;

	return obj;
}

void SpriteComponent::unserialize_json(Json::Value obj)
{

	IRenderable::unserialize_json(obj[typeid(IRenderable).name()]);

	file_path = obj["sprite-file-path"].asString();
	//sorting_layer = Global::string_to_layer(obj["layer"].asString());
	//z_order = obj["z-order"].asInt64();

	//file_path = obj["sprite-file-path"].asString();
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

void SpriteComponent::add_to_buffer(sf::View* view)
{
	set_view(view);
}

const std::string SpriteComponent::get_file_path() const
{
	return file_path;
}

const sf::Vector2i SpriteComponent::get_size() const
{
	return size;
}

SortingLayer SpriteComponent::get_layer()
{
	return get_sorting_layer();
}

int SpriteComponent::get_order()
{
	return get_z_order();
}

void SpriteComponent::set_sprite(std::string file_path)
{
	//if (!this->texture_sheet.loadFromFile(sprite_path + file_path))
	//	return;
	if (!ResourceManager::has_texture(file_path))
		return;
	this->texture_sheet = ResourceManager::get_texture(file_path);

	this->file_path = file_path;
	sprite.setTexture(texture_sheet);
	sprite.setTextureRect(
		sf::IntRect(sf::Vector2i(0, 0), 
		sf::Vector2i(size))
	);
}

void SpriteComponent::set_size(int size_x, int size_y)
{
	size.x = size_x;
	size.y = size_y;
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0),
		sf::Vector2i(size)));
}

void SpriteComponent::set_position(float x, float y)
{
	sprite.setPosition(x, y);
}

void SpriteComponent::set_layer(SortingLayer sorting_layer)
{
	IRenderable::set_sorting_layer(sorting_layer);
}

void SpriteComponent::set_z_order(int order)
{
	IRenderable::set_z_order(order);
}


}