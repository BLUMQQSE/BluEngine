#include "pch.h"
#include "Inventory.h"
#include "ResourceManager.h"

namespace bm98
{
Inventory::Inventory()
{
	//content = new InventoryItem[size];
}

Inventory::~Inventory()
{
	delete[] content;
}

void Inventory::init()
{
}

void Inventory::awake()
{
}

void Inventory::update()
{
}

Json::Value Inventory::serialize_json()
{
	Json::Value obj;
	obj["size"] = size;
	for (int i = 0; i < size; i++)
	{
		Json::Value obj2;
		obj2["current-capacity"] = content[i].current_capacity;
		if(content[i].current_capacity > 0)
		obj2["item"] = content[i].item.get_name();
		obj2["index"] = i;
		obj["content"].append(obj2);
	}
	return obj;
}

void Inventory::unserialize_json(Json::Value obj)
{
	size = obj["size"].asInt64();
	
	content = new InventoryItem[size];
	
	for (Json::Value item : obj["content"])
	{
		int i = item["index"].asInt64();
		content[i].current_capacity = item["current-capacity"].asInt64();
		if(content[i].current_capacity > 0)
			content[i].item = *dynamic_cast<ItemData*>(core::ResourceManager::Instance()->get_data_asset(item["item"].asString()));
		
	}
}


}