#pragma once
#include "DataAsset.h"
namespace bm98
{
class ItemData :
    public DataAsset
{
public:
    ItemData(std::string name);
    virtual ~ItemData();



    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

protected:
    std::string file_path;

    sf::Sprite icon;
    sf::Texture texture_sheet;
    sf::IntRect icon_rect;

    int stackable_limit;

};

}