#pragma once
#include "Component.h"

namespace bm98
{

class IDamageable : public Component
{
public:
	virtual void take_damage(float damage, DamageNS::Type damage_type, 
							 DamageNS::Target damage_target) = 0;

protected:
	IDamageable() {}
	virtual ~IDamageable() {}

};

}