#pragma once
#include "Component.h"

#include "GameObject.h"
#include "core/Debug.h"

namespace bm98
{

class IDamageable;

class Damager :
    public Component
{
public:

    Damager();
    virtual ~Damager();

    virtual void fixed_update() override;

    void set_hitbox(FloatConvex& hitbox) { this->hitbox = hitbox; }
    void apply_damage(float amount, DamageNS::Type type,
                      DamageNS::Target target);

private:

    virtual void unserialize_json(Json::Value obj) override;
    virtual Json::Value serialize_json() override;

    FloatConvex& hitbox;
    PhysicsNS::LayerMask damageable_mask;
    //std::vector<IDamageable*> collisions;
    std::vector<std::weak_ptr<IDamageable>> collisions;

};

}