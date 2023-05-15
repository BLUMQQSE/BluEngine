#pragma once
#include "../IData.h"
#include "../GraphicsSettings.h"
#include "../Math.h"
namespace bm98
{
	class State;
}
namespace bm98::core
{

struct AudioSettings
{
	float master_volume = 0.f;
	float music_volume = 0.f;
	float sound_volume = 1.f;
	float ambient_volume = 1.f;
};

struct GameSettings
{
	AudioSettings audio_settings;

	bool draw_gizmos = false;
};

extern bool DEBUG_MODE;

class Game : public IData
{
public:
	Game();
	virtual ~Game();
	void run();

	static GameSettings& get_game_settings();

protected:


private:
	bool successful_shutdown;
	GraphicsSettings graphics_settings;

	sf::RenderWindow* window;
	sf::Event sfevent;
	/// <summary>
	/// No idea why this needs to exist... but loading a sound variable in this file
	/// allows other gameobjects to create sf::Sound variables instantly
	/// </summary>
	sf::Sound this_variable_is_not_used;

	//Renderer* renderer;

	int fps_averager = 0;
	float fps_col;
	float fps;

	static GameSettings game_settings;

	//Input* input_handler;

	std::stack <State*> states;
	int state_count;

	void update_sfml_events();
	void update_delta_time();
	void update();
	void fixed_update();
	void render();

	/// <summary>
	/// Initialize singletons so they can subscribe to their events.
	/// </summary>
	void init_singletons();
	void init_variables();
	void init_graphics_settings();
	void init_window();
	void init_states();
	void end_application();


	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;
	//void check_first_launch();


};

}