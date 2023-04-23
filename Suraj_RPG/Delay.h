#pragma once
#include "EventSystem.h"

namespace bm98
{
using namespace core;

class Delay : public Listener
{
public:
	Delay();
	virtual ~Delay(){}

	void reset();

	/// <returns>Number of frames since last reset.</returns>
	int get_count() { return frame_count; }
	/// <returns>Duration in seconds since last reset.</returns>
	inline float get_duration() { return duration; }

private:
	
	int frame_count = 0;
	float duration = 0;

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}
