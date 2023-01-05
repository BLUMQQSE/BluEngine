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
    void delete_buttons();

private:
    bool _render = true;
    SortingLayer background_layer = SortingLayer::BACKGROUND;
    unsigned char z_order = 0;
    sf::Texture background_texture;
    sf::RectangleShape background;
    std::map<std::string, GUI::Button*> buttons;

    sf::Shader shader;
    sf::Music music;

};

}