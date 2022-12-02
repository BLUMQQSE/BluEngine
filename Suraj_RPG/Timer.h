#pragma once
#include "Time.h"

namespace bm98
{
class Timer
{
public:
	Timer(float duration);
	virtual ~Timer();

	void update();
	bool timer_complete();
	float duration_left();

private:
	float time_remaining;

};


}