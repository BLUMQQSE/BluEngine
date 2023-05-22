#pragma once
#include "State.h"

namespace bm98
{

class MainMenuState :
    public State
{

public:
    MainMenuState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphic_settings);
    virtual ~MainMenuState();

    virtual void init_state() override;
    virtual void on_end_state();
    virtual void update_input();

    virtual void update();

    virtual void render();

protected:
    void init_variables();
    void init_background();
    //void init_fonts();
    void init_buttons();

private: 
    IRenderable background_renderable{ Sorting::Layer::BACKGROUND, 0, true };

    sf::Texture background_texture;
    sf::RectangleShape background;
    std::map<std::string, std::unique_ptr<GUI::Button>> buttons;

    sf::Shader shader;
    sf::Music music;


};

}