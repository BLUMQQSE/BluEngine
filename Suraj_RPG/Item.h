#pragma once
#include "IInteractable.h"
namespace bm98
{

class ItemData;

class Item :
    public IInteractable
{
public:
	Item() {}
	~Item() {}

protected:
    ItemNS::State item_state = ItemNS::State::DROPPED;
	ItemData* data;

	virtual void handle_instant_interaction() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;
};

}