#include "pch.h"
#include "AnimationComponent.h"
#include "ChildAnimationComponent.h"
#include "SpriteComponent.h"
#include "GameObject.h"
namespace bm98
{
ChildAnimationComponent::ChildAnimationComponent()
{

}
ChildAnimationComponent::ChildAnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
	AnimationComponent& parent_animation_component)
	: sprite(&sprite), texture_sheet(&texture_sheet), parent_animation_component(&parent_animation_component)
{
	name = "ChildAnimationComponent";
	add_animations();
	parents_last_animation = parent_animation_component.get_active_animation_key();
}

ChildAnimationComponent::~ChildAnimationComponent()
{
	for (auto& i : animations)
	{
		delete i.second;
	}
}

void ChildAnimationComponent::init()
{
	sprite = &game_object->get_component<SpriteComponent>().get_sprite();
	texture_sheet = &game_object->get_component<SpriteComponent>().get_texture_sheet();
}

void ChildAnimationComponent::awake()
{
	if (game_object->get_parent())
		if (&game_object->get_parent()->get_component<AnimationComponent>())
		{
			parent_animation_component = &game_object->get_parent()->get_component<AnimationComponent>();
			add_animations();
			parents_last_animation = parent_animation_component->get_active_animation_key();
			return;
		}

	std::cout << "\nERROR::ChildAnimationComponent::ISSUE WITH PARENT AnimationComponent\n";
}

void ChildAnimationComponent::update()
{
	//handles playing same animation as parent 
	//can confirm key is accurate
	if (parents_last_animation != parent_animation_component->get_active_animation_key())
	{
		animations[parents_last_animation]->reset();
		parents_last_animation = parent_animation_component->get_active_animation_key();
	}
	animations[parent_animation_component->get_active_animation_key()]->play
	(std::abs(parent_animation_component->get_modifier()), std::abs(parent_animation_component->get_max_modifier()));

}

Json::Value ChildAnimationComponent::serialize_json()
{
	Json::Value obj;

	obj["name"] = name;

	return obj;
}

void ChildAnimationComponent::unserialize_json(Json::Value obj)
{
	obj["name"] = name;
}

void ChildAnimationComponent::add_animations()
{
	for (auto& a : parent_animation_component->get_animations())
	{
		animations[a.first] = new Animation(*sprite, *texture_sheet, *a.second);
	}
}
}