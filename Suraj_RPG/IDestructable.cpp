#include "pch.h"
#include "IDestructable.h"

void bm98::IDestructable::take_damage(float amount)
{
	integrity -= amount;
}

Json::Value bm98::IDestructable::serialize_json()
{
	Json::Value obj;

	obj["integrity"] = integrity;

	return obj;
}

void bm98::IDestructable::unserialize_json(Json::Value obj)
{
	integrity = obj["integrity"].asFloat();
}
