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
	std::vector<ColliderComponent*> cols;
	int count = core::Physics::Instance()->OverlapConvex(hitbox, 
													 damageable_mask, this->game_object, cols);

	if (count == 0)
		return;

	IDamageable* damageable = nullptr;
	for (int i = 0; i < count; i++)
	{
		if (cols[i]->get_game_object()->has_component_of_type<IDamageable>())
		{
			IDamageable* col_i = cols[i]->get_game_object()->get_component_of_type<IDamageable>().lock().get();
			collisions.push_back(col_i->get_game_object());
		}
	}

}

void Damager::apply_damage(float amount, DamageNS::Type type, DamageNS::Target target)
{
	auto iter = collisions.begin();
	while (iter != collisions.end())
	{
		// check that iter still exists
		if (iter->expired())
		{
			iter = collisions.erase(iter);
		}
		if((*iter).lock()->has_component<IDamageable>())
			(*iter).lock()->get_component<IDamageable>().lock()->take_damage(amount, type, target);
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