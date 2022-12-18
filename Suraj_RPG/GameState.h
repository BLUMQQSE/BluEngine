#pragma once
#include "State.h"
#include "IData.h"
namespace bm98
{
class Player;
class GameObject;
class Scene;
class ParticleSystem; 
class PauseMenu;

class GameState :
    public State, public IData
{

public:
    GameState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings,
        std::string game_save_name);
    virtual ~GameState();

    virtual void init_state() override;
    virtual void on_end_state() override;
    virtual void update_input() override;

    virtual void pause_state() override;
    virtual void unpause_state() override;

    virtual void update();
    virtual void fixed_update();
    virtual void late_update();

    virtual void render();

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

protected:

    void init_players();
    void init_view();


private:
    std::string game_save_name;
    std::string active_scene_name;


    sf::View* view;
    //std::vector<Scene*> scenes;
    Scene* active_scene;

    Player* player;
    GameObject* component_player;

    PauseMenu* pmenu;
    // Tilemap* tilemap;
    ParticleSystem* particles;

};

}