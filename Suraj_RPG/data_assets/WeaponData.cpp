#include "../pch.h"
#include "WeaponData.h"

namespace bm98
{

void WeaponData::unserialize_json(Json::Value obj)
{
	ItemData::unserialize_json(obj);
	attack_cooldown = obj["attack-cooldown"].asFloat();
	double_handed = obj["double_handed"].asBool();

	attack_radius = obj["attack-radius"].asFloat();
	attack_cross_section = obj["attack-cross-section"].asFloat();
	damage = obj["damage"].asFloat();
}

}