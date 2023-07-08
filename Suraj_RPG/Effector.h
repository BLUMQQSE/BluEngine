#pragma once
#include "Component.h"
#include "data_assets/DataAssets.h"

#include "ColliderComponent.h"

class ColliderComponent;
namespace bm98
{
class Effector :
    public Component
{
public:
    Effector();
    virtual ~Effector();

    virtual void init() override;

    void apply_effect();

protected:
    std::string effect_data_name;
    std::weak_ptr<EffectData> effect_data;

    std::weak_ptr<ColliderComponent> collider;

};
}
