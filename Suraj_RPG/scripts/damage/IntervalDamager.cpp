#include "../../pch.h"
#include "IntervalDamager.h"
#include "IDamageable.h"

namespace bm98
{
IntervalDamager::IntervalDamager()
{
}
IntervalDamager::~IntervalDamager()
{
}
void IntervalDamager::update()
{
	if (!damager_data.lock())
		return;

	int i = 0;
	for (auto& hd : hit_delay)
	{
		if (hd.first == 0)
		{
			// damageable just entered, should get hit
			//collisions.at(i).lock()->take_damage(damage_amount, damage_type, damage_target);
			hd.first++;
		}
		else
		{
			// damageable is waiting for timer to complete
			if (hd.second.get_elapsed_time().asSeconds() >= hit_interval)
			{
				hd.second.restart();
				//collisions.at(i).lock()->take_damage(damage_amount, damage_type, damage_target);
				hd.first++;
			}
		}
		i++;
	}
}

void IntervalDamager::fixed_update()
{
	Damager::fixed_update();
	//hit_delay.resize(collisions.size());
}

std::vector<Editor::SerializedVar> IntervalDamager::get_editor_values()
{
	std::vector<Editor::SerializedVar> values;
	values = Damager::get_editor_values();

	values.push_back(Editor::SerializedVar("interval", (void*)&hit_interval, Var::Type::Float));

	return values;
}

Json::Value IntervalDamager::serialize_json()
{
	Json::Value obj;

	obj[RemoveNamespace(typeid(Damager).name())] = Damager::serialize_json();
	obj["hit-interval"] = hit_interval;

	return obj;
}

void IntervalDamager::unserialize_json(Json::Value obj)
{
	Damager::unserialize_json(RemoveNamespace(typeid(Damager).name()));
	hit_interval = obj["hit-interval"].asFloat();
}

}
