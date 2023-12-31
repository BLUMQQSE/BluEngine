#include "../../pch.h"
#include "Chest.h"
#include "Interactor.h"
#include "../../GameObject.h"
#include "../InventoryGUIController.h"
#include "../InventoryWindow.h"
#include "../../core/Input.h"
#include "../../SpriteComponent.h"

namespace bm98
{

void Chest::init()
{
	debug_initiate_message = " OPENED CHEST ";
	debug_exit_message = " CLOSED CHEST ";
}

void Chest::initiate_interaction(std::shared_ptr<Interactor> interactor)
{
	if (!IInteractable::check_can_initiate(interactor))
		return;

	IInteractable::initiate_interaction(interactor);

	// Orient interactor to chest
	interactor->get_game_object()->get_component<RigidbodyComponent>().lock()->set_velocity(0.f);
	interactor->get_game_object()->get_component<RigidbodyComponent>().lock()->set_orientation(
		Orientation::VectorToDirection(this->game_object->get_visual_center() - interactor->get_game_object()->get_visual_center()));

	if (interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		// TODO: play "open_chest" animation
		game_object->get_component<SpriteComponent>().lock()->get_sprite().setTextureRect(sf::IntRect(0, 32, 32, 32));

		// TODO: play "open_chest" audio

		interactor->get_game_object()->get_component<InventoryGUIController>().lock()->set_external(game_object->get_component<InventoryWindow>().lock());
		interactor->get_game_object()->get_component<InventoryGUIController>().lock()->toggle_inventory(InventoryNS::WindowToggle::OPEN_ALL);
	}
	input_delay.reset();

}

void Chest::handle_interaction()
{
	IInteractable::handle_interaction();

	if ((core::Input::Instance()->get_action_down("INVENTORY") || core::Input::Instance()->get_action_down("INTERACT"))
		&& input_delay.get_count() > 0)
	{
		exit_interaction();
	}
}

void Chest::exit_interaction()
{


	if (current_interactor.lock()->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		// TODO: play "close_chest" animation
		game_object->get_component<SpriteComponent>().lock()->get_sprite().setTextureRect(sf::IntRect(0, 0, 32, 32));
		// TODO: play "close_chest" audio



		current_interactor.lock()->get_game_object()->get_component<InventoryGUIController>().lock()->toggle_inventory(InventoryNS::WindowToggle::CLOSE_ALL);
		current_interactor.lock()->get_game_object()->get_component<InventoryGUIController>().lock()->remove_external();
	}

	IInteractable::exit_interaction();
}



Json::Value Chest::serialize_json()
{
	Json::Value obj;

	obj[RemoveNamespace(typeid(IInteractable).name())] = IInteractable::serialize_json();

	return obj;
}

void Chest::unserialize_json(Json::Value obj)
{
	IInteractable::unserialize_json(obj[RemoveNamespace(typeid(IInteractable).name())]);
}



}