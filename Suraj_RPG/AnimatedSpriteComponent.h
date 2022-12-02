#pragma once
#include "Animation.h"
namespace bm98
{
class AnimatedSpriteComponent
{

public:
	AnimatedSpriteComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
		sf::IntRect animation_rect, float animation_timer, int frame_width, int frame_height);
	//AnimatedSpriteComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimatedSpriteComponent();

	void update();

	void start();
	void stop();
	void next();
	void previous();
	void begin();
	void end();

private:
	Animation* animation;

};
}