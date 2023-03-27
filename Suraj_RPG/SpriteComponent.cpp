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
	Renderer::Instance()->remove(&sprite);
}

void SpriteComponent::init()
{
	if (!ResourceManager::Instance()->has_texture(file_path))
	{
		return;
	}
	texture_sheet = &ResourceManager::Instance()->get_texture(file_path);

	sprite.setTexture(*texture_sheet);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), size));
	Renderer::Instance()->add(Renderer::RenderObject(&sprite, this));
}

void SpriteComponent::update()
{

}

void SpriteComponent::set_world_position(const Vector2f pos)
{
	sprite.setPosition(pos.x, pos.y);
}

Json::Value SpriteComponent::serialize_json()
{
	Json::Value obj;

	obj[typeid(IRenderable).name()] = IRenderable::serialize_json();

	obj["sprite-file-path"] = file_path;

	return obj;
}

void SpriteComponent::unserialize_json(Json::Value obj)
{

	IRenderable::unserialize_json(obj[typeid(IRenderable).name()]);

	file_path = obj["sprite-file-path"].asString();
	set_size(64, 64);
	set_sprite(file_path);
}

void SpriteComponent::set_active(bool active)
{
	Component::set_active(active);
	set_render(active);
}

std::vector<Editor::SerializedVar> SpriteComponent::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;
	
	values.push_back(Editor::SerializedVar("sorting_layer", static_cast<void*>(&get_sorting_layer()), Editor::VarType::Dropdown,
		Sorting::ToVector()));
	values.push_back(Editor::SerializedVar("z_order", static_cast<void*>(&get_z_order()), Editor::VarType::Int));
	values.push_back(Editor::SerializedVar("file_name", static_cast<void*>(&file_path), Editor::VarType::String));
	
	return values;
}

sf::Sprite& SpriteComponent::get_sprite()
{
    return sprite;
}

sf::Texture& SpriteComponent::get_texture_sheet()
{
	return *texture_sheet;
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

void SpriteComponent::set_sprite(std::string file_path)
{
	if (!ResourceManager::Instance()->has_texture(file_path))
		return;
	this->texture_sheet = &ResourceManager::Instance()->get_texture(file_path);

	this->file_path = file_path;
	sprite.setTexture(*texture_sheet);
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

}