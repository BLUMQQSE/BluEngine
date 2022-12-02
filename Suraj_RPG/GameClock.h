#pragma once
#include "globals.h"
namespace bm98
{
enum class Season
{
	SPRING,
	SUMMER,
	FALL,
	WINTER
};

const struct GameTime
{
	GameTime(Season season, int day, int hour, int minute)
	{
		this->season = season;
		this->day = day;
		this->hour = std::min(23, hour);
		this->minute = std::min(59, minute);

	}
	Season season;
	unsigned day;
	unsigned hour;
	unsigned minute;
};

class GameClock
{
public:

	/// <summary>
	/// TODO: add file save and load for initializing variables
	/// </summary>
	static void init();
	static void update();

	static unsigned get_minute();
	static unsigned get_hour();
	/// <summary>
	/// Returns the day count of the game.
	/// </summary>
	static unsigned get_day();
	static std::string get_day_name();
	/// <summary>
	/// Returns day of the current week.
	/// </summary>
	/// <returns>Value ranging from 0-6.</returns>
	static unsigned get_week_day();
	static Season get_season();
	static std::string get_season_name();

private:

	static const std::vector<std::string> week_days;

	static GameTime game_time;
	static float total_game_sec;

	static std::string season_to_string(Season season);

};

}