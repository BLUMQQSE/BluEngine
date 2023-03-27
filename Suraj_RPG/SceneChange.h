#pragma once
#include "Math.h"
#include "IInteractable.h"
namespace bm98
{
class SceneChange :
    public IInteractable
{

public:
	SceneChange() {}
	virtual ~SceneChange() {}	

protected:
	Vector2f destination;
	std::string destination_scene_name;

	virtual void handle_instant_interaction() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;
};

}