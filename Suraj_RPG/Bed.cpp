#include "pch.h"
#include "Bed.h"
#include "Interactor.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"

namespace bm98
{

void Bed::init()
{
	require_player_input = true;
}

void Bed::initiate_interaction(Interactor * interactor)
{
	if (!IInteractable::check_can_initiate(interactor))
		return;
	if (owner)
		if (owner != interactor->get_game_object())
			return;

	IInteractable::initiate_interaction(interactor);

	// Orient interactor to chest
	interactor->get_game_object()->get_component<RigidbodyComponent>().set_velocity(Vector2f::Zero());

	if (interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		original_time_scale = Time::Instance()->get_time_scale();
		interaction_duration = 5;
		Time::Instance()->set_time_scale(10);
	}

}

void Bed::handle_interaction()
{
	IInteractable::handle_interaction();

}

void Bed::exit_interaction()
{
	if (original_time_scale != INFINITY)
		Time::Instance()->set_time_scale(original_time_scale);
}

Json::Value Bed::serialize_json()
{
	return Json::Value();
}

void Bed::unserialize_json(Json::Value obj)
{
}

}