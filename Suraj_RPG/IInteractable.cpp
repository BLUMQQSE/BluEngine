#include "pch.h"
#include "IInteractable.h"
#include "Interactor.h"
#include "GameObject.h"
#include "Input.h"
namespace bm98
{

void IInteractable::handle_interaction()
{
	if (interaction_type == Interaction::Type::DURATION)
	{
		timer_as_float = interaction_timer.getElapsedTime().asSeconds();
		if (check_interaction_complete())
		{
			exit_interaction();
			return;
		}
	}
}

void IInteractable::handle_instant_interaction()
{
}

bool IInteractable::check_interaction_complete()
{
	return (interaction_timer.getElapsedTime().asSeconds() >= interaction_duration);
}

void IInteractable::late_update()
{
	if (!busy)
		return;

	handle_interaction();
}

bool IInteractable::check_can_initiate(Interactor* interactor)
{
	if (interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
		if (require_player_input && !Input::Instance()->get_action_down("INTERACT"))
		{
			return false;
		}
	
	return true;
}

void IInteractable::initiate_interaction(Interactor* interactor)
{
	if (!check_can_initiate(interactor))
		return;

	Debug::LogLevel level = Debug::LogLevel::BASIC;
	if (interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
		level = Debug::LogLevel::INFO;
	
	if (interaction_type == Interaction::Type::INSTANT)
	{
		Debug::Instance()->log(interactor->get_game_object()->get_info().name +
			debug_instant_message
			+ game_object->get_info().name, level);
		current_interactor = interactor;
		handle_instant_interaction();
		exit_interaction();
		return;
	}

	Debug::Instance()->log(interactor->get_game_object()->get_info().name + debug_initiate_message
		+ game_object->get_info().name, level);

	busy = true;
	current_interactor = interactor;
	interaction_timer.restart();
}

void IInteractable::exit_interaction()
{
	if (interaction_type != Interaction::Type::INSTANT)
	{
		Debug::LogLevel level = Debug::LogLevel::BASIC;
		if (current_interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
			level = Debug::LogLevel::INFO;

		Debug::Instance()->log(current_interactor->get_game_object()->get_info().name +
			debug_exit_message
			+ game_object->get_info().name, level);
	}
	busy = false;
	if(current_interactor)
		current_interactor->remove_interactable();
	current_interactor = nullptr;
}


#pragma region IData

Json::Value IInteractable::serialize_json()
{
	Json::Value obj;

	obj["interaction-type"] = Interaction::ToString(interaction_type);
	obj["interaction-duration"] = interaction_duration;
	obj["interaction-prompt"] = interaction_prompt;
	obj["failed-interaction-prompt"] = failed_interaction_prompt;
	obj["interaction-priority"] = Interaction::ToString(interaction_priority);
	obj["require-player-input"] = require_player_input;

	return obj;
}

void IInteractable::unserialize_json(Json::Value obj)
{
	interaction_type = Interaction::ToType(obj["interaction-type"].asString());
	interaction_duration = obj["interaction-duration"].asFloat();
	interaction_prompt = obj["interaction-prompt"].asString();
	failed_interaction_prompt = obj["failed-interaction-prompt"].asString();
	interaction_priority = Interaction::ToPriority(obj["interaction-priority"].asString());
	require_player_input = obj["require-player-input"].asBool();

	busy = false;
	current_interactor = nullptr;

}

#pragma endregion

std::vector<Editor::SerializedVar> IInteractable::get_editor_values()
{
	std::vector<Editor::SerializedVar> vals;
	
	vals.push_back(Editor::SerializedVar("prompt", static_cast<void*>(&interaction_prompt), Var::Type::String));
	vals.push_back(Editor::SerializedVar("failed_prompt", static_cast<void*>(&failed_interaction_prompt), 
		Var::Type::String));

	vals.push_back(Editor::SerializedVar("priority", static_cast<void*>(&interaction_priority), Var::Type::Dropdown,
				   Interaction::ToPriorityVector()));
	vals.push_back(Editor::SerializedVar("type", static_cast<void*>(&interaction_type), Var::Type::Dropdown,
				   Interaction::ToVector()));

	vals.push_back(Editor::SerializedVar("busy", static_cast<void*>(&busy), Var::Type::Bool));
	vals.push_back(Editor::SerializedVar("duration", static_cast<void*>(&interaction_duration), Var::Type::Float));
	vals.push_back(Editor::SerializedVar("timer", static_cast<void*>(&timer_as_float), Var::Type::Float));
	priority_as_string = Interaction::ToString(interaction_priority);
	

	return vals;
}


}