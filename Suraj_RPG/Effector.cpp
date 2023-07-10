#include "pch.h"
#include "Effector.h"
#include "GameObject.h"
#include "core/Physics.h"
#include "IEffectable.h"

namespace bm98
{



Effector::Effector()
{
}

Effector::~Effector()
{
}

void Effector::init()
{
	effect_data = ResourceManager::Instance()->get_data_asset<EffectData>(effect_data_name);
	this->collider = game_object->get_component_of_type<ColliderComponent>();
}

void Effector::apply_effect()
{
	if (!effect_data.lock())
		return;

	std::vector<std::weak_ptr<ColliderComponent>> cols;
	int count = core::Physics::Instance()->OverlapConvex(collider.lock()->get_collider_bounds(),
														 effect_data.lock()->get_effect_mask(), this->game_object, Physics::CollisionCheckType::ALL, cols);

	if (count == 0)
		return;

	for (int i = 0; i < count; i++)
	{
		if (cols[i].lock()->get_game_object()->has_component_with_interface<IEffectable>())
		{

			IEffectable* col_i = cols[i].lock()->get_game_object()->get_component_with_interface<IEffectable>();

			if (col_i->can_take_effect())
				col_i->take_effect(*effect_data.lock().get());
		}
	}
}


}