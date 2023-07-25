#pragma once
#include "Light.h"
#include "ColliderComponent.h"
#include "Collisions.h"
namespace bm98
{
class PointLight :
    public Light, public IRenderable
{
    
public:
    PointLight() {}
    virtual ~PointLight() {}
    
    virtual void init() override;
    virtual void awake() override;

    virtual void update() override;
    virtual void fixed_update() override;
    
    virtual void on_trigger_enter(Collider info) override;
    virtual void on_trigger_exit(Collider info) override;

    virtual void on_draw_gizmos() override;
    
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

    virtual std::vector<Editor::SerializedVar> get_editor_values() override;
    
private:
    float radius;
    float intensity;
    sf::Color color;
    Vector2f last_fixed_pos;
    bool update_light = true;

    std::string shader_file_name;
    sf::Shader* light_shader = nullptr;

    sf::VertexArray tris;

    std::vector<std::pair<std::weak_ptr<ColliderComponent>, Vector2f>> light_blocker_colliders;
    std::vector<std::pair<std::weak_ptr<ColliderComponent>, Vector2f>> light_absorber_colliders;

    std::weak_ptr<ColliderComponent> light_collider;

    std::vector<std::tuple<Vector2f, Vector2f, float>> rays;

    void calculate_rays();
    void calculate_triangles();
    void check_positions();
    
};
}
