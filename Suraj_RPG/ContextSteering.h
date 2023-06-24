#pragma once
#include "Component.h"

namespace bm98
{
class EnemyData;

class ContextSteering :
    public Component
{
public:
    ContextSteering(){}
    virtual ~ContextSteering() {}
    virtual void init() override;
    virtual void awake() override;

    virtual void fixed_update() override;
    virtual std::vector<Editor::SerializedVar> get_editor_values() override;

    virtual void unserialize_json(Json::Value obj) override;
    virtual Json::Value serialize_json() override;

private:
    std::string enemy_data_id;
    std::weak_ptr<EnemyData> enemy_data;

    std::vector<std::weak_ptr<GameObject>> targets;
    //std::vector<std::weak_ptr<ColliderComponent>> obstacles;

    PhysicsNS::LayerMask target_mask;
    std::weak_ptr<GameObject> current_target;
    unsigned long current_target_id = 0;

};

}