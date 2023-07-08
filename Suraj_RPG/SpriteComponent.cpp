#include "pch.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "core/Debug.h"
#include "core/ResourceManager.h"
#include "core/Renderer.h"

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
	core::Renderer::Instance()->remove(&sprite);
}

void SpriteComponent::init()
{
	set_sprite(file_path);
}

void SpriteComponent::awake()
{
	core::Renderer::Instance()->add(core::Renderer::RenderObject(&sprite, this));
}

void SpriteComponent::update()
{
	set_y_pos(game_object->get_visual_center().y + y_pos_offset);
}

void SpriteComponent::set_world_position(const Vector2f pos)
{
	sprite.setPosition(pos.x, pos.y);
}

Json::Value SpriteComponent::serialize_json()
{
	Json::Value obj;

	obj[RemoveNamespace(typeid(IRenderable).name())] = IRenderable::serialize_json();

	obj["sprite-file-path"] = file_path;
	obj["size"] = size.serialize_json();
	obj["sorting-group"] = sorting_group;
	obj["pivot-offset"] = y_pos_offset;

	return obj;
}

void SpriteComponent::unserialize_json(Json::Value obj)
{
	size = Vector2i::Zero();
	IRenderable::unserialize_json(obj[RemoveNamespace(typeid(IRenderable).name())]);

	file_path = obj["sprite-file-path"].asString();
	size.unserialize_json(obj["size"]);
	sorting_group = obj["sorting-group"].asBool();
	y_pos_offset = obj["pivot-offset"].asFloat();
	
	if (size.x == 0 || size.y == 0)
		set_size(64, 64);
	else
		set_size(size.x, size.y);

	//set_size(64, 64);
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
	
	values.push_back(Editor::SerializedVar("size", (void*)&size, Var::Type::Vector2i));
	values.push_back(Editor::SerializedVar("sorting_layer", (void*)&get_sorting_layer(), Var::Type::Dropdown,
		Sorting::ToVector()));
	values.push_back(Editor::SerializedVar("z_order", (void*)&get_z_order(), Var::Type::Int));
	values.push_back(Editor::SerializedVar("file_name", (void*)&file_path, Var::Type::String));
	
	return values;
}

void SpriteComponent::editor_update()
{
	set_sprite(file_path);
	set_y_pos(game_object->get_visual_center().y + y_pos_offset);
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
	if (!core::ResourceManager::Instance()->has_texture(file_path))
	{
		return;
	}
	this->texture_sheet = &core::ResourceManager::Instance()->get_texture(file_path);

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