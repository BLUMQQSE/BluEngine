#include "../../pch.h"
#include "ItemController.h"
#include "../interaction/Interactor.h"
#include "../../GameObject.h"
#include "../Inventory.h"
#include "../../data_assets/DataAssets.h"
#include "../../core/ResourceManager.h"
#include "../../core/SceneManager.h"
namespace bm98
{

void ItemController::handle_instant_interaction()
{
	if (item_state != ItemNS::State::DROPPED)
		return;
	if (current_interactor.lock()->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		std::weak_ptr<Inventory> inv = current_interactor.lock()->get_game_object()->get_child("GeneralInventory").lock()->get_component<Inventory>();

		int index = inv.lock()->get_first_available_include_match(item_data.lock());
		if (index == -1)
			return;
		//try to put as many into index as possible

		inv.lock()->add_item(index, item_data.lock(), 1);
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
	obj["item-data"] = item_data.lock()->get_id();

	return obj;
}

void ItemController::unserialize_json(Json::Value obj)
{
	IInteractable::unserialize_json(obj[RemoveNamespace(typeid(IInteractable).name())]);
	item_state = ItemNS::ToState(obj["item-state"].asString());
	item_data = ResourceManager::Instance()->get_data_asset<ItemData>(obj["item-data"].asString());
	

}

std::vector<Editor::SerializedVar> ItemController::get_editor_values()
{
	std::vector<Editor::SerializedVar> vals;

	vals = IInteractable::get_editor_values();
	vals.push_back(Editor::SerializedVar("state", static_cast<void*>(&item_state), Var::Type::Dropdown,
		ItemNS::ToVector()));
	if(item_data.lock())
		vals.push_back(Editor::SerializedVar("name", static_cast<void*>(&item_data.lock()->get_name()),
			Var::Type::String));

	return vals;
}



}