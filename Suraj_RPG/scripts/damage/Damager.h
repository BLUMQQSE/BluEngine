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
    /// <summary>
    /// Retrieves all IDamageables within the damager's collider.
    /// </summary>
    virtual void fixed_update() override;
    virtual void editor_update() override;
    /// <summary>
    /// Immediately applies
    /// </summary>
    /// <param name="amount"></param>
    /// <param name="type"></param>
    /// <param name="target"></param>
    void apply_damage_to_all();

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

protected:

    virtual void unserialize_json(Json::Value obj) override;
    virtual Json::Value serialize_json() override;

    std::weak_ptr<ColliderComponent> collider;

    PhysicsNS::LayerMask damageable_mask;

    float damage_amount = 1;
    DamageNS::Type damage_type = DamageNS::Type::DEFAULT;
    //DamageNS::Target damage_target = DamageNS::Target::ALL;
    
    EnumFlag damage_target{ DamageNS::TargetVector() };
    
    std::vector<std::weak_ptr<IDamageable>> collisions;

};

}