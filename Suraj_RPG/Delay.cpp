#include "pch.h"
#include "Delay.h"
#include "Time.h"
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
	//sf::Time time = *static_cast<sf::Time*>(event->get_parameter());

	duration += Time::Instance()->delta_time();
	frame_count++;
}

}