#include "pch.h"
#include "Item.h"
#include "Interactor.h"
#include "GameObject.h"
#include "Inventory.h"
#include "ItemData.h"
#include "ResourceManager.h"
#include "SceneManager.h"
namespace bm98
{

void Item::handle_instant_interaction()
{
	if (item_state != ItemNS::State::DROPPED)
		return;
	if (current_interactor->get_game_object()->get_info().tag == Tags::Tag::PLAYER)
	{
		Inventory* inv = &current_interactor->get_game_object()->get_child("GeneralInventory")->get_component<Inventory>();

		int index = inv->get_first_available_include_match(data);
		if (index == -1)
			return;
		//try to put as many into index as possible

		inv->add_item(index, data, 1);
		item_state = ItemNS::State::IN_USE;
		//if any remain place into next available position
		SceneManager::Instance()->destroy_gameobject(this->game_object);
	}
}

Json::Value Item::serialize_json()
{
	Json::Value obj;
	obj[typeid(IInteractable).name()] = IInteractable::serialize_json();

	obj["item-state"] = ItemNS::ToString(item_state);
	obj["item-data"] = data->get_file_name();

	return obj;
}

void Item::unserialize_json(Json::Value obj)
{
	IInteractable::unserialize_json(obj[typeid(IInteractable).name()]);
	item_state = ItemNS::ToState(obj["item-state"].asString());
	data = dynamic_cast<ItemData*>(ResourceManager::Instance()->get_data_asset(obj["item-data"].asString()));
	

}

std::vector<Editor::SerializedVar> Item::get_editor_values()
{
	std::vector<Editor::SerializedVar> vals;

	vals = IInteractable::get_editor_values();
	vals.push_back(Editor::SerializedVar("state", static_cast<void*>(&item_state), Editor::VarType::Dropdown,
		ItemNS::ToVector()));
	if(data)
		vals.push_back(Editor::SerializedVar("name", static_cast<void*>(&data->get_name()), Editor::VarType::String));

	return vals;
}



}