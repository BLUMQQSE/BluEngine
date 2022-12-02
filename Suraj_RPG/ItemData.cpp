#include "pch.h"
#include "ItemData.h"
#include "FileManager.h"
namespace bm98
{
using namespace core;
ItemData::ItemData(std::string name)
{
	this->name = name;
	file_path = "Data/DataAssets/Items/";
	FileManager::load_from_file(file_path + name, true);
}

ItemData::~ItemData()
{
	// SHOULDNT NEED TO DESTRUCT ANYTHING
}

Json::Value ItemData::serialize_json()
{
	// SHOULDNT NEED TO SERIALIZE ANYTHING
	return Json::Value();
}

void ItemData::unserialize_json(Json::Value obj)
{
	texture_sheet.loadFromFile(file_path + obj["icon"].asString());
	icon_rect.left = obj["icon_rect.left"].asInt64();
	icon_rect.top = obj["icon_rect.top"].asInt64();
	icon_rect.width = obj["icon_rect.width"].asInt64();
	icon_rect.height = obj["icon_rect.height"].asInt64();

	stackable_limit = obj["stackable_limit"].asInt64();
}
}