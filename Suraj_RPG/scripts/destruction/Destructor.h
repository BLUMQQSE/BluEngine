#pragma once
#include "../../Component.h"
namespace bm98
{

class IDestructable;

class Destructor :
    public Component
{
public:
    Destructor() {}
    virtual ~Destructor() {}

    virtual void destruct();

private:
    float damage = 0;
    /// <summary>Amount damage decreases per 1 UNIT past min_distance</summary>
    int attentuation = 0;
    float max_distance = 0;
    float min_distance = 0;
    PhysicsNS::LayerMask damage_mask = PhysicsNS::LayerMask();

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

};

}
