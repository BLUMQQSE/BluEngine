#pragma once
#include"Component.h"
#include "Animation.h"
namespace bm98
{
class AnimatedSpriteComponent : public Component
{

public:
	AnimatedSpriteComponent();
	AnimatedSpriteComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
		sf::IntRect animation_rect, float animation_timer, int frame_width, int frame_height);
	//AnimatedSpriteComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimatedSpriteComponent();

	virtual void init() override;
	virtual void update() override;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

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