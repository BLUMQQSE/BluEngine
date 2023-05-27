#pragma once
#include "../globals.h"
#include "../IData.h"
namespace bm98
{
class DataAsset : public IData
{
public:
	DataAsset(std::string id) : id(id) {}
	virtual ~DataAsset() {}

    std::string& get_id() { return id; }
    std::string& get_name() { return name; }

	// Inherited via IData
	virtual Json::Value serialize_json() override;

	virtual void unserialize_json(Json::Value obj) override;


protected:
	std::string id;
    std::string name;
};

class ItemData :
    public DataAsset
{
public:
    ItemData(std::string id) : DataAsset(id) {}
    virtual ~ItemData() {}

    int get_stackable_limit() { return stackable_limit; }
    sf::Texture* get_texture() { return texture_sheet; }
    sf::IntRect& get_rect() { return icon_rect; }
    ItemNS::WearableLocation get_wearable_location() { return wearable_location; }
    inline std::string get_prefab_file_name() { return prefab_file_name; }

    // Inherited via IData
    virtual void unserialize_json(Json::Value obj) override;

protected:
    sf::Texture* texture_sheet;
    sf::IntRect icon_rect;
    std::string texture_file_name;
    std::string prefab_file_name;

    bool tradable = false;
    float worth = 0.0f;
    float cost = 0.0f;

    ItemNS::WearableLocation wearable_location;
    int stackable_limit;


};


class DamagerData;
class WeaponData :
    public ItemData
{

public:
    WeaponData(std::string id) : ItemData(id) { }
    virtual ~WeaponData() {}

    float get_attack_radius() { return attack_radius; }

    // Inherited via IData
    virtual void unserialize_json(Json::Value obj) override;

protected:

    float attack_cooldown = 0.0f;
    bool double_handed = false;

    float attack_radius = 0.0f;
    float attack_cross_section = 0.0f;

    DamagerData* damager_data;

};

class DamagerData : public DataAsset
{
public:

    // Inherited via IData
    virtual void unserialize_json(Json::Value obj) override;

protected:

    PhysicsNS::LayerMask damageable_mask;
    float damage_amount;
    EnumFlag damage_type{ DamageNS::TypeVector() };
    EnumFlag damage_target{ DamageNS::TargetVector() };

};






}