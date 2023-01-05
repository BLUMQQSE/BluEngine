#pragma once
#include "Math.h"
#include "Component.h"
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

    sf::View& get_camera_view();
    sf::FloatRect get_view_bounds();

    void set_viewport(sf::FloatRect viewport);
    void set_position(Vector2f pos);

private:
    sf::View camera_view;

};
}
