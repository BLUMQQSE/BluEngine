#include "pch.h"
#include "Timer.h"

namespace bm98
{
using namespace core;
Timer::Timer(float duration)
{
	time_remaining = duration;
}

Timer::~Timer()
{
}

void Timer::update()
{
	time_remaining -= Time::delta_time();
}

bool Timer::timer_complete()
{
	return time_remaining <= 0;
}

float Timer::duration_left()
{
	return time_remaining;
}
}