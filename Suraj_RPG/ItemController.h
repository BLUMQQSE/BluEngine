#pragma once
#include "IInteractable.h"
namespace bm98
{

class ItemData;

class ItemController :
    public IInteractable
{
public:
	ItemController() {}
	~ItemController() {}

	virtual ItemData* get_data() { return item_data; }
	virtual void set_data(ItemData* data) { item_data = data; };

protected:
    ItemNS::State item_state = ItemNS::State::DROPPED;
	ItemData* item_data;

	virtual void handle_instant_interaction() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;
};

}