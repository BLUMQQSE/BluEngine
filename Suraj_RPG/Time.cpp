#include "pch.h"
#include "Time.h"

namespace bm98::core
{

float Time::delta = 0.f;
float Time::time = 0.f;
float Time::time_scale = 1.f;
float Time::time_since_state_change_var = 0.f;
float Time::total_real_delta = 0.f;
float Time::fixed_delta;

Time::Time()
{
}

Time::~Time()
{
}

void Time::init(float total_time)
{
	set_real_time(total_time);
}

void Time::update_delta(const float& dt)
{
	delta = dt;
	total_real_delta += dt;
	fixed_delta += dt;
	time += delta;
	time_since_state_change_var += dt;
}

void Time::reset_fixed_delta()
{
	fixed_delta -= .02f;
}

void Time::apply_time_scale()
{
	delta *= time_scale;
}

void Time::set_time_scale(const float& scale)
{
	time_scale = scale;
}

void Time::reset_time_since_state_change()
{
	time_since_state_change_var = 0.f;
}

const float Time::delta_time()
{
	return delta;
}

const float Time::fixed_delta_time()
{
	return fixed_delta;
}

const float Time::get_time_scale()
{
	return time_scale;
}

const float Time::time_since_state_change()
{
	return time_since_state_change_var;
}

const float Time::time_since_startup()
{
	return time;
}

const float Time::total_real_time()
{
	return total_real_delta;
}

void Time::set_real_time(const float d)
{
	total_real_delta = d;
}

}