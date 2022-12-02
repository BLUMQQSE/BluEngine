#include "pch.h"
#include "GameClock.h"
#include "Time.h"
namespace bm98
{
using namespace core;
GameTime GameClock::game_time = GameTime(Season::FALL, 10, 0, 0);
float GameClock::total_game_sec = 0;
const std::vector<std::string> GameClock::week_days =
{ "Moon", "Ares", "Hermes", "Zues", "Aphrodite", "Kronos", "Sun" };



void GameClock::init()
{
}

void GameClock::update()
{
	total_game_sec += Time::delta_time();

	game_time.minute = std::floor(static_cast<int>(total_game_sec) % 60);
	game_time.hour = std::floor(static_cast<int>(total_game_sec) / 60 % 24);
	game_time.day = std::floor(total_game_sec / 60 / 24);

}

unsigned GameClock::get_minute()
{
	return game_time.minute;
}

unsigned GameClock::get_hour()
{
	return game_time.hour;
}

unsigned GameClock::get_day()
{
	return game_time.day;
}

std::string GameClock::get_day_name()
{
	return week_days[get_week_day()];
}

unsigned GameClock::get_week_day()
{
	return game_time.day % 7;
}

Season GameClock::get_season()
{
	return game_time.season;
}

std::string GameClock::get_season_name()
{
	return season_to_string(get_season());
}

std::string GameClock::season_to_string(Season season)
{
	switch (season)
	{
	case Season::SPRING:
		return "SPRING";
	case Season::SUMMER:
		return "SUMMER";
	case Season::FALL:
		return "FALL";
	case Season::WINTER:
		return "WINTER";
	}
}
}