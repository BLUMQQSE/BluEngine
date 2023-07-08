#pragma once
#include "../../Component.h"
#include "../../Timer.h"
#include "../../data_assets/DataAssets.h"

namespace bm98
{

class IDamageable 
{
public:
	IDamageable() {}
	virtual ~IDamageable() {}
	bool can_take_damage(std::weak_ptr<GameObject> damager, 
						 std::weak_ptr<DamagerData> damage_data);

	virtual void take_damage(std::weak_ptr<DamagerData> damage_data) = 0;

protected:

	unsigned health;

	Timer invincibility_timer;
	float invincibility_duration = 0.05f;

	//std::unordered_map<std::pair<unsigned long, std::string>, std::pair<float, Timer>> 
	//	active_cooldowns;

	std::unordered_map<std::string, std::pair<float, Timer>>
		active_cooldowns;

	bool invincible = false;



};

}