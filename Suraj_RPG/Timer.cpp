#include "pch.h"
#include "Timer.h"

namespace bm98
{



Timer::Timer()
{
	delta_clock.restart();
}

Timer::~Timer()
{

}

float Timer::restart()
{
	return delta_clock.restart().asSeconds() * 1000;
}

float Timer::get_elapsed_time()
{
	return delta_clock.getElapsedTime().asSeconds() * 1000;
}


}