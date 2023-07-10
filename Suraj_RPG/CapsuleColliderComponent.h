#pragma once
#include "ColliderComponent.h"
namespace bm98
{
class CapsuleColliderComponent :
    public ColliderComponent
{
    // Inherited via ColliderComponent
    virtual const bool blocks_light() override;
};

}