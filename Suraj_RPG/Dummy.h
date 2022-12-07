#pragma once
#include "GameObject.h"
namespace bm98
{
class Dummy :
    public GameObject
{
public:
    Dummy(float x, float y);
    virtual ~Dummy();

    void init_anim();

    virtual void on_collision_enter(Collision info);

    void update();
    void fixed_update();

};

}