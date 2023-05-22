#include "pch.h"
#include "Destructor.h"
#include "IDestructable.h"
#include "ColliderComponent.h"
#include "GameObject.h"
#include "core/Physics.h"

namespace bm98
{



void Destructor::destruct()
{
	std::vector<std::weak_ptr<ColliderComponent>> cols;
	int count = core::Physics::Instance()->OverlapCircle(game_object->get_world_position(), max_distance,
		damage_mask, this->game_object, cols);

	if (count == 0)
		return;

	for (int i = 0; i < count; i++)
	{
		if (cols[i].lock()->get_game_object()->has_component_of_type<IDestructable>())
		{
			Vector2f distance = cols[i].lock()->get_game_object()->get_world_position() -
				this->game_object->get_world_position();

			float true_damage;
			if (distance.sqr_magnitude() < max_distance * max_distance)
				continue;
			else if (distance.sqr_magnitude() < min_distance * min_distance)
				true_damage = damage;
			else
			{
				float dist_past_min = distance.sqr_magnitude() - (min_distance * min_distance);
				int units = dist_past_min / 32;
				true_damage = damage - (units * attentuation);
			}

			cols[i].lock()->get_game_object()->get_component_of_type<IDestructable>().lock()->take_damage(true_damage);
		}
	}

}

Json::Value Destructor::serialize_json()
{
	Json::Value obj;

	obj["damage"] = damage;
	obj["attentuation"] = attentuation;
	obj["max-distance"] = max_distance;
	obj["min-distance"] = min_distance;
	
	obj["damage-mask"] = damage_mask.serialize_field();

	return obj;
}

void Destructor::unserialize_json(Json::Value obj)
{
	damage_mask.unserialize_field(obj["damage-mask"]);
}


}