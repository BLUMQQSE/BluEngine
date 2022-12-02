#pragma once
#include "GameObject.h"
namespace bm98
{
class Dummy;

class Player :
    public GameObject, IData
{

public:
    Player(float x, float y);
    virtual ~Player();

    void update_input();
    void update_animations();

    virtual void update();
    virtual void fixed_update();

    virtual void on_collision_enter(Collision info) override;
    virtual void on_collision_stay(Collision info) override;
    virtual void on_collision_exit(Collision info) override;
    virtual void on_trigger_enter(Collider info) override;



protected:


    // Inherited via IData
    virtual Json::Value serialize_json() override;

    virtual void unserialize_json(Json::Value obj) override;

private:

    sf::Vector2f movement_input;
    bool attack;
    Dummy* dummy;

    virtual void init_variables();
    void init_components();

};
}
