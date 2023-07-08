#include "pch.h"
#include "DummyAI.h"

#include "core/UITagSystem.h"

#include "GameObject.h"

namespace bm98
{

using namespace core;


DummyAI::DummyAI()
{
}

DummyAI::~DummyAI()
{
}

void DummyAI::init()
{
	name_tag = core::UITagSystem::Instance()->create_text_tag(game_object->get_world_position(), game_object->get_info().name, 16, INFINITY).lock();
	name_tag->add_action(UITag::Action::FOLLOW, 0, &name_tag_pos);
	this->health = 100;
}

void DummyAI::update()
{
	name_tag_pos = game_object->get_world_position() + Vector2f(10, -10);
}

void DummyAI::take_damage(std::weak_ptr<DamagerData> damage_data)
{
	health -= damage_data.lock()->get_damage_amount();

	if (health <= 0)
	{
		SceneManager::Instance()->destroy_gameobject(game_object->self());
		return;
	}

	std::weak_ptr<TextUITag> t = UITagSystem::Instance()->create_text_tag(game_object->get_visual_center() + Vector2f(0, -32), "-" + std::to_string(damage_data.lock()->get_damage_amount()), 16, 3);
	hit = std::make_pair(Vector2f(0, -1), 4);
	t.lock()->set_fill_color(sf::Color::Red);
	t.lock()->set_outline_color(sf::Color::White);
	t.lock()->add_action(UITag::Action::MOVE_AND_FADE, 0, &hit);
	

}


}