#include "pch.h"
#include "ChildAnimationComponent.h"
namespace bm98
{
ChildAnimationComponent::ChildAnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
	AnimationComponent& parent_animation_component)
	: sprite(sprite), texture_sheet(texture_sheet), parent_animation_component(parent_animation_component)
{
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

void ChildAnimationComponent::update()
{
	//handles playing same animation as parent 
	//can confirm key is accurate
	if (parents_last_animation != parent_animation_component.get_active_animation_key())
	{
		animations[parents_last_animation]->reset();
		parents_last_animation = parent_animation_component.get_active_animation_key();
	}
	animations[parent_animation_component.get_active_animation_key()]->play
	(std::abs(parent_animation_component.get_modifier()), std::abs(parent_animation_component.get_max_modifier()));

}

void ChildAnimationComponent::add_animations()
{
	for (auto& a : parent_animation_component.get_animations())
	{
		animations[a.first] = new Animation(sprite, texture_sheet, *a.second);
	}
}
}