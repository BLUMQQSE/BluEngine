#include "pch.h"
#include "Dummy.h"
#include "Collisions.h"
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

	//add_box_collider_component(sprite, 16.f, -14.f, 32.f, 50.f);

	//sf::IntRect animation_rect;
	//animation_rect.left = 0;
	//animation_rect.top = 0;
	//animation_rect.width = 320 - 64;

	//add_animated_sprite_component(texture_sheet, sf::IntRect(animation_rect), 50.f, 64, 64);
	//add_animation_component(texture_sheet);

	//_animation_component->add_animation("IDLE_UP", 30.f, 0,0,0,0,64,64,true);
	//_animation_component->play("IDLE_UP");


	render_object = new Renderer::RenderObject(&sprite, SortingLayer::ACTOR, 1);
}

Dummy::~Dummy()
{
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