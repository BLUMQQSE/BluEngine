#pragma once
#include "ItemData.h"
#include "Component.h"
namespace bm98
{
class Inventory :
    public Component
{
public:
    struct InventoryItem
    {
        ItemData item;
        int current_capacity;
    };

public:
    Inventory();
    virtual ~Inventory();

    virtual void init() override;
    virtual void awake() override;
    virtual void update() override;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

private:
    //ItemData content[];
    InventoryItem* content;
    int size = 0;
};

}