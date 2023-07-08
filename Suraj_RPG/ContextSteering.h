#pragma once
#include "Component.h"
#include "ColliderComponent.h"

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

    virtual void on_draw_gizmos() override;

    virtual void unserialize_json(Json::Value obj) override;
    virtual Json::Value serialize_json() override;

private:
    std::string enemy_data_id;
    std::weak_ptr<EnemyData> enemy_data;

   //std::vector<std::weak_ptr<GameObject>> targets;
    std::vector<std::weak_ptr<ColliderComponent>> obstacles;

    std::weak_ptr<GameObject> current_target;
    unsigned long current_target_id = 0;

    bool target_in_range = false;
    bool object_unblocked = false;

    // obstacle avoidance
    std::array<float, 8> danger;
    //float radius = 5.f;
    // largest this, the more serious it takes dangers?
    float obstacle_buffer_space = 50.f;
    
    // steering
    std::array<float, 8> interest;
    float target_reached_threshold = 0.f;
    bool reached_last_target = false;

    Vector2f target_position_cached;

    Vector2f result_direction = Vector2f::Zero();

    bool following = false;

    void detect_dangers();
    void detect_interests();

    void calculate_direction_to_move();

};

}