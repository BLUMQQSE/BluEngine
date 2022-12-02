#pragma once
#include "GameObject.h"
#include "SceneManager.h"
#include "Player.h"
namespace bm98
{
class SceneChange :
    public GameObject, public IObject
{
public:
    SceneChange(float pos_x, float pos_y, std::string scene_name,
        float spawn_x, float spawn_y);
    virtual ~SceneChange();

    virtual void on_collision_enter(Collision info) override;

    std::string get_scene();
    sf::Vector2f get_scene_destination();



private:
    std::string scene_name;
    sf::Vector2f scene_destination;
};

}