#include "pch.h"
#include "PlayerInteractor.h"
#include "GameObject.h"
#include "Collisions.h"
#include "Physics.h"
#include "Time.h"
#include "IInteractable.h"
namespace bm98
{
/*
using namespace core;

PlayerInteractor::PlayerInteractor()
{
}

PlayerInteractor::~PlayerInteractor()
{
}

void PlayerInteractor::awake()
{
	interactable_mask.add_layer(Layer::INTERACTABLE);
	interactable_mask.add_layer(Layer::ACTOR);
}

void PlayerInteractor::update()
{
	if (failed_interaction)
	{
		if (failed_delta > 2)
		{
			failed_interaction = false;
			update_interactions();
		}
		else
		{
			failed_delta += Time::delta_time();
		}
	}
}

void PlayerInteractor::late_update()
{
}

void PlayerInteractor::fixed_update()
{
}

void PlayerInteractor::update_interactions()
{
	colliders = Physics::OverlapCircle((this->game_object->get_transform().position + this->game_object->get_transform().local_position),
		interaction_radius, interactable_mask, this->game_object);

	if (colliders.size() > 0)
	{
		if (!colliders[0].game_object->has_component<IInteractable>())
			return;
		if (colliders[0].game_object != current_collider.game_object)
		{
			if (current_interactable)
			{
				current_interactable = nullptr;
			}
			
			current_collider = colliders[0];
		}

		current_interactable = &current_collider.game_object->get_component<IInteractable>();

		if (!failed_interaction)
		{
			current_interactable->get_interaction_prompt();
		}

		interacting = current_interactable->is_interacting();

		if (current_interactable != nullptr && (interact || current_interactable->get_player_interaction_on_contact()))
		{
			if (!current_interactable->interact(this))
			{
				current_interactable->get_failed_interaction_promp();
				failed_interaction = true;
				failed_delta = 0;
			}
			else
			{
				failed_interaction = false;
			}
			interact = false;
		}
	}
}

*/
}