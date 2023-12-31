#include "../pch.h"
#include "Inventory.h"
#include "../core/ResourceManager.h"
#include "../core/SceneManager.h"
#include "../GameObject.h"
#include "../core/EventSystem.h"
#include "items/ItemController.h"
#include "items/WeaponController.h"

namespace bm98
{
void Inventory::init()
{

}

#pragma region IData

Json::Value Inventory::serialize_json()
{
	Json::Value obj;
	obj["max-size"] = max_size;
	obj["inventory-type"] = InventoryNS::ToString(inventory_type);
	for (int i = 0; i < max_size; i++)
	{
		Json::Value obj2;
		if (content[i].current_capacity > 0)
		{
			obj2["current-capacity"] = content[i].current_capacity;
			obj2["item"] = content[i].item.lock()->get_id();
			obj2["index"] = i;
			obj["content"].append(obj2);
		}
	}
	return obj;
}

void Inventory::unserialize_json(Json::Value obj)
{
	max_size = obj["max-size"].asInt64();
	inventory_type = InventoryNS::ToType(obj["inventory-type"].asString());
	content = std::vector<InventoryItem>(max_size);

	for (int i = 0; i < max_size; i++)
	{
		content[i].current_capacity = 0;
		content[i].item = std::shared_ptr<ItemData>(nullptr);
	}

	for (Json::Value item : obj["content"])
	{
		int i = item["index"].asInt64();
		content[i].current_capacity = item["current-capacity"].asInt64();
		content[i].item = core::ResourceManager::Instance()->get_data_asset<ItemData>(item["item"].asString());
	}
}

#pragma endregion

bool Inventory::check_compatability(int index, std::shared_ptr<ItemData> data)
{
	if (inventory_type != InventoryNS::Type::COMBAT)
		return true;

	if (index != static_cast<int>(data->get_wearable_location()))
		return false;

	return true;
}

int Inventory::get_first_available_index()
{
	for (int i = 0; i < content.size(); i++)
		if (content[i].current_capacity == 0)
			return i;

	return -1;
}

std::vector<int> Inventory::get_all_available_indexes()
{
	std::vector<int> available;

	for (int i = 0; i < content.size(); i++)
		if (content[i].current_capacity == 0)
			available.push_back(i);

	return available;
}

int Inventory::get_first_available_include_match(std::shared_ptr<ItemData> item)
{
	int first_empty_index = -1;

	for (int i = 0; i < content.size(); i++)
	{
		if (content[i].item.lock() == item && content[i].current_capacity < item->get_stackable_limit())
			return i;
		else
			if (first_empty_index == -1 && content[i].current_capacity == 0 && check_compatability(i, item))
				first_empty_index = i;
	}
	return first_empty_index;
}

int Inventory::add_item(int index, std::shared_ptr<ItemData> item, int count)
{
	core::EventSystem::Instance()->push_event(core::EventID::INTERACTION_INVENTORY_UPDATED, nullptr,
											  Caller(Caller::Name::INVENTORY, (void*)this));

	int result = 0;
	if (content[index].current_capacity == 0)
	{
		content[index].item = item;
		if (count > item->get_stackable_limit())
		{
			result = count - item->get_stackable_limit();
			content[index].current_capacity = item->get_stackable_limit();
		}
		else
		{
			content[index].current_capacity += count;
		}
	}
	else
	{

		if (content[index].item.lock() != item)
			return count;

		if (content[index].current_capacity + count > item->get_stackable_limit())
		{
			result = (content[index].current_capacity + count) - item->get_stackable_limit();
			content[index].current_capacity = item->get_stackable_limit();
		}
		else
		{
			content[index].current_capacity += count;
		}

	}

	// we've successfully placed the item, so now instantiate if placed into combat
	if (result == 0 && inventory_type == InventoryNS::Type::COMBAT && item)
	{
		//GameObject* go = new GameObject();
		std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
		if (ResourceManager::Instance()->has_prefab_data(item->get_prefab_file_name()))
		{
			//ItemData* item = (ItemData*) ResourceManager::Instance()->get_data_asset("SwordData.json");
			//WeaponData* weapon = (WeaponData*)item;

			go->unserialize_json(ResourceManager::Instance()->get_prefab_data(item->get_prefab_file_name()));

			ItemController* cont = go->get_component_of_type<ItemController>().lock().get();
			if (cont)
			{
				cont->set_data(item);
			}
		}
		go->get_info().name = ItemNS::ToString(static_cast<ItemNS::WearableLocation>(index));
		go->set_parent(this->game_object->self());
		SceneManager::Instance()->instantiate_gameobject(go);
	}

	return result;
}

std::weak_ptr<ItemData> Inventory::remove_item(int index, int count)
{
	std::weak_ptr<ItemData> item = std::shared_ptr<ItemData>(nullptr);

	if (content[index].current_capacity == 0)
		return item;

	core::EventSystem::Instance()->push_event(core::EventID::INTERACTION_INVENTORY_UPDATED, nullptr,
											  Caller(Caller::Name::INVENTORY, (void*)this));

	item = content[index].item;
	content[index].current_capacity = std::max(0, content[index].current_capacity - count);
	if (content[index].current_capacity == 0)
		content[index].item = std::shared_ptr<ItemData>(nullptr);

	// we've successfully found item we're removing, now destroy gameobject if combat inventory
	if (item.lock() && inventory_type == InventoryNS::Type::COMBAT)
	{
		std::cout << "destroying " << ItemNS::ToString(static_cast<ItemNS::WearableLocation>(index)) << "\n";
		SceneManager::Instance()->destroy_gameobject(std::shared_ptr<GameObject>(this->game_object->get_child(
			ItemNS::ToString(static_cast<ItemNS::WearableLocation>(index)))));
	}

	return item;
}

std::vector<Editor::SerializedVar> Inventory::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;

	values.push_back(Editor::SerializedVar("size", static_cast<void*>(&max_size), Var::Type::Int));
	values.push_back(Editor::SerializedVar("type", static_cast<void*>(&inventory_type), Var::Type::Dropdown, InventoryNS::ToVector()));

	/*
	for (int i = 0; i < max_size; i++)
	{
		if(content[i].current_capacity > 0)
			values.push_back(Editor::SerializedVar("item " + std::to_string(i), static_cast<void*>(&content[i].item->get_name()), Editor::VarType::String));
		else
			values.push_back(Editor::SerializedVar("item " + std::to_string(i), nullptr, Editor::VarType::String));

		values.push_back(Editor::SerializedVar("current_capacity " + std::to_string(i), static_cast<void*>(&content[i].current_capacity), Editor::VarType::Int));
	}
	*/

	return values;
}


}