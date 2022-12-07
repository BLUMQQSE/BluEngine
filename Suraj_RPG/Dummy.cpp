#include "pch.h"
#include "Dummy.h"
#include "Collisions.h"
#include "ChildAnimationComponent.h"
namespace bm98
{
using namespace core;

Dummy::Dummy(float x, float y)
{
	info.id = 102;
	info.tag = Tag::ACTOR;
	info.layer = Layer::ACTOR;
	info.name = "Dummy";
	set_position(x, y);

	if (!texture_sheet.loadFromFile("Resources/Images/Sprites/Items/pants.png"))
	{
		throw("ERROR::DUMMY::COULD NOT LOAD texture_sheet");
	}

	set_sprite_texture(texture_sheet);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(32, 32)));

	render_object = new Renderer::RenderObject(&sprite, SortingLayer::ACTOR, 1);
}

Dummy::~Dummy()
{
}

void Dummy::init_anim()
{
	add_component<ChildAnimationComponent>(sprite, texture_sheet, 
		parent->get_component<AnimationComponent>());
}

void Dummy::on_collision_enter(Collision info)
{
	GameObject::on_collision_enter(info);
}

void Dummy::update()
{
	GameObject::update();
}

void Dummy::fixed_update()
{
}
}