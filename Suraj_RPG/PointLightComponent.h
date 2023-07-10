#pragma once
#include "Component.h"
#include "ColliderComponent.h"

namespace bm98
{
class PointLightComponent :
    public Component
{
public:
    PointLightComponent() {}
    virtual ~PointLightComponent() {}

    virtual void init() override;

    virtual void update() override;
    virtual void fixed_update() override;

    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

private:
    float light_distance;

    std::vector<std::weak_ptr<ColliderComponent>> colliders;

};
}
