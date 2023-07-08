#pragma once
#include "../globals.h"
#include "../IData.h"
#include "../core/EventSystem.h"
namespace bm98
{
using namespace core;
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

class EnemyData : public DataAsset
{
public:
    EnemyData(std::string id) : DataAsset(id) {}
    virtual ~EnemyData() {}

    const float& get_detection_radius() const { return detection_radius; }
    const PhysicsNS::LayerMask& get_target_mask() const { return target_mask; }
    const PhysicsNS::LayerMask& get_obstacle_mask() const { return obstacle_mask; }

    virtual void unserialize_json(Json::Value obj) override;

private:
    float detection_radius;
    PhysicsNS::LayerMask target_mask;
    PhysicsNS::LayerMask obstacle_mask;

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

    std::weak_ptr<DamagerData> damager_data;

};

class DamagerData : public DataAsset
{
public:
    DamagerData(std::string id) : DataAsset(id){}
    virtual ~DamagerData(){}
    // Inherited via IData
    virtual void unserialize_json(Json::Value obj) override;

    const unsigned& get_damage_amount() const { return damage_amount; }
    const unsigned& get_knockback_amount() const { return knockback_amount; }
    const float& get_cooldown() const { return cooldown; }
    const PhysicsNS::LayerMask& get_damageable_mask() const { return damageable_mask; }
    const EnumFlag& get_damage_type() const { return damage_type; }
    const EnumFlag& get_damage_target() const { return damage_target; }

protected:

    PhysicsNS::LayerMask damageable_mask;
    unsigned damage_amount;
    unsigned knockback_amount;
    float cooldown;
    EnumFlag damage_type{ DamageNS::TypeVector() };
    EnumFlag damage_target{ DamageNS::TargetVector() };

};


class EffectData : public DataAsset
{
public:
    EffectData(std::string id) : DataAsset(id){}
    virtual ~EffectData(){}

    virtual void unserialize_json(Json::Value obj) override;

    std::string get_effect_name() { return effect_name; }
    std::string get_effect_description() { return effect_description; }
    bool get_reccuring_effect() { return reccuring_effect; }
    float get_effect_duration() { return effect_duration; }
    float get_intermission_duration() { return intermission_duration; }
    const EnumFlag& get_effect_type() const { return effect_type; }
    const PhysicsNS::LayerMask& get_effect_mask() const { return effect_mask; }
    float get_effect_modifier() { return effect_modifier; }

protected:

    std::string effect_name;
    std::string effect_description;
    bool reccuring_effect;
    float effect_duration;
    // if recurring effect and intermission duration is 0, steadily remove using delta_time
    float intermission_duration;
    EnumFlag effect_type{EffectNS::ToVector()};

    PhysicsNS::LayerMask effect_mask;


    float effect_modifier;


};




}