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
			IDamageable* col_i = cols[i]->get_game_object()->get_component_of_type<IDamageable>();
			collisions.push_back(col_i);
		}
	}

}

void Damager::apply_damage(float amount)
{
	std::vector<IDamageable*>::iterator iter;
	while (iter != collisions.end())
	{
		(*iter)->take_damage(amount);
	}

}

}