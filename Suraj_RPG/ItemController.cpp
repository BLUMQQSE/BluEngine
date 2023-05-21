#include "pch.h"
#include "ItemController.h"
#include "Interactor.h"
#include "GameObject.h"
#include "Inventory.h"
#include "ItemData.h"
#include "core/ResourceManager.h"
#include "core/SceneManager.h"
namespace bm98
{

void ItemController::handle_instant_interaction()
{
	if (item_state != ItemNS::State::DROPPED)
		return;
	if (current_interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		Inventory* inv = current_interactor->get_game_object()->get_child("GeneralInventory").lock()->get_component<Inventory>().lock().get();

		int index = inv->get_first_available_include_match(item_data);
		if (index == -1)
			return;
		//try to put as many into index as possible

		inv->add_item(index, item_data, 1);
		item_state = ItemNS::State::IN_USE;
		//if any remain place into next available position
		SceneManager::Instance()->destroy_gameobject(this->game_object->self());
	}
}

Json::Value ItemController::serialize_json()
{
	Json::Value obj;
	obj[RemoveNamespace(typeid(IInteractable).name())] = IInteractable::serialize_json();

	obj["item-state"] = ItemNS::ToString(item_state);
	obj["item-data"] = item_data->get_file_name();

	return obj;
}

void ItemController::unserialize_json(Json::Value obj)
{
	IInteractable::unserialize_json(obj[RemoveNamespace(typeid(IInteractable).name())]);
	item_state = ItemNS::ToState(obj["item-state"].asString());
	item_data = dynamic_cast<ItemData*>(ResourceManager::Instance()->get_data_asset(obj["item-data"].asString()));
	

}

std::vector<Editor::SerializedVar> ItemController::get_editor_values()
{
	std::vector<Editor::SerializedVar> vals;

	vals = IInteractable::get_editor_values();
	vals.push_back(Editor::SerializedVar("state", static_cast<void*>(&item_state), Var::Type::Dropdown,
		ItemNS::ToVector()));
	if(item_data)
		vals.push_back(Editor::SerializedVar("name", static_cast<void*>(&item_data->get_name()), 
			Var::Type::String));

	return vals;
}



}