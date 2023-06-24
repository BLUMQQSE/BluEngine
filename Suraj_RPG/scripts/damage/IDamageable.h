#pragma once
#include "../../Component.h"
#include "../../Timer.h"

namespace bm98
{

class IDamageable : public Component
{
public:

	bool can_take_damage();

	virtual void take_damage(unsigned damage, EnumFlag damage_type, 
							 EnumFlag damage_target) = 0;

protected:
	IDamageable() {}
	virtual ~IDamageable() {}

	unsigned health;

	Timer invincibility_timer;
	float invincibility_duration = 0.05f;
	bool invincible = false;



};

}