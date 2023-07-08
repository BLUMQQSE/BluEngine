#include "../pch.h"
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
	case EventID::_SYSTEM_TIME_RESET_SINCE_STATE_CHANGE_:
		Instance()->reset_time_since_state_change();
		break;
	case EventID::_SYSTEM_TIME_UPDATE_:
		Instance()->update_delta(game_timer.restart().asSeconds());
		Instance()->update_fps_average();
		break;
	case EventID::_SYSTEM_TIME_RESET_FIXED_:
		Instance()->reset_fixed_delta();
		break;
	case EventID::_SYSTEM_TIME_FIXED_UPDATE_:
		update_fixed_delta();
		break;
	}
}

void Time::init(float total_time)
{
	set_real_time(total_time);
}

void Time::update_delta(const float dt)
{
	total_real_delta += dt;
	time += delta;
	time_since_state_change_var += dt;

	delta = dt * time_scale;
}

void Time::reset_fixed_delta()
{
	fixed_delta -= .01f;
}

void Time::update_fps_average()
{
	if (fps_averager == 50)
	{
		fps_averager = 0;
		fps = (unsigned)(fps_col / 50);
		fps_col = 0;
	}
	else
	{
		fps_averager++;
		fps_col += 1000 / (Time::Instance()->delta_time() * 1000);
	}
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

void Time::update_fixed_delta()
{
	if (game_fixed_timer.get_elapsed_time().asSeconds() >= FIXED_INTERVAL)
	{
		fixed_delta = game_fixed_timer.restart().asSeconds();
	}
}

Time::Time()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_UPDATE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_FIXED_UPDATE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_RESET_FIXED_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_TIME_RESET_SINCE_STATE_CHANGE_, this);
}

void Time::set_real_time(const float d)
{
	total_real_delta = d;
}

}