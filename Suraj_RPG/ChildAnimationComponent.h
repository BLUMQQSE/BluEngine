#pragma once
#include"Component.h"
#include "Animation.h"
namespace bm98
{
class AnimationComponent;

class ChildAnimationComponent : public Component
{
public:
	ChildAnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
		AnimationComponent& parent_animation_component);
	virtual ~ChildAnimationComponent();

	void update();

private:
	sf::Sprite& sprite;
	sf::Texture& texture_sheet;
	AnimationComponent& parent_animation_component;
	std::map<std::string, Animation*> animations;
	std::string parents_last_animation;

	void add_animations();

};

}