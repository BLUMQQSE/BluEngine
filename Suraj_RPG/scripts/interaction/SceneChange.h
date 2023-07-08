#pragma once
#include "../../Math.h"
#include "IInteractable.h"
namespace bm98
{
class SceneChange :
    public IInteractable
{

public:
	struct Destination
	{
		Vector2f position;
		std::string scene_name;
	};

	SceneChange() {}
	virtual ~SceneChange() {}	

		


protected:
	Destination destination;

	virtual void handle_instant_interaction() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;

};

}