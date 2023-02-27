#pragma once
#include "Math.h"
#include "Component.h"
namespace bm98
{
class SceneChange :
    public Component
{

public:
	SceneChange();
	virtual ~SceneChange();

	virtual void on_trigger_enter(Collider info) override;
	virtual void on_trigger_stay(Collider info) override;
	virtual void on_trigger_exit(Collider info) override; 

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

private:
	Vector2f destination;
	std::string destination_scene_name;
	bool triggered = false;
};

}