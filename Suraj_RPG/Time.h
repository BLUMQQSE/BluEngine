#pragma once
#include "EventSystem.h"
#include "IData.h"

namespace bm98::core
{

class Time : public IData, public Listener
{
public:
	// Returns instance of the EventSystem
	static Time* Instance()
	{
		static Time instance;
		return &instance;
	}

	void set_time_scale(const float& scaler);

	const float delta_time();
	const float fixed_delta_time();
	const float get_time_scale();
	const float time_since_state_change();
	const float time_since_startup();

	const float total_real_time();


	// Inherited via IData
	virtual Json::Value serialize_json() override;

	virtual void unserialize_json(Json::Value obj) override;

private:
	Time();
	~Time() {  }
	Time(const Time& rhs)
	{

	}
	Time& operator=(const Time& rhs) {}


	float delta = 0.f;
	float fixed_delta = 0.f;
	float time = 0.f;
	float time_scale = 1.f;
	float time_since_state_change_var = 0.f;
	float total_real_delta = 0.f;


	void init(float total_time);
	void set_real_time(const float d); 
	void update_delta(const float dt);
	void reset_time_since_state_change();
	void reset_fixed_delta();
	void apply_time_scale();


	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}