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
	std::shared_ptr<AnimationComponent> parent_animation_component)
	: sprite(&sprite), texture_sheet(&texture_sheet), parent_animation_component(parent_animation_component)
{
	add_animations();
	parents_last_animation = parent_animation_component->get_active_animation_key();
}

ChildAnimationComponent::~ChildAnimationComponent()
{
}

void ChildAnimationComponent::init()
{
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_COMPONENT_ADDED_FLAG, this);
	EventSystem::Instance()->subscribe(EventID::GAMEOBJECT_COMPONENT_REMOVED_FLAG, this);

	sprite = &game_object->get_component<SpriteComponent>().lock()->get_sprite();
	texture_sheet = &game_object->get_component<SpriteComponent>().lock()->get_texture_sheet();
}

void ChildAnimationComponent::awake()
{
	std::shared_ptr<GameObject> g = game_object->get_parent().lock();
	if (g)
	{
		while (g->get_parent().lock() && !g->has_component<AnimationComponent>())
		{
			g = g->get_parent().lock();
		}
		if (g->get_component<AnimationComponent>().lock())
		{
			parent_animation_component = g->get_component<AnimationComponent>();
			add_animations();
			parents_last_animation = parent_animation_component.lock()->get_active_animation_key();
			return;
		}
	}
	
	
	parent_animator_exists = false;
	std::stringstream s;
	s << "[ChildAnimationComponent]" << game_object->get_info().name << "["<<game_object->get_unique_runtime_id() << "]" << "Failed to find parent animation component";
	core::Debug::Instance()->log(s.str(), core::Debug::LogLevel::WARNING);
}

void ChildAnimationComponent::update()
{
	if (animations.size() == 0)
		return;

	if (parents_last_animation != parent_animation_component.lock()->get_active_animation_key())
	{
		animations[parents_last_animation]->reset();
		parents_last_animation = parent_animation_component.lock()->get_active_animation_key();
	}
	animations[parent_animation_component.lock()->get_active_animation_key()]->play
	(std::abs(parent_animation_component.lock()->get_modifier()), std::abs(parent_animation_component.lock()->get_max_modifier()));

}

void ChildAnimationComponent::late_update()
{
	if (animations.size() == 0)
		return;

	if (parent_animation_component.lock()->get_animations()[parent_animation_component.lock()->get_active_animation_key()].lock()->frame_changed)
	{
		EventSystem::Instance()->push_event(EventID::ANIMATION_FRAME_CHANGE, nullptr, 
			Caller(Caller::Name::CHILD_ANIMATION_COMPONENT, (void*)this));
	}
	if (parent_animation_component.lock()->get_animations()[parent_animation_component.lock()->get_active_animation_key()].lock()->animation_ended_this_frame)
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
	animations.clear();
	for (auto& a : parent_animation_component.lock()->get_animations())
	{
		animations[a.first] = std::make_shared<Animation>(*sprite, *texture_sheet, *a.second.lock());
	}
}

void ChildAnimationComponent::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
		{
		case EventID::GAMEOBJECT_COMPONENT_ADDED_FLAG:
		case EventID::GAMEOBJECT_COMPONENT_REMOVED_FLAG:

			GameObject* go = static_cast<GameObject*>(static_cast<core::Caller>(event->get_caller()).pointer);

			if (go == this->game_object->get_parent().lock().get())
			{
				if (go->has_component<AnimationComponent>())
				{
					// here would need to check if need to toggle animating back on
					if (parent_animator_exists == false)
					{
						parent_animator_exists = true;
						parent_animation_component = go->get_component<AnimationComponent>();
						add_animations();
					}
				}
				else
				{
					// here would need to check if need to toggle animating back on
					if (parent_animator_exists == true)
					{
						parent_animator_exists = false;
						parent_animation_component = std::weak_ptr<AnimationComponent>();
						animations.clear();
					}
				}
			}
			break;
		}
		default:
			break;
	}
}

}