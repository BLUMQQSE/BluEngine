#pragma once
#include "../interaction/IInteractable.h"
namespace bm98
{

class ItemData;

class ItemController :
    public IInteractable
{
public:
	ItemController() {}
	~ItemController() {}

	virtual std::weak_ptr<ItemData> get_data() { return item_data; }
	virtual void set_data(std::shared_ptr<ItemData> data) { item_data = data; };

protected:
    ItemNS::State item_state = ItemNS::State::DROPPED;
	std::weak_ptr<ItemData> item_data;

	virtual void handle_instant_interaction() override;
	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	virtual std::vector<Editor::SerializedVar> get_editor_values() override;
};

}