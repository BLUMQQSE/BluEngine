#pragma once
#include "State.h"

namespace bm98
{
class Player;
class GameObject;
class Scene;
class ParticleSystem; 
class PauseMenu;

class GameState :
    public State
{

public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings);
    virtual ~GameState();

    virtual void init_state();
    virtual void on_end_state();
    virtual void update_input();

    virtual void update();
    virtual void fixed_update();
    virtual void late_update();

    virtual void render();

protected:

    void init_players();
    void init_view();


private:
    sf::View view;
    //std::vector<Scene*> scenes;
    Scene* active_scene;

    Player* player;
    GameObject* component_player;

    PauseMenu* pmenu;
    // Tilemap* tilemap;
    ParticleSystem* particles;
};

}