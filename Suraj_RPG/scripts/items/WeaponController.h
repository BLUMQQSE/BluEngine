#pragma once
#include "ItemController.h"
#include "../../data_assets/DataAssets.h"
namespace bm98
{

class WeaponController :
    public ItemController
{

public:
    WeaponController() {}
    virtual ~WeaponController() {}

    virtual std::weak_ptr<ItemData> get_data() override { return weapon_data; }
    virtual void set_data(std::shared_ptr<ItemData> data) override 
    {
        weapon_data = std::static_pointer_cast<WeaponData>(data);
        std::cout << "test to see: " << weapon_data.lock()->get_attack_radius() << "\n";
    };

protected:
    std::weak_ptr<WeaponData> weapon_data;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

};

}