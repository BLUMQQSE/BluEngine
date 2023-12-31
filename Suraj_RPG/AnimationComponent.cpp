#include "pch.h"
#include "AnimationComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "Time.h"
namespace bm98
{
#pragma region PUBLIC

AnimationComponent::AnimationComponent()
{

}

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet)
	:sprite(&sprite), texture_sheet(&texture_sheet), last_animation(NULL)
{

}

AnimationComponent::~AnimationComponent()
{
//	for (auto& i : animations)
	//{
		//delete i.second;
	//}
	animations.clear();
}

void AnimationComponent::init()
{
	sprite = &game_object->get_component<SpriteComponent>().lock()->get_sprite();
	texture_sheet = &game_object->get_component<SpriteComponent>().lock()->get_texture_sheet();
}

void AnimationComponent::late_update()
{
	
}

/// <summary>
/// Plays animation mapped to key.
/// </summary>
/// <param name="key">Name of animation.</param>
/// <param name="modifier">Speed modifier.</param>
/// <param name="modifier_max">Speed modifier max.</param>
/// <returns>True if animation is still playing, False if animation is complete</returns>
bool AnimationComponent::play(const std::string key, const float modifier, const float modifier_max)
{
	if (last_animation != animations[key])
	{
		if (last_animation == NULL)
			last_animation = animations[key];

		if (last_animation->must_complete &&
			!last_animation->animation_complete)
		{
			last_animation->play(1, 1);
			return true;
		}
		last_animation->reset();
		last_animation = animations[key];
	}
	active_animation_key = key;
	this->modifier = modifier;
	this->max_modifier = modifier_max;

	bool play = animations[key]->play(std::abs(modifier), std::abs(modifier_max));

	if (animations.at(active_animation_key)->frame_changed)
	{
		EventSystem::Instance()->push_event(EventID::ANIMATION_FRAME_CHANGE, &active_animation_key,
			Caller(Caller::Name::ANIMATION_COMPONENT, (void*)this));
	}
	if (animations.at(active_animation_key)->animation_ended_this_frame)
	{
		EventSystem::Instance()->push_event(EventID::ANIMATION_COMPLETE, &active_animation_key,
			Caller(Caller::Name::ANIMATION_COMPONENT, (void*)this));
	}

	return play;
}

std::string& AnimationComponent::get_active_animation_key()
{
	return active_animation_key;
}

float AnimationComponent::get_modifier()
{
	return modifier;
}

float AnimationComponent::get_max_modifier()
{
	return max_modifier;
}

std::map<std::string, std::weak_ptr<Animation>> AnimationComponent::get_animations()
{
	std::map<std::string, std::weak_ptr<Animation>> result;
	for (auto i = animations.begin(); i != animations.end(); ++i)
	{
		result[i->first] = i->second;
	}

	return result;
}

void AnimationComponent::add_animation(const std::string key,
	float anim_timer, int start_frame_x, int start_frame_y, int frames_x, int frames_y,
	int frame_width, int frame_height, bool loop_animation, bool must_complete)
{
	animations[key] = std::make_shared<Animation>(*this->sprite, *this->texture_sheet, anim_timer * UNIT_SIZE,
		start_frame_x, start_frame_y, frames_x - 1, frames_y - 1, frame_width, frame_height,
		loop_animation, must_complete);
}

const bool AnimationComponent::frame_changed() const
{
	return animations.at(active_animation_key)->frame_changed;
}

#pragma endregion

}