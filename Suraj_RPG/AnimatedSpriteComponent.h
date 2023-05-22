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

	/// <summary>
	/// Should only be observed on a late_update().
	/// </summary>
	const bool frame_changed() const;

	void start();
	void stop();
	void next();
	void previous();
	void begin();
	void end();

private:
	std::unique_ptr<Animation> animation;

};
}