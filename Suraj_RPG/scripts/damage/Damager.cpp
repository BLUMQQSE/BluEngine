#include "../../pch.h"
#include "Damager.h"

#include "../../core/Physics.h"

#include "../../GameObject.h"
#include "IDamageable.h"

namespace bm98
{
void Damager::init()
{
	damager_data = ResourceManager::Instance()->get_data_asset<DamagerData>(damager_data_name);
	this->collider = game_object->get_component_of_type<ColliderComponent>();
}

void Damager::editor_update()
{
	damager_data = ResourceManager::Instance()->get_data_asset<DamagerData>(damager_data_name);
}

void Damager::apply_damage()
{
	if (!damager_data.lock())
		return;

	std::vector<std::weak_ptr<ColliderComponent>> cols;
	int count = core::Physics::Instance()->OverlapConvex(collider.lock()->get_collider_bounds(),
													 damager_data.lock()->get_damageable_mask(), this->game_object, cols);

	if (count == 0)
		return;

	for (int i = 0; i < count; i++)
	{
		if (cols[i].lock()->get_game_object()->has_component_with_interface<IDamageable>())
		{

			IDamageable* col_i = cols[i].lock()->get_game_object()->get_component_with_interface<IDamageable>();

			if (col_i->can_take_damage(this->game_object, damager_data))
			{
				col_i->take_damage(damager_data);
				// apply knockback to col_i gameobject
				if (damager_data.lock()->get_knockback_amount() > 0)
				{
					Vector2f knockback_dir = cols[i].lock()->get_game_object()->get_physical_center() - game_object->get_physical_center();
					Vector2f::Normalize(knockback_dir);
					cols[i].lock()->get_game_object()->get_component<RigidbodyComponent>().lock()->apply_external_force(knockback_dir * 10.f);
					//knockback_dir.x *= damager_data.lock()->get_knockback_amount();
					//knockback_dir.y *= damager_data.lock()->get_knockback_amount();
					//cols[i].lock()->get_game_object()->move(knockback_dir);
				}
			}
		}
	}

}

std::vector<Editor::SerializedVar> Damager::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;
	
	values.push_back(Editor::SerializedVar("damager_data_name", (void*)&damager_data_name, Var::Type::String));

	if (damager_data.lock())
	{
		values.push_back(Editor::SerializedVar("DamagerData", nullptr, Var::Type::Header));
		values.push_back(Editor::SerializedVar("damage_amount", (void*)&damager_data.lock()->get_damage_amount(), Var::Type::Int, false));
		values.push_back(Editor::SerializedVar("damage_type", (void*) &damager_data.lock()->get_damage_type(), Var::Type::FlagDropdown, false));
		values.push_back(Editor::SerializedVar("damage_target", (void*)&damager_data.lock()->get_damage_target(), Var::Type::FlagDropdown, false));
		values.push_back(Editor::SerializedVar("damage_mask", (void*)&damager_data.lock()->get_damageable_mask(), Var::Type::FlagDropdown, false));
	}

	return values;
}

void Damager::unserialize_json(Json::Value obj)
{	
	damager_data_name = obj["damager-data"].asString();
}

Json::Value Damager::serialize_json()
{
	Json::Value obj;

	obj["damager-data"] = damager_data_name;
	return obj;
}

}