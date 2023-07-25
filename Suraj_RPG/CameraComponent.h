#pragma once
#include "Math.h"
#include "Component.h"
#include "GraphicsSettings.h"
namespace bm98
{
class CameraComponent :
    public Component
{
public:
    CameraComponent();
    virtual ~CameraComponent();

    virtual void init() override;
    virtual void awake() override;
    virtual void start() override;
    virtual void update() override;

    void move(Vector2f offset);
    void set_position(Vector2f pos);

    sf::View& get_camera_view();
    sf::FloatRect get_view_bounds();

    void set_viewport(sf::FloatRect viewport);
    void set_graphics_settings(GraphicsSettings* gfx) { gfx_settings = gfx; }

private:
    sf::View camera_view;
    float camera_speed = 20.f;
    float zoom = 1;

    Vector2f allowed_offset = Vector2f(100, 50);

    GraphicsSettings* gfx_settings;

};

}
