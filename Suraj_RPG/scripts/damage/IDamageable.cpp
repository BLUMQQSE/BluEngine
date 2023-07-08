#include "../../pch.h"
#include "IDamageable.h"
#include "../../GameObject.h"
#include "../../Timer.h"

namespace bm98
{
bool IDamageable::can_take_damage(std::weak_ptr<GameObject> damager, std::weak_ptr<DamagerData> damage_data)
{
	std::unordered_map<std::string, std::pair<float, Timer>>::iterator it = active_cooldowns.begin();
	
	while (it != active_cooldowns.end())
	{
		if (it->second.second.get_elapsed_time().asSeconds() >= it->second.first)
			it = active_cooldowns.erase(it);
		else
			++it;
	}

	std::string pair = std::to_string(damager.lock()->get_info().unique_id) + damage_data.lock()->get_id();
	if (active_cooldowns.find(pair) != active_cooldowns.end())
		return false;
	else
	{
		// add to map
		active_cooldowns[pair] = std::make_pair(damage_data.lock()->get_cooldown(), Timer());
	}

	if (invincible)
	{
		return false;
	}
	else if (invincibility_timer.get_elapsed_time().asSeconds() > invincibility_duration)
	{
		invincibility_timer.restart();
		return true;
	}

	return false;
}

}