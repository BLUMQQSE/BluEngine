#include "pch.h"
#include "AnimatedSpriteComponent.h"

namespace bm98
{

AnimatedSpriteComponent::AnimatedSpriteComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
	sf::IntRect animation_rect, float animation_timer, int frame_width, int frame_height)
{
	int x = static_cast<int>(animation_rect.width / frame_width);

	//need to convert animation_rect.left to be divided by frame_width
	//need to convert animation_rect.top to be divided by frame_height

	int width = static_cast<int>(animation_rect.left / frame_width);
	int height = static_cast<int>(animation_rect.top / frame_height);

	animation = new Animation(sprite, texture_sheet, animation_timer, width,
		height, x, 0, frame_width, frame_height, true, false);
	animation->play(1, 1);
}

AnimatedSpriteComponent::~AnimatedSpriteComponent()
{
	delete animation;
}

void AnimatedSpriteComponent::update()
{
	animation->play(1, 1);
}

void AnimatedSpriteComponent::start()
{
	animation->paused = false;
}

void AnimatedSpriteComponent::stop()
{
	animation->paused = true;
}

void AnimatedSpriteComponent::next()
{
	if (animation->current_rect.left == animation->end_rect.left)
		animation->current_rect.left = animation->start_rect.left + animation->frame_width;
	else if (animation->current_rect.left + animation->frame_width == animation->end_rect.left)
		animation->current_rect.left = animation->start_rect.left;
	else
		animation->current_rect.left += animation->frame_width;
}

void AnimatedSpriteComponent::previous()
{
	if (animation->current_rect.left == animation->start_rect.left)
		animation->current_rect.left = animation->end_rect.left - animation->frame_width;
	else
		animation->current_rect.left -= animation->frame_width;
}

void AnimatedSpriteComponent::begin()
{
	animation->current_rect.left = animation->start_rect.left;
}

void AnimatedSpriteComponent::end()
{
	animation->current_rect.left = animation->end_rect.left - animation->frame_width;
}
}