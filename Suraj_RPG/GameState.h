#pragma once
#include "State.h"
#include "GameClock.h"
#include "PauseMenu.h"
#include "ParticleSystem.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Player.h"
namespace bm98
{
class Player;

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
    Scene* active_scene;

    Player* player;
    // Dummy* dummy;

     //std::vector<GameObject*> objects;
     //std::vector<GameObject*> objects;

    PauseMenu* pmenu;
    // Tilemap* tilemap;
    ParticleSystem* particles;
};

}