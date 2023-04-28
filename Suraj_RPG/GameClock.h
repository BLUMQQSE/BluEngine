#pragma once
#include "globals.h"
#include "core/Debug.h"
namespace bm98
{

class GameTime
{
#define GAME_TIME_MULTIPLIER .8f
public:
#pragma region SEASON

	enum class Season
	{
		SPRING,
		SUMMER,
		FALL,
		WINTER
	};

	static std::vector<std::string> ToVector()
	{
		return { "SPRING", "SUMMER", "FALL", "WINTER" };
	}

	static std::string ToString(Season season)
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

		core::Debug::Instance()->log("[GameClock] SEASON COULD NOT BE CONVERTED TO STRING",
									 core::Debug::LogLevel::WARNING);
		return "SPRING";
	}

	static Season ToSeason(std::string season)
	{
		if (season == "SPRING")
			return Season::SPRING;
		if (season == "SUMMER")
			return Season::SUMMER;
		if (season == "FALL")
			return Season::FALL;
		if (season == "WINTER")
			return Season::WINTER;

		core::Debug::Instance()->log("[GameClock] STRING COULD NOT BE CONVERTED TO SEASON",
									 core::Debug::LogLevel::WARNING);
		return Season::SPRING;
	}

#pragma endregion

#pragma region DAYPART

	enum class DayPart
	{
		MORNING,
		MIDDAY,
		AFTERNOON,
		EVENING,
		OVERNIGHT
	};

	static std::string ToString(DayPart part)
	{
		switch (part)
		{
			case DayPart::MORNING:
				return "MORNING";
			case DayPart::MIDDAY:
				return "MIDDAY";
			case DayPart::AFTERNOON:
				return "AFTERNOON";
			case DayPart::EVENING:
				return "EVENING";
			case DayPart::OVERNIGHT:
				return "OVERNIGHT";
		}

		core::Debug::Instance()->log("[GameClock] DAYPART" + std::to_string((int)part) + 
									 "COULD NOT BE CONVERTED TO STRING",
									 core::Debug::LogLevel::WARNING);
		return "MORNING";
	}

	static DayPart ToDayPart(std::string part)
	{
		if (part == "MORNING")
			return DayPart::MORNING;
		if (part == "MIDDAY")
			return DayPart::MIDDAY;
		if (part == "AFTERNOON")
			return DayPart::AFTERNOON;
		if (part == "EVENING")
			return DayPart::EVENING;
		if (part == "OVERNIGHT")
			return DayPart::OVERNIGHT;

		core::Debug::Instance()->log("[GameClock] STRING COULD NOT BE CONVERTED TO DAYPART",
									 core::Debug::LogLevel::WARNING);
		return DayPart::MORNING;

	}

#pragma endregion


	GameTime()
	{
		this->season = Season::SPRING;
		this->part = DayPart::MORNING;
		this->day = 0;
		this->hour = 0;
		this->minute = 0;
	}
	GameTime(Season season, DayPart part, int day, int hour, int minute)
	{
		this->season = season;
		this->part = part;
		this->day = day;
		this->hour = std::min(23, hour);
		this->minute = std::min(59, minute);

	}
	Season season;
	DayPart part;
	unsigned day;
	unsigned hour;
	unsigned minute;
};

class GameClock
{
public:
	// Returns instance of the EventSystem
	static GameClock* Instance()
	{
		static GameClock instance;
		return &instance;
	}
	/// <summary>
	/// TODO: add file save and load for initializing variables
	/// </summary>
	void init();
	void update();

	inline unsigned get_minute() { return game_time.minute; }
	inline unsigned get_hour() { return game_time.hour; }
	
	inline unsigned get_day() { return game_time.day; }
	inline std::string get_day_name() { return week_days[get_week_day()]; }
	/// <summary>
	/// Returns day of the current week.
	/// </summary>
	/// <returns>Value ranging from 0-6.</returns>
	inline unsigned get_week_day() { return game_time.day % 7; }
	inline GameTime::Season get_season() { return game_time.season; }
	inline std::string get_season_name(){ return GameTime::ToString(get_season()); }

	inline GameTime::DayPart get_daypart() { return game_time.part; }
	inline std::string get_daypart_name() { return GameTime::ToString(get_daypart()); }
	std::string get_time_formatted()
	{
		std::string hour;
		std::string min = "";
		std::string suffix = "am";

		if (game_time.minute < 10)
			min = "0";
		min.append(std::to_string(game_time.minute));
		unsigned game_hour = game_time.hour;
		if (game_hour == 0)
			game_hour = 12;
		else if (game_hour == 12)
		{
			suffix = "pm";
		}
		else if (game_hour > 12)
		{
			game_hour -= 12;
			suffix = "pm";
		}
		hour = std::to_string(game_hour);

		return hour + ":" + min + suffix;
	}

private:
	GameClock() {}
	~GameClock() {}
	GameClock(const GameClock& rhs)
	{

	}
	GameClock& operator=(const GameClock& rhs) {}
	
	void set_day(unsigned day);
	void set_hour(unsigned hour);
	void set_minute(unsigned minute);

	void check_push_events(unsigned day, unsigned hour, unsigned min);

	
	const std::vector<std::string> week_days = 
	{ "Moon", "Ares", "Hermes", "Zues", "Aphrodite", "Kronos", "Sun" };

	GameTime game_time;
	double total_game_sec;


};

namespace GUI
{

class Panel;
using namespace core;
class GameClockDisplay : public Listener
{
public:
	static GameClockDisplay* Instance()
	{
		static GameClockDisplay instance;
		return &instance;
	}

	void init();

private:
	GameClockDisplay() {}
	~GameClockDisplay() { delete clock_panel; }
	GameClockDisplay(const GameClockDisplay& rhs)
	{

	}
	GameClockDisplay& operator=(const GameClockDisplay& rhs) {}
	
	void update_clock();
	

	// Inherited via Listener
	virtual void handle_event(Event* event) override;

	Panel* clock_panel;
	

};

}


}