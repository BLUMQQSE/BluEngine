#pragma once
#include "Renderer.h"
#include "Time.h"
namespace bm98
{
using namespace core;

class Animation
{
public:
	sf::Sprite& sprite;
	sf::Texture& texture_sheet;
	float animation_timer;
	float timer;
	int frame_width;
	int frame_height;
	int start_frame_x;
	int start_frame_y;
	int frames_x;
	int frames_y;
	sf::IntRect start_rect;
	sf::IntRect current_rect;
	sf::IntRect end_rect;

	bool loop_animation;
	bool must_complete;
	bool animation_complete;
	bool paused;

	/// <summary>
	///	Constructor used by AnimationComponent.
	/// </summary>
	Animation(sf::Sprite& sprite, sf::Texture& texture_sheet, float anim_timer,
		int start_frame_x, int start_frame_y, int frames_x, int frames_y,
		int frame_width, int frame_height, bool loop_animation, bool must_complete)
		: sprite(sprite), texture_sheet(texture_sheet), animation_timer(anim_timer),
		frame_width(frame_width), frame_height(frame_height),
		loop_animation(loop_animation), must_complete(must_complete), paused(false)
	{
		this->animation_complete = false;
		this->timer = 0.f;
		this->start_rect = sf::IntRect(start_frame_x * frame_width, start_frame_y *
			frame_height, frame_width, frame_height);
		this->end_rect = sf::IntRect(this->start_rect.left + frames_x * frame_width, this->start_rect.top + frames_y * frame_height,
			frame_width, frame_height);
		this->current_rect = start_rect;
		this->start_frame_x = start_frame_x;
		this->start_frame_y = start_frame_y;
		this->frames_x = frames_x;
		this->frames_y = frames_y;
		this->sprite.setTexture(this->texture_sheet, true);
		this->sprite.setTextureRect(this->start_rect);
	}

	/// <summary>
	/// Constructor used by ChildAnimationComponent.
	/// </summary>
	Animation(sf::Sprite& sprite, sf::Texture& texture_sheet, Animation& animation)
		: sprite(sprite), texture_sheet(texture_sheet), animation_timer(animation.animation_timer),
		frame_width(animation.frame_width), frame_height(animation.frame_height),
		loop_animation(animation.loop_animation), must_complete(animation.must_complete), paused(false)
	{
		this->animation_complete = false;
		this->timer = animation.timer;
		this->start_frame_x = animation.start_frame_x;
		this->start_frame_y = animation.start_frame_y;
		this->frames_x = animation.frames_x;
		this->frames_y = animation.frames_y;
		this->start_rect = sf::IntRect(start_frame_x * frame_width, start_frame_y *
			frame_height, frame_width, frame_height);
		this->end_rect = sf::IntRect(this->start_rect.left + frames_x * frame_width, this->start_rect.top + frames_y * frame_height,
			frame_width, frame_height);
		this->current_rect = start_rect;
		this->sprite.setTexture(this->texture_sheet, true);
		this->sprite.setTextureRect(this->start_rect);
	}

	bool play(const float& modifier, const float& modifier_max)
	{
		//std::cout << "Start:\nx: " << start_rect.left << "\ny: " << start_rect.top << "\nCurrent:\n" <<
			//"x: " << current_rect.left << "\ny: " << current_rect.top << "\nEnd:\n" << "x: " << end_rect.left <<
			//"\ny: " << end_rect.top << "\n";
		if (paused)
			return false;
		if (timer >= animation_timer)
		{
			timer = 0.f;
			//std::cout << "current_rect.left: " << current_rect.left << "\n";
			if (current_rect.left != end_rect.left)
			{
				current_rect.left += frame_width;
			}
			else
			{
				if (loop_animation)
					current_rect.left = start_rect.left;
				else
				{
					animation_complete = true;
					return false;
				}
				//TODO: Handle if an animation is not loop
				//Likely will involve returning a bool false when done
				//true if not or looping
			}
		}
		else
		{
			if (modifier / modifier_max < 0.5f)
				timer += (0.5f) * 100.f * Time::delta_time();
			else
				timer += (modifier / modifier_max) * 100.f * Time::delta_time();

		}
		sprite.setTextureRect(current_rect);
		return true;
	}

	void reset()
	{
		timer = 0.f;
		current_rect = start_rect;
		animation_complete = false;
	}
};
}