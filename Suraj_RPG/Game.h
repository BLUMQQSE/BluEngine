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

	void update_sfml_events();
	void update_delta_time();

	void update();

	void render();
	void run();

protected:


private:
	bool successful_shutdown;

	GraphicsSettings graphics_settings;
	sf::RenderWindow* window;
	sf::Event sfevent;


	//Renderer* renderer;

	sf::Clock deltaClock;
	float fixed_delta_timer;
	float fps;

	//Input* input_handler;

	std::stack <State*> states;
	int state_count;

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