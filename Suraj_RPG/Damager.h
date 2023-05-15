#pragma once
#include "Component.h"

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
    void apply_damage(float amount);

private:

    virtual void unserialize_json(Json::Value obj) override;
    virtual Json::Value serialize_json() override;

    FloatConvex& hitbox;
    PhysicsNS::LayerMask damageable_mask;
    std::vector<IDamageable*> collisions;

};

}