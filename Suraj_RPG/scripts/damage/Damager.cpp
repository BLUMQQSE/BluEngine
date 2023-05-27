#include "../../pch.h"
#include "Damager.h"

#include "../../core/Physics.h"

#include "../../GameObject.h"
#include "IDamageable.h"
#include "../../ColliderComponent.h"

namespace bm98
{
void Damager::init()
{
	this->collider = game_object->get_component_of_type<ColliderComponent>();
}
void Damager::fixed_update()
{
	collisions.clear();
	std::vector<std::weak_ptr<ColliderComponent>> cols;
	int count = core::Physics::Instance()->OverlapConvex(collider.lock()->get_collider_bounds(),
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

void Damager::editor_update()
{
}

void Damager::apply_damage_to_all()
{
	for (auto iter = collisions.begin(); iter != collisions.end(); ++iter)
	{
		// check that iter still exists
		if (!iter->expired())
			iter->lock()->take_damage(damage_amount, damage_type, damage_target);
		
	}
}

std::vector<Editor::SerializedVar> Damager::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;
	
	values.push_back(Editor::SerializedVar("mask", (void*)&damageable_mask, Var::Type::FlagDropdown, PhysicsNS::ToVector()));
	values.push_back(Editor::SerializedVar("damage", (void*)&damage_amount, Var::Type::Float));
	values.push_back(Editor::SerializedVar("type", (void*)&damage_type, Var::Type::FlagDropdown, DamageNS::TypeVector()));
	values.push_back(Editor::SerializedVar("target", static_cast<void*>(&damage_target), Var::Type::FlagDropdown, DamageNS::TargetVector()));

	return values;
}

void Damager::unserialize_json(Json::Value obj)
{	
	damageable_mask.unserialize_field(obj["damageable-mask"]);
	damage_amount = obj["damage-amount"].asFloat();
	damage_type.unserialize_field(obj["damage-type"]);
	
	damage_target.unserialize_field(obj["damage-target"]);
}

Json::Value Damager::serialize_json()
{
	Json::Value obj;

	obj["damageable-mask"] = damageable_mask.serialize_field();
	obj["damage-amount"] = damage_amount;
	obj["damage-target"] = damage_target.serialize_field();
	obj["damage-type"] = damage_type.serialize_field();
	return obj;
}

}