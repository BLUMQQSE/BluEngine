#pragma once
#include "Input.h"
#include "Renderer.h"
namespace bm98::core
{
class Debug
{
public:
	enum class LogLevel
	{
		/// <summary> Message containing info. </summary>
		BASIC = spdlog::level::trace,
		/// <summary> Message acknowledging potential issue. </summary>
		INFO = spdlog::level::info,
		WARNING = spdlog::level::warn,
		FAILURE = spdlog::level::err

	};
	// Returns instance of the Debugger
	static Debug* Instance()
	{
		static Debug instance;
		return &instance;
	}

	void core_log(std::string message, LogLevel level = LogLevel::BASIC);
	void log(std::string message, LogLevel level = LogLevel::BASIC);

	//static void init();
	/// <summary>
	/// Log provided string to the console.
	/// </summary>
	/// <param name="message"></param>
	//static void Log(std::string message);
	//static void LogError(std::string message);
	void mouse_position_display(sf::Font& font, sf::View* view = nullptr);
	void mouse_position_display(sf::Font& font, sf::View* view,
		sf::Vector2f offset);

private:
	
	Debug();
	~Debug() { shutdown(); }
	
	Debug(const EventSystem& rhs)
	{

	}
	Debug& operator=(const EventSystem& rhs) {}
	
	void shutdown() {};

	std::shared_ptr<spdlog::logger> core_logger;
	std::shared_ptr<spdlog::logger> game_logger;

	sf::Text text_display;
	//static bool active;
	//static Sorting::Layer layer;
	//static char z_order;
};
}
