#pragma once
#include "DataAsset.h"
#include "globals.h"
namespace bm98
{
class ItemData :
    public DataAsset
{
public:
    ItemData() {}
    virtual ~ItemData() {}

    int get_stackable_limit() { return stackable_limit; }
    sf::Texture* get_texture() { return texture_sheet; }
    sf::IntRect& get_rect() { return icon_rect; }
    ItemNS::WearableLocation get_wearable_location() { return wearable_location; }
    inline std::string get_prefab_file_name() { return prefab_file_name; }

   

protected:
    sf::Texture* texture_sheet; 
    sf::IntRect icon_rect;
    std::string texture_file_name;
    std::string prefab_file_name;

    ItemNS::WearableLocation wearable_location;
    int stackable_limit;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

};

}