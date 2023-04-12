#pragma once
#include"Component.h"
#include "Animation.h"
namespace bm98
{
class AnimationComponent;

class ChildAnimationComponent : public Component
{
public:
	ChildAnimationComponent();
	ChildAnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
		AnimationComponent& parent_animation_component);


	virtual ~ChildAnimationComponent();

	virtual void init() override;
	virtual void awake() override;
	virtual void update() override;
	virtual void late_update() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

private:
	sf::Sprite* sprite;
	sf::Texture* texture_sheet;
	AnimationComponent* parent_animation_component;
	std::map<std::string, Animation*> animations;
	std::string parents_last_animation;

	void add_animations();

};

}