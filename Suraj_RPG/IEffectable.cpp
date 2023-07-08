#include "pch.h"
#include "IEffectable.h"

namespace bm98
{



IEffectable::IEffectable()
{
}

IEffectable::~IEffectable()
{
}

void IEffectable::take_effect(EffectData effect_data)
{
}

bool IEffectable::can_take_effect()
{
	return true;
}


}