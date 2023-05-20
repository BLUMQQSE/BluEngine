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
class GameEditorView;

class Dialogue;

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
    virtual void update_sfml(sf::Event sfEvent) override;
    virtual void fixed_update();
    virtual void late_update();

    virtual void render();

    // Inherited via IData
    virtual Json::Value serialize_json() override;
    virtual void unserialize_json(Json::Value obj) override;

protected:

    void init_view();
    void init_singletons();
private:
    std::string active_scene_name;

    sf::View* view;
    std::unique_ptr<Scene> active_scene;

    PauseMenu* pmenu;
    GameEditorView* editor_view;

    Dialogue* test_dialogue;

};

}