#pragma once
#include "data_assets/DataAssets.h"

namespace bm98
{

class IEffectable
{
public:
	IEffectable();
	virtual ~IEffectable();

	virtual void take_effect(EffectData effect_data);

	bool can_take_effect();

protected:


};

}
