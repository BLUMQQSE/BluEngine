#include "pch.h"
#include "ItemData.h"
#include "FileManager.h"
namespace bm98
{
using namespace core;
ItemData::ItemData()
{
}

ItemData::ItemData(std::string name)
{
	this->name = name;
	file_path = "Data/DataAssets/Items/";
	FileManager::load_from_file(file_path + name);
}

ItemData::~ItemData()
{
	// SHOULDNT NEED TO DESTRUCT ANYTHING
}

int ItemData::get_stackable_limit()
{
	return stackable_limit;
}

Json::Value ItemData::serialize_json()
{
	// SHOULDNT NEED TO SERIALIZE ANYTHING
	return Json::Value();
}

void ItemData::unserialize_json(Json::Value obj)
{
	//texture_sheet.loadFromFile(file_path + obj["icon"].asString());
	//icon_rect.left = obj["icon-rect.left"].asInt64();
	///icon_rect.top = obj["icon-rect.top"].asInt64();
	//icon_rect.width = obj["icon-rect.width"].asInt64();
	//icon_rect.height = obj["icon-rect.height"].asInt64();
	DataAsset::unserialize_json(obj);
	stackable_limit = obj["stackable-limit"].asInt64();
}
}