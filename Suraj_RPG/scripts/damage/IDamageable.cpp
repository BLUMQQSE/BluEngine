#include "../../pch.h"
#include "IDamageable.h"

namespace bm98
{
bool IDamageable::can_take_damage()
{
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