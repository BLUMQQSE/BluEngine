#include "../../pch.h"
#include "Interactor.h"
#include "../../core/Physics.h"
#include "../../GameObject.h"
#include "IInteractable.h"
namespace bm98
{


void Interactor::interact()
{
	if (potential_interactable.expired())
		return;

	if (!potential_interactable.lock()->check_can_initiate(std::static_pointer_cast<Interactor>(this->self())))
	{
	//	Debug::Instance()->log(game_object->get_info().name + " COULD NOT INITIATE INTERACTION WITH "
	//		+ potential_interactable->get_game_object()->get_info().name, Debug::LogLevel::WARNING);
		return;
	}
	if (potential_interactable.lock()->get_type() != Interaction::Type::INSTANT)
	{
		current_interactable = potential_interactable;
		interacting = true;
	}

	//Debug::Instance()->log(game_object->get_info().name + " INITIATING INTERACTION WITH "
	//	+ potential_interactable->get_game_object()->get_info().name);

	potential_interactable.lock()->initiate_interaction(std::static_pointer_cast<Interactor>(this->self()));
	potential_interactable = std::weak_ptr<IInteractable>();
	
}

void Interactor::interact(std::weak_ptr<IInteractable> interactable)
{
	if (!interactable.lock())
		return;
	
	if (!interactable.lock()->check_can_initiate(std::static_pointer_cast<Interactor>(this->self())))
	{
		Debug::Instance()->log(game_object->get_info().name + " COULD NOT INITIATE INTERACTION WITH "
			+ interactable.lock()->get_game_object()->get_info().name, Debug::LogLevel::WARNING);
		return;
	}
	if (!current_interactable.expired())
		current_interactable.lock()->exit_interaction();

	if (interactable.lock()->get_type() != Interaction::Type::INSTANT)
	{
		current_interactable = interactable;
		interacting = true;
	}

	Debug::Instance()->log(game_object->get_info().name + " INITIATING INTERACTION WITH "
		+ interactable.lock()->get_game_object()->get_info().name);
	interactable.lock()->initiate_interaction(std::static_pointer_cast<Interactor>(this->self()));

}

void Interactor::cancel_interaction()
{
	if (current_interactable.expired())
		return;

	current_interactable.lock()->exit_interaction();
}

void Interactor::fixed_update()
{
	potential_interactable = std::weak_ptr<IInteractable>();
	std::vector<std::weak_ptr<ColliderComponent>> cols;
	count = core::Physics::Instance()->OverlapCircle(game_object->get_center(), interaction_radius,
		interactable_mask, this->game_object, cols);

	if (count == 0)
		return;

	// need to reduce cols to only include gameobjects including IInteractable
	std::weak_ptr<IInteractable> interactable;
	for (int i = 0; i < count; i++)
	{
		if (cols[i].lock()->get_game_object()->has_component_of_type<IInteractable>())
		{
			std::weak_ptr<IInteractable> col_i = cols[i].lock()->get_game_object()->get_component_of_type<IInteractable>();
			if (!interactable.lock())
				interactable = col_i;
			else if (!col_i.lock()->is_busy() && (interactable.lock()->get_priority() < col_i.lock()->get_priority()))
				interactable = col_i;
		}
	}

	if (!interactable.lock())
		return;

	if (current_interactable.lock())
	{
		if (current_interactable.lock() == interactable.lock())
			return;
		if (current_interactable.lock()->get_priority() >= interactable.lock()->get_priority())
			return;
	}

	potential_interactable = interactable;

}

void Interactor::on_draw_gizmos()
{
	if (count > 0)
		Gizmo::outline_color = Color::Red;
	else
		Gizmo::outline_color = Color::Cyan;

	Gizmo::draw_circle(game_object->get_center(), interaction_radius);
	Gizmo::draw_text(game_object->get_center() + Vector2f(0, interaction_radius), "[" +
					 std::to_string(game_object->get_info().unique_id) + "] Interactor", 10, Color::White);
}

void Interactor::remove_interactable()
{
	current_interactable = std::weak_ptr<IInteractable>();
	interacting = false;
}

#pragma region IData

Json::Value Interactor::serialize_json()
{
	Json::Value obj;

	obj["interaction-radius"] = interaction_radius;
	obj["interactable-mask"] = interactable_mask.serialize_field();
	
	return obj;
}

void Interactor::unserialize_json(Json::Value obj)
{

	interaction_radius = obj["interaction-radius"].asFloat();

	interactable_mask.unserialize_field(obj["interactable-mask"]);

}

#pragma endregion

std::vector<Editor::SerializedVar> Interactor::get_editor_values()
{
	std::vector<Editor::SerializedVar> vars;

	vars.push_back(Editor::SerializedVar("count", static_cast<void*>(&count), Var::Type::Int));
	vars.push_back(Editor::SerializedVar("radius", static_cast<void*>(&interaction_radius), Var::Type::Float));
	vars.push_back(Editor::SerializedVar("interacting", static_cast<void*>(&interacting), Var::Type::Bool));
	vars.push_back(Editor::SerializedVar("mask", static_cast<void*>(&interactable_mask), Var::Type::FlagDropdown));

	return vars;
}


}