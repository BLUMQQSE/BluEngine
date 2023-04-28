#pragma once
#include "IData.h"
#include "core/Renderer.h"
#include "core/Time.h"
namespace bm98
{
using namespace core;

class Animation : public IData
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
	/// True if frame changed this play cycle. Should only be observed on late_update
	///or after calling play.
	/// </summary>
	bool frame_changed = false;
	bool animation_ended_this_frame = false;

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

	/// <summary>
	/// Constructor used for loading an animation from json data.
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="texture_sheet"></param>
	Animation(sf::Sprite& sprite, sf::Texture& texture_sheet)
		:sprite(sprite), texture_sheet(texture_sheet)
	{
	}

	bool play(const float& modifier, const float& modifier_max)
	{
		if (paused)
			return false;
		
		if (frame_changed)
			frame_changed = false;
		if (animation_ended_this_frame)
		{
			animation_ended_this_frame = false;
		}
		if (timer >= animation_timer)
		{
			timer = 0.f;
			if (current_rect.left != end_rect.left)
			{
				current_rect.left += frame_width;
				frame_changed = true;
			}
			else
			{
				if (loop_animation)
				{
					current_rect.left = start_rect.left;
					// Dont waste time frame changing if animation is just a single frame
					if(start_rect.left != end_rect.left)
						frame_changed = true;
				}
				else
				{
					// Ensure this gets set true only once
					if (!animation_complete)
					{
						animation_ended_this_frame = true;
					}
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
			float boost = modifier / modifier_max;
			if (boost < 0.5f)
			{
				timer += (0.5f) * 100.f * Time::Instance()->delta_time();
			}
			else
			{
				if (modifier == 1)
					boost *= 25;
				timer += boost * 100.f * Time::Instance()->delta_time();
			}
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


	// Inherited via IData
	virtual Json::Value serialize_json() override
	{
		Json::Value obj;
		/* Currently useless to save animations
		obj["animation-timer"] = animation_timer;
		obj["timer"] = timer;
		obj["frame-width"] = frame_width;
		obj["frame-height"] = frame_height;
		obj["start-frame-x"] = start_frame_x;
		obj["start-frame-y"] = start_frame_y;
		obj["frame-count-x"] = frames_x;
		obj["frame-count-y"] = frames_y;
		obj["loop"] = loop_animation;
		obj["must-complete"] = must_complete;
		obj["animation-complete"] = animation_complete;
		obj["paused"] = paused;

		obj["current-frame-left"] = current_rect.left;
		obj["current-frame-top"] = current_rect.top;
		*/
		return obj;
	}
	virtual void unserialize_json(Json::Value obj) override
	{
		/* Currently useless to save animations
		animation_timer = obj["animation-timer"].asFloat();
		timer = obj["timer"].asFloat();
		frame_width = obj["frame-width"].asInt64();
		frame_height = obj["frame-height"].asInt64();
		start_frame_x = obj["start-frame-x"].asInt64();
		start_frame_y = obj["start-frame-y"].asInt64();
		frames_x = obj["frame-count-x"].asInt64();
		frames_y = obj["frame-count-y"].asInt64();
		loop_animation = obj["loop"].asBool();
		must_complete = obj["must-complete"].asBool();
		animation_complete = obj["animation-complete"].asBool();
		paused = obj["paused"].asBool();

		start_rect = sf::IntRect(start_frame_x * frame_width, start_frame_y *
			frame_height, frame_width, frame_height);
		end_rect = sf::IntRect(start_rect.left + frames_x * frame_width, 
			start_rect.top + frames_y * frame_height,
			frame_width, frame_height);
		current_rect = start_rect;
		current_rect.left = obj["current-frame-left"].asFloat();;
		current_rect.top = obj["current-frame-top"].asFloat();
		*/
	}

};
}