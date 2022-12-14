#include "pch.h"
#include "Inventory.h"
namespace bm98
{
Inventory::Inventory()
{
	//content = new InventoryItem[size];
}

Inventory::~Inventory()
{
	//delete[] content;
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
		obj2["item"] = content[i].item.serialize_json();
		obj["content"].append(obj2);
	}
	return obj;
}

void Inventory::unserialize_json(Json::Value obj)
{
	size = obj["size"].asInt64();
	int i = 0;
	for (Json::Value item : obj["content"])
	{
		content[i].current_capacity = item["current-capacity"].asInt64();
		content[i].item.unserialize_json(item["item"]);
		//obj["content"].append(content[i].item.serialize_json());
		i++;
	}
}


}