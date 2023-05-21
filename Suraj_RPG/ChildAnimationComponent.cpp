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
	sprite = &game_object->get_component<SpriteComponent>().lock()->get_sprite();
	texture_sheet = &game_object->get_component<SpriteComponent>().lock()->get_texture_sheet();
}

void ChildAnimationComponent::awake()
{
	if (game_object->get_parent().lock())
	{
		std::shared_ptr<GameObject> g = game_object->get_parent().lock();
		while (g->get_parent().lock() && !g->has_component<AnimationComponent>())
		{
			g = g->get_parent().lock();
		}
		if (g->get_component<AnimationComponent>().lock())
		{
			parent_animation_component = g->get_component<AnimationComponent>().lock().get();
			add_animations();
			parents_last_animation = parent_animation_component->get_active_animation_key();
			return;
		}
	}
	std::stringstream s;
	s << "[ChildAnimationComponent]" << game_object->get_info().name << "["<<game_object->get_unique_runtime_id() << "]" << "Failed to find parent animation component";
	core::Debug::Instance()->log(s.str(), core::Debug::LogLevel::WARNING);
}

void ChildAnimationComponent::update()
{
	if (animations.size() == 0)
		return;

	if (parents_last_animation != parent_animation_component->get_active_animation_key())
	{
		animations[parents_last_animation]->reset();
		parents_last_animation = parent_animation_component->get_active_animation_key();
	}
	animations[parent_animation_component->get_active_animation_key()]->play
	(std::abs(parent_animation_component->get_modifier()), std::abs(parent_animation_component->get_max_modifier()));

}

void ChildAnimationComponent::late_update()
{
	if (parent_animation_component->get_animations()[parent_animation_component->get_active_animation_key()]->frame_changed)
	{
		EventSystem::Instance()->push_event(EventID::ANIMATION_FRAME_CHANGE, nullptr, 
			Caller(Caller::Name::CHILD_ANIMATION_COMPONENT, (void*)this));
	}
	if (parent_animation_component->get_animations()[parent_animation_component->get_active_animation_key()]->animation_ended_this_frame)
	{
		EventSystem::Instance()->push_event(EventID::ANIMATION_COMPLETE, nullptr, 
			Caller(Caller::Name::CHILD_ANIMATION_COMPONENT, (void*)this));
	}
}

Json::Value ChildAnimationComponent::serialize_json()
{
	Json::Value obj;

	return obj;
}

void ChildAnimationComponent::unserialize_json(Json::Value obj)
{
}

void ChildAnimationComponent::add_animations()
{
	for (auto& a : parent_animation_component->get_animations())
	{
		animations[a.first] = new Animation(*sprite, *texture_sheet, *a.second);
	}
}
}