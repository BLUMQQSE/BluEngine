#pragma once
#include "ItemData.h"

namespace bm98
{

class WeaponData :
    public ItemData
{
public:
    WeaponData() {}
    virtual ~WeaponData() {}

    float get_attack_radius() { return attack_radius; }

protected:
    
    float attack_cooldown = 0.0f;
    bool double_handed = false;

    float attack_radius = 0.0f;
    float attack_cross_section = 0.0f;
    float damage = 0.0f;

    // Inherited via IData
    virtual void unserialize_json(Json::Value obj) override;

};

}
