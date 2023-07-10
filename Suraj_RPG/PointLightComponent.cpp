#include "pch.h"
#include "PointLightComponent.h"
#include "GameObject.h"
#include "core/Physics.h"

namespace bm98
{



void PointLightComponent::init()
{
}

void PointLightComponent::update()
{
}

void PointLightComponent::fixed_update()
{
	PhysicsNS::LayerMask mask{true};
	core::Physics::Instance()->OverlapCircle(game_object->get_physical_center(), light_distance,
											 mask, this->game_object, core::Physics::CollisionCheckType::COLLIDER, colliders);

	// then here would check each collider for meta data about whether it blocks light
	// for now assume all colliders block light
	std::vector<std::weak_ptr<ColliderComponent>>::iterator it;
	for (it = colliders.begin(); it != colliders.end(); ++it)
	{
		if (!it->lock()->blocks_light())
		{
			it = colliders.erase(it);
			continue;
		}



	}
}

Json::Value PointLightComponent::serialize_json()
{
	return Json::Value();
}

void PointLightComponent::unserialize_json(Json::Value obj)
{
}


}