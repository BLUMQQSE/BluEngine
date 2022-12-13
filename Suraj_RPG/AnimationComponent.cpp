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
	for (auto& i : animations)
	{
		delete i.second;
	}
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
	return(animations[key]->play(std::abs(modifier), std::abs(modifier_max)));
}

std::string AnimationComponent::get_active_animation_key()
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

std::map<std::string, Animation*> AnimationComponent::get_animations()
{
	return animations;
}

void AnimationComponent::add_animation(const std::string key,
	float anim_timer, int start_frame_x, int start_frame_y, int frames_x, int frames_y,
	int frame_width, int frame_height, bool loop_animation, bool must_complete)
{
	animations[key] = new Animation(*this->sprite, *this->texture_sheet, anim_timer,
		start_frame_x, start_frame_y, frames_x, frames_y, frame_width, frame_height,
		loop_animation, must_complete);
}

void AnimationComponent::init()
{
	sprite = &game_object->get_component<SpriteComponent>().get_sprite();
	texture_sheet = &game_object->get_component<SpriteComponent>().get_texture_sheet();
}

Json::Value AnimationComponent::serialize_json()
{
	Json::Value obj;
	
	/* Currently useless to save animations
	for (auto& a : animations)
	{
		Json::Value obj2;
		obj2["key"] = a.first;
		obj2["animation"] = a.second->serialize_json();
		
		obj["animations"].append(obj2);
	}
	*/
	return obj;
}

void AnimationComponent::unserialize_json(Json::Value obj)
{

	/* Currently useless to save animations
	for (Json::Value anim : obj["animations"])
	{
		animations[anim["key"].asString()] = new Animation(game_object->get_component<SpriteComponent>().get_sprite(),
			game_object->get_component<SpriteComponent>().get_texture_sheet());
		animations[anim["key"].asString()]->unserialize_json(anim["animation"]);
	}
	*/
}

#pragma endregion

}