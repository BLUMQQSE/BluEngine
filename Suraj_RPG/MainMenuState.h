#pragma once
#include "State.h"
#include "GameState.h"
#include "EditorState.h"
#include "SettingsState.h"
namespace bm98
{
class MainMenuState :
    public State
{

public:
    MainMenuState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphic_settings);
    virtual ~MainMenuState();

    virtual void on_end_state();
    virtual void update_input();

    virtual void update();
    virtual void fixed_update();
    virtual void late_update();

    virtual void render();

protected:
    void init_variables();
    void init_background();
    //void init_fonts();
    void init_buttons();

private:
    sf::Texture background_texture;
    sf::RectangleShape background;
    std::map<std::string, GUI::Button*> buttons;

};

}