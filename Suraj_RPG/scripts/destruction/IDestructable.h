#pragma once
#include "../../Component.h"
namespace bm98
{

class IDestructable :
    public Component
{

public:
    IDestructable() {}
    virtual ~IDestructable(){}

    virtual void take_damage(float amount);

private:
    float integrity = 0;

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

};

}
