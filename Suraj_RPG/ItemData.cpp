#include "pch.h"
#include "ItemData.h"
#include "FileManager.h"
#include "ResourceManager.h"
namespace bm98
{
using namespace core;

void ItemData::unserialize_json(Json::Value obj)
{
	DataAsset::unserialize_json(obj);
	stackable_limit = obj["stackable-limit"].asInt64();
	prefab_file_name = obj["prefab-file-name"].asString();
	wearable_location = ItemNS::ToWearableLocation(obj["wearable-location"].asString());
	
	texture_file_name = obj["texture-file-name"].asString();

	icon_rect.left = obj["icon-rect"]["left"].asInt64() * 32;
	icon_rect.width = (obj["icon-rect"]["width"].asInt64() * 32) + 32;
	icon_rect.top = obj["icon-rect"]["top"].asInt64() * 32;
	icon_rect.height = (obj["icon-rect"]["height"].asInt64() * 32) + 32;

	tradable = obj["tradable"].asBool();
	worth = obj["worth"].asFloat();
	cost = obj["cost"].asFloat();

	texture_sheet = &ResourceManager::Instance()->get_texture(texture_file_name);
	

}
}