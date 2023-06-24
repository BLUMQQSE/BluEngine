#pragma once
#include "../../Component.h"

#include "../../ColliderComponent.h"
#include "../../GameObject.h"
#include "../../core/Debug.h"

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
    virtual void update() override;
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

    PhysicsNS::LayerMask damageable_mask;

    unsigned damage_amount = 1;
    EnumFlag damage_type{ DamageNS::TypeVector() };
    //DamageNS::Target damage_target = DamageNS::Target::ALL;
    
    EnumFlag damage_target{ DamageNS::TargetVector() };
    
    std::vector<std::weak_ptr<IDamageable>> collisions;

};

}