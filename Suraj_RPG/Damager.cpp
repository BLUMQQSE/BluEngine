#include "pch.h"
#include "Damager.h"

#include "core/Physics.h"

#include "GameObject.h"
#include "IDamageable.h"
#include "ColliderComponent.h"

namespace bm98
{

void Damager::fixed_update()
{
	collisions.clear();
	std::vector<std::weak_ptr<ColliderComponent>> cols;
	int count = core::Physics::Instance()->OverlapConvex(hitbox, 
													 damageable_mask, this->game_object, cols);

	if (count == 0)
		return;

	for (int i = 0; i < count; i++)
	{
		if (cols[i].lock()->get_game_object()->has_component_of_type<IDamageable>())
		{
			std::weak_ptr<IDamageable> col_i = cols[i].lock()->get_game_object()->get_component_of_type<IDamageable>();
			collisions.push_back(col_i);
		}
	}
}

void Damager::apply_damage(float amount, DamageNS::Type type, DamageNS::Target target)
{
	for (auto iter = collisions.begin(); iter != collisions.end(); ++iter)
	{
		// check that iter still exists
		if (!iter->expired())
			iter->lock()->take_damage(amount, type, target);
		
	}
}

void Damager::unserialize_json(Json::Value obj)
{
	hitbox.unserialize_json(obj);
}

Json::Value Damager::serialize_json()
{
	return hitbox.serialize_json();
}

}