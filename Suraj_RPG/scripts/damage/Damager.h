#pragma once
#include "../../Component.h"

#include "../../ColliderComponent.h"
#include "../../core/Debug.h"
#include "../../data_assets/DataAssets.h"

namespace bm98
{

class IDamageable;

class Damager :
    public Component
{
public:

    Damager() {}
    virtual ~Damager(){}

    virtual void init() override;
    virtual void editor_update() override;
    /// <summary>
    /// Immediately applies damage to all applicable damageables in damager collider.
    /// </summary>
    /// <param name="amount"></param>
    /// <param name="type"></param>
    /// <param name="target"></param>
    void apply_damage();

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

protected:

    virtual void unserialize_json(Json::Value obj) override;
    virtual Json::Value serialize_json() override;

    std::weak_ptr<ColliderComponent> collider;

    std::string damager_data_name;
    std::weak_ptr<DamagerData> damager_data;

};

}