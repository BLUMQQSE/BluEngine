#pragma once
#include "Component.h"
#include "Math.h"
namespace bm98
{

class AnimationComponent;
class RigidbodyComponent;

class PlayerController :
    public Component
{

public:
    PlayerController();
    virtual ~PlayerController();

    virtual void init() override;
    virtual void awake() override;
    virtual void update() override;
    virtual void late_update() override;
    virtual void fixed_update() override;
    virtual void add_to_buffer(sf::View* = nullptr) override;

    virtual void on_collision_enter(Collision info) override;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;


private:
    Vector2f movement_input;
    bool attack;
    AnimationComponent* anim;
    RigidbodyComponent* rigid;

    void init_animations();

    void update_input();
    void update_animations();

};

}