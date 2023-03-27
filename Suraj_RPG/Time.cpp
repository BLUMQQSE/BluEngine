#include "pch.h"
#include "Time.h"

namespace bm98::core
{
Json::Value Time::serialize_json()
{
	return Json::Value();
}

void Time::unserialize_json(Json::Value obj)
{

}

void Time::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::_SYSTEM_TIME_INITIALIZE_:
		Instance()->init(*static_cast<float*>(event->get_parameter()));
		break;
	case EventID::_SYSTEM_TIME_APPLY_SCALE_:
		Instance()->apply_time_scale();
		break;
	case EventID::_SYSTEM_TIME_RESET_SINCE_STATE_CHANGE_:
		Instance()->reset_time_since_state_change();
		break;
	case EventID::_SYSTEM_TIME_UPDATE_:
		Instance()->update_delta(*static_cast<float*>(event->get_parameter()));
		break;
	case EventID::_SYSTEM_TIME_RESET_FIXED_:
		Instance()->reset_fixed_delta();
		break;

	}
}

void Time::init(float total_time)
{
	set_real_time(total_time);
	
	
}

void Time::update_delta(const float dt)
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
	return Instance()->delta;
}

const float Time::fixed_delta_time()
{
	return Instance()->fixed_delta;
}

const float Time::get_time_scale()
{
	return Instance()->time_scale;
}

const float Time::time_since_state_change()
{
	return Instance()->time_since_state_change_var;
}

const float Time::time_since_startup()
{
	return Instance()->time;
}

const float Time::total_real_time()
{
	return Instance()->total_real_delta;
}

Time::Time()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_UPDATE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_APPLY_SCALE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_RESET_FIXED_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_RESET_SINCE_STATE_CHANGE_, this);
}

void Time::set_real_time(const float d)
{
	Instance()->total_real_delta = d;
}

}