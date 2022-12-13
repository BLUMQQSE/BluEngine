#pragma once
#include "Component.h"
namespace bm98
{
class Inventory :
    public Component
{
public:
    Inventory();
    virtual ~Inventory();

    virtual void init() override;
    virtual void awake() override;
    virtual void update() override;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;
};

}