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
	name_tag = core::UITagSystem::Instance()->create_text_tag(game_object->get_world_position(), "Dummy", 16, INFINITY);
	name_tag->add_action(UITag::Action::FOLLOW, 0, &name_tag_pos);
}

void DummyAI::update()
{
	name_tag_pos = game_object->get_world_position() + Vector2f(10, -10);
}


}