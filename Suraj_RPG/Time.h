#pragma once
#include "IData.h"

namespace bm98::core
{

class Time : IData
{
public:

	Time();
	virtual ~Time();

	static void init(float total_time);
	static void update_delta(const float& dt);
	static void reset_fixed_delta();
	static void apply_time_scale();
	static void set_time_scale(const float& scaler);
	static void reset_time_since_state_change();

	const static float delta_time();
	const static float fixed_delta_time();
	const static float get_time_scale();
	const static float time_since_state_change();
	const static float time_since_startup();

	const static float total_real_time();


private:
	static float delta;
	static float fixed_delta;
	static float time;
	static float time_scale;
	static float time_since_state_change_var;
	static float total_real_delta;


	static void set_real_time(const float d);


};

}