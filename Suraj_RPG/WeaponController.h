#pragma once
#include "ItemController.h"
#include "WeaponData.h"
namespace bm98
{

class WeaponController :
    public ItemController
{

public:
    WeaponController() {}
    virtual ~WeaponController() {}

    virtual ItemData* get_data() override { return weapon_data; }
    virtual void set_data(ItemData* data) override 
    {
        weapon_data = (WeaponData*) data;
        std::cout << "test to see: " << weapon_data->get_attack_radius() << "\n";
    };

protected:
    WeaponData* weapon_data;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

};

}