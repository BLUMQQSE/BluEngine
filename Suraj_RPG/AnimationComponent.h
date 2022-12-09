#pragma once
#include"Component.h"
#include "Animation.h"
namespace bm98
{

class AnimationComponent : public Component
{
public:
	AnimationComponent();
	AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimationComponent();

	bool play(const std::string key, const float modifier = 1, const float modifier_max = 1);

	std::string get_active_animation_key();
	float get_modifier();
	float get_max_modifier();
	std::map<std::string, Animation*> get_animations();

	void add_animation(const std::string key, float anim_timer,
		int start_frame_x, int start_frame_y, int frames_x, int frames_y,
		int frame_width, int frame_height, bool loop_animation, bool must_complete = false);

	virtual void init() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

protected:


private:


	sf::Sprite* sprite;
	sf::Texture* texture_sheet;
	std::map<std::string, Animation*> animations;

	float modifier;
	float max_modifier;
	std::string active_animation_key;

	Animation* last_animation;

};

}