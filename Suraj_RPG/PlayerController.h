#pragma once
#include "core/EventSystem.h"
#include "Component.h"
#include "Math.h"
namespace bm98
{

class CameraComponent;
class AnimationComponent;
class RigidbodyComponent;
class Interactor;
class Inventory;
class InventoryGUIController;

using namespace core;
class PlayerController :
    public Component, public Listener
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
    virtual void on_collision_stay(Collision info) override;
    virtual void on_collision_exit(Collision info) override;


    virtual void on_trigger_enter(Collider info) override;

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;


private:
    Vector2f movement_input;
    bool attack;
    AnimationComponent* anim;
    RigidbodyComponent* rigid;
    Interactor* interactor;
    InventoryGUIController* inventory;

    CameraComponent* camera;
    Inventory* inv;

    void init_animations();

    void update_input();
    void update_animations();


    // Inherited via Listener
    virtual void handle_event(Event* event) override;

    Vector2f dialog_tag_pos;

};

}