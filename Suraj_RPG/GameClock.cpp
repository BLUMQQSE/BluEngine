#include "pch.h"
#include "GameClock.h"
#include "core/Time.h"
#include "Gui.h"
#include "core/ResourceManager.h"
namespace bm98
{

using namespace core;

#pragma region GameClock

void GameClock::init()
{
	set_day(3);
	set_hour(23);
	set_minute(45);
}

void GameClock::update()
{
	total_game_sec += Time::Instance()->delta_time() * GAME_TIME_MULTIPLIER;

	unsigned min = game_time.minute;
	unsigned hour = game_time.hour;
	unsigned day = game_time.day;
	GameTime::DayPart part = game_time.part;

	game_time.minute = static_cast<int>(std::floor(total_game_sec)) % 60;
	game_time.hour = static_cast<int>(std::floor(total_game_sec)) / 60 % 24;
	game_time.day = static_cast<int>(std::floor(total_game_sec)) / 60 / 24;

	check_push_events(day, hour, min);

}

void GameClock::set_day(unsigned day)
{
	if (day > game_time.day)
		total_game_sec += (day * 24 * 60);
	else
		total_game_sec -= ((game_time.day - day) * 24 * 60);

	//game_time.day = std::floor(total_game_sec / 60 / 24);
}

void GameClock::set_hour(unsigned hour)
{
	unsigned temp = game_time.hour;

	if (hour > game_time.hour)
		total_game_sec += (hour * 60);
	else
		total_game_sec -= ((game_time.hour - hour) * 60);

	//game_time.hour = std::floor(static_cast<int>(total_game_sec) / 60 % 24);

	//check_push_events(game_time.day, temp, game_time.minute);
}

void GameClock::set_minute(unsigned minute)
{
	if (minute > game_time.minute)
		total_game_sec += minute;
	else
		total_game_sec -= (game_time.minute - minute);

	//game_time.minute = std::floor(static_cast<int>(total_game_sec) % 60);

}

void GameClock::check_push_events(unsigned day, unsigned hour, unsigned min)
{
	if (min != game_time.minute)
		EventSystem::Instance()->push_event(EventID::GAMECLOCK_MIN_CHANGE, (void*)&game_time,
											Caller(Caller::Name::GAMECLOCK, (void*)this));
	if (hour != game_time.hour)
	{
		bool day_part_change = false;
		if (game_time.hour >= 19 && game_time.part != GameTime::DayPart::EVENING) //evening 7pm -12am
		{
			game_time.part = GameTime::DayPart::EVENING;
			day_part_change = true;
		}
		else if (game_time.hour >= 15 && game_time.hour < 19
				 && game_time.part != GameTime::DayPart::AFTERNOON) // afternoon 3pm - 7pm
		{
			game_time.part = GameTime::DayPart::AFTERNOON;
			day_part_change = true;
		}
		else if (game_time.hour >= 10 && game_time.hour < 15 &&
				 game_time.part != GameTime::DayPart::MIDDAY) //midday 10am - 3pm
		{
			game_time.part = GameTime::DayPart::MIDDAY;
			day_part_change = true;
		}
		else if (game_time.hour >= 6 && game_time.hour < 10
				 && game_time.part != GameTime::DayPart::MORNING) // morning 6am - 10am
		{
			game_time.part = GameTime::DayPart::MORNING;
			day_part_change = true;
		}
		else if (game_time.hour >= 0 && game_time.hour < 6 
				 && game_time.part != GameTime::DayPart::OVERNIGHT) // overnight 12am - 6am
		{
			game_time.part = GameTime::DayPart::OVERNIGHT;
			day_part_change = true;
		}

		if (day_part_change)
			EventSystem::Instance()->push_event(EventID::GAMECLOCK_DAYPART_CHANGE, (void*)&game_time,
												Caller(Caller::Name::GAMECLOCK, (void*)this));

		EventSystem::Instance()->push_event(EventID::GAMECLOCK_HOUR_CHANGE, (void*)&game_time,
											Caller(Caller::Name::GAMECLOCK, (void*)this));
	}
	if (day != game_time.day)
		EventSystem::Instance()->push_event(EventID::GAMECLOCK_DAY_CHANGE, (void*)&game_time,
											Caller(Caller::Name::GAMECLOCK, (void*)this));
}

}

#pragma endregion

#pragma region GameClockDisplay


void bm98::GUI::GameClockDisplay::init()
{
	EventSystem::Instance()->subscribe(EventID::GAMECLOCK_MIN_CHANGE, this);
	EventSystem::Instance()->subscribe(EventID::GAMECLOCK_DAY_CHANGE, this);

	clock_panel = std::make_unique<Panel>(10, 10, 200, 60, sf::Color(180, 180, 180, 220));

	Label* day_label = 
		new Label(0, 0, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Day: " + GameClock::Instance()->get_day_name());
	Label* time_label =
		new Label(100, 0, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Time: " + GameClock::Instance()->get_time_formatted());
	Label* season_label = 
		new Label(0, 20, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Season: " + GameClock::Instance()->get_season_name());
	Label* daypart_label =
		new Label(0, 40, 16, ResourceManager::Instance()->get_font("calibri-regular.ttf"), "Daypart: " + GameClock::Instance()->get_daypart_name());

	clock_panel->add_element("day", day_label);
	clock_panel->add_element("time", time_label);
	clock_panel->add_element("season", season_label);
	clock_panel->add_element("daypart", daypart_label);

}

void bm98::GUI::GameClockDisplay::update_clock()
{
	dynamic_cast<Label*>(clock_panel->get_element("day"))->set_text("Day: " + GameClock::Instance()->get_day_name());
	dynamic_cast<Label*>(clock_panel->get_element("time"))->set_text("Time: " + GameClock::Instance()->get_time_formatted());
	dynamic_cast<Label*>(clock_panel->get_element("season"))->set_text("Season: " + GameClock::Instance()->get_season_name());
	dynamic_cast<Label*>(clock_panel->get_element("daypart"))->set_text("DayPart: " + GameClock::Instance()->get_daypart_name());
}

void bm98::GUI::GameClockDisplay::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
		case EventID::GAMECLOCK_MIN_CHANGE:
			update_clock();
			break;
		case EventID::GAMECLOCK_DAY_CHANGE:
			GameTime g = *static_cast<GameTime*>(event->get_parameter());
			break;
	}
}



#pragma endregion






