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

sf::Time Timer::restart()
{
	return delta_clock.restart();
}

sf::Time Timer::get_elapsed_time()
{
	return delta_clock.getElapsedTime();
}


}