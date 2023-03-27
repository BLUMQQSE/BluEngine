#include "pch.h"
#include "Interactor.h"
#include "Physics.h"
#include "GameObject.h"
#include "IInteractable.h"
namespace bm98
{


void Interactor::interact()
{
	if (potential_interactable == nullptr)
		return;

	if (!potential_interactable->check_can_initiate(this))
		return;

	if (potential_interactable->get_type() != Interaction::Type::INSTANT)
	{
		current_interactable = potential_interactable;
		interacting = true;
	}
	potential_interactable->initiate_interaction(this);
	potential_interactable = nullptr;
	
}

void Interactor::interact(IInteractable* interactable)
{
	if (interactable == nullptr)
		return;
	
	if (!interactable->check_can_initiate(this))
	{
		std::cout << "ERROR::INTERACTOR::interact(IInteractable* interactor):: COULD NOT INITIATE interactable\n";
	}
	if (current_interactable)
		current_interactable->exit_interaction();

	if (interactable->get_type() != Interaction::Type::INSTANT)
	{
		current_interactable = interactable;
		interacting = true;
	}

	interactable->initiate_interaction(this);

}

void Interactor::cancel_interaction()
{
	if (!current_interactable)
		return;

	current_interactable->exit_interaction();
}

void Interactor::fixed_update()
{
	potential_interactable = nullptr;
	std::vector<ColliderComponent*> cols;
	count = core::Physics::Instance()->OverlapCircle(game_object->get_world_position(), interaction_radius,
		interactable_mask, this->game_object, cols);

	if (count == 0)
		return;

	// need to reduce cols to only include gameobjects including IInteractable
	IInteractable* interactable = nullptr;
	for (int i = 0; i < count; i++)
	{
		if (cols[i]->get_game_object()->has_component_of_type<IInteractable>())
		{
			IInteractable* col_i = cols[i]->get_game_object()->get_component_of_type<IInteractable>();
			if (interactable == nullptr)
				interactable = col_i;
			else if (!col_i->is_busy() && (interactable->get_priority() < col_i->get_priority()))
				interactable = col_i;
		}
	}

	if (interactable == nullptr)
		return;

	if (current_interactable)
	{
		if (current_interactable == interactable)
			return;
		if (current_interactable->get_priority() >= interactable->get_priority())
			return;
	}

	potential_interactable = interactable;

}

void Interactor::remove_interactable()
{
	current_interactable = nullptr;
	interacting = false;
}

#pragma region IData

Json::Value Interactor::serialize_json()
{
	Json::Value obj;

	obj["interaction-radius"] = interaction_radius;

	for (auto mask : PhysicsNS::LayerMask::ToVector(interactable_mask.layers))
	{
		if (mask.second)
		{
			Json::Value obj2;
			obj2["layer"] = mask.first;
			obj["interactable-mask"].append(obj2);
		}
	}

	return obj;
}

void Interactor::unserialize_json(Json::Value obj)
{

	interaction_radius = obj["interaction-radius"].asFloat();

	for (Json::Value i : obj["interactable-mask"])
		interactable_mask.add_layer(PhysicsNS::ToLayer(i["layer"].asString()));

}

#pragma endregion

std::vector<Editor::SerializedVar> Interactor::get_editor_values()
{
	std::vector<Editor::SerializedVar> vars;

	vars.push_back(Editor::SerializedVar("count", static_cast<void*>(&count), Editor::VarType::Int));
	vars.push_back(Editor::SerializedVar("radius", static_cast<void*>(&interaction_radius), Editor::VarType::Float));
	vars.push_back(Editor::SerializedVar("interacting", static_cast<void*>(&interacting), Editor::VarType::Bool));

	return vars;
}


}