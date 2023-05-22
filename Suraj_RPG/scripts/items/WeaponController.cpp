#include "../../pch.h"
#include "WeaponController.h"

namespace bm98
{

Json::Value WeaponController::serialize_json()
{
	return ItemController::serialize_json();
}

void WeaponController::unserialize_json(Json::Value obj)
{
	ItemController::unserialize_json(obj);
}


}