#pragma once
#include "globals.h"

namespace bm98
{
class Timer
{
public:
	Timer();
	virtual ~Timer();

	/// <summary>
	/// Resets the timer and returns the elapsed time since last restart in milliseconds.
	/// </summary>
	sf::Time restart();
	/// <summary>
	/// Returns the elapsed time since last restart in milliseconds without resetting the
	/// clock.
	/// </summary>
	sf::Time get_elapsed_time();

private:
	sf::Clock delta_clock;

};

}
