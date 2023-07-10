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

    virtual void on_draw_gizmos_selected() override;

    TilemapIntersect intersects(const FloatConvex collider, 
        PhysicsNS::LayerMask mask);

    virtual Vector2f get_closest_point(Vector2f pos) override;

private:
    std::weak_ptr<TilemapComponent> tilemap;
    std::vector<std::vector<FloatConvex>> colliders;

    void create_colliders();
    FloatConvex helper(int x, int y, int z, std::vector<std::vector<std::vector<Tile*>>> map);


    // Inherited via ColliderComponent
    virtual const bool blocks_light() override;

};

}