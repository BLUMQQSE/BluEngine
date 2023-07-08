#pragma once
#include "Component.h"
#include "globals.h"
#include "scripts/damage/IDamageable.h"
#include "core/UITagSystem.h"

namespace bm98
{

class DummyAI :
    public IDamageable, public Component
{

public:
    DummyAI();
    virtual ~DummyAI();

    virtual void init() override;

    virtual void update() override;

private:
    std::shared_ptr<TextUITag> name_tag;
    Vector2f name_tag_pos;

    std::pair<Vector2f, float> hit;

    // Inherited via IDamageable
    virtual void take_damage(std::weak_ptr<DamagerData> damage_data) override;
};

}
