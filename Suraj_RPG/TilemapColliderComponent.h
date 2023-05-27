#pragma once
#include "ColliderComponent.h"
#include "TilemapComponent.h"
namespace bm98
{
class TilemapColliderComponent :
    public ColliderComponent
{
public: 
    TilemapColliderComponent();
    virtual ~TilemapColliderComponent();

    virtual void awake() override;
    virtual void start() override;

    Vector2f intersects(const FloatConvex collider, 
        PhysicsNS::LayerMask mask);

private:

    std::weak_ptr<TilemapComponent> tilemap;
    std::vector<std::vector<FloatConvex>> colliders;

    void create_colliders();
    FloatConvex helper(int x, int y, int z, std::vector<std::vector<std::vector<Tile*>>> map);

};

}