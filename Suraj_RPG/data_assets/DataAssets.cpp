#include "../pch.h"
#include "DataAssets.h"
#include "../core/FileManager.h"
#include "../core/ResourceManager.h"
#include "../core/Debug.h"

namespace bm98
{

#pragma region DataAsset

Json::Value DataAsset::serialize_json()
{
    return Json::Value();
}

void DataAsset::unserialize_json(Json::Value obj)
{
	name = obj["name"].asString();
}

#pragma endregion

#pragma region EnemyData

void EnemyData::unserialize_json(Json::Value obj)
{
	detection_radius = obj["pathfinding"]["detection-radius"].asFloat();
	target_mask.unserialize_field(obj["pathfinding"]["target-mask"]);
	obstacle_mask.unserialize_field(obj["pathfinding"]["obstacle-mask"]);
}

#pragma endregion


#pragma region ItemData

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

#pragma endregion

#pragma region WeaponData

void WeaponData::unserialize_json(Json::Value obj)
{
	ItemData::unserialize_json(obj);
	attack_cooldown = obj["attack-cooldown"].asFloat();
	double_handed = obj["double_handed"].asBool();

	attack_radius = obj["attack-radius"].asFloat();
	attack_cross_section = obj["attack-cross-section"].asFloat();

	std::string damager_name = obj["damager-data"].asString();
	if (core::ResourceManager::Instance()->has_data_asset(damager_name))
	{
		//damager_data = static_cast<DamagerData*>(core::ResourceManager::Instance()->get_data_asset(damager_name));
		damager_data = core::ResourceManager::Instance()->get_data_asset<DamagerData>(damager_name);
	}
	else
		core::Debug::Instance()->core_log("Failed to find DamagerData " + damager_name, core::Debug::LogLevel::WARNING);
	//damager_data->unserialize_json(obj["damager-data"]);
}

#pragma endregion

#pragma region DamagerData

void DamagerData::unserialize_json(Json::Value obj)
{
	damage_amount = obj["damage-amount"].asUInt64();
	knockback_amount = obj["knockback-amount"].asUInt64();
	cooldown = obj["damage-cooldown"].asFloat();
	damageable_mask.unserialize_field(obj["damageable-mask"]);
	damage_type.unserialize_field(obj["damage-type"]);
	damage_target.unserialize_field(obj["damage-target"]);
}

#pragma endregion

#pragma region EffectData

void EffectData::unserialize_json(Json::Value obj)
{
	effect_name = obj["effect-name"].asString();
	effect_description = obj["effect-description"].asString();
	effect_duration = obj["effect-duration"].asFloat();
	effect_type.unserialize_field(obj["effect-type"]);

	reccuring_effect = obj["reccuring-effect"].asBool();
	intermission_duration = obj["intermission-duration"].asFloat();
	effect_modifier = obj["effect-modifier"].asFloat();
}

#pragma endregion

}