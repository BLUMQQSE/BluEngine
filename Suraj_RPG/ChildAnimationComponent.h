#pragma once
#include"Component.h"
#include "core/EventSystem.h"
#include "Animation.h"
namespace bm98
{
class AnimationComponent;

class ChildAnimationComponent : public Component, public Listener
{
public:
	ChildAnimationComponent();
	ChildAnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet,
		std::shared_ptr<AnimationComponent> parent_animation_component);


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
	std::weak_ptr<AnimationComponent> parent_animation_component;
	std::map<std::string, std::shared_ptr<Animation>> animations;
	std::string parents_last_animation;

	void add_animations();

	bool parent_animator_exists = false;

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}