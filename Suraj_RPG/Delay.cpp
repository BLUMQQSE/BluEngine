#include "pch.h"
#include "Delay.h"

namespace bm98
{

bm98::Delay::Delay()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_UPDATE_, this);
}

void bm98::Delay::reset()
{
	duration = 0;
	frame_count = 0;
}

void bm98::Delay::handle_event(Event* event)
{
	float delta = *static_cast<float*>(event->get_parameter());

	duration += delta;
	frame_count++;
}

}