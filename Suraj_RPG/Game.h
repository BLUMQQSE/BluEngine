#pragma once
#include "IData.h"
#include "GraphicsSettings.h"
namespace bm98
{
	class State;
}
namespace bm98::core
{

class Game : IData
{

public:
	Game();
	virtual ~Game();
	void run();

protected:


private:
	bool successful_shutdown;

	bool in_dev_window = false;

	GraphicsSettings graphics_settings;
	GraphicsSettings dev_graphics_settings;

	sf::RenderWindow* window;
	sf::RenderWindow* dev_window;
	sf::Event sfevent;


	//Renderer* renderer;

	sf::Clock deltaClock;
	float fixed_delta_timer;
	int fps_averager = 0;
	float fps_col;
	float fps;

	//Input* input_handler;

	std::stack <State*> states;
	int state_count;

	void update_sfml_events();
	void update_delta_time();

	void update();

	void render();

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