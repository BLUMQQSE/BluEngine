#pragma once
#include "State.h"
#include "Math.h"
namespace bm98
{
class SettingsState :
    public State
{

    // Could be used to break up displays in settings
    // such as Video, Audio, Gameplay, Contols, etc
    /*
    struct SettingsDisplay
    {

    };
    */
public:
    SettingsState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings);
    virtual ~SettingsState();

    virtual void init_variables();

    virtual void update_input();
    virtual void on_end_state();
    virtual void update();
    virtual void late_update();
    virtual void fixed_update();
    virtual void render();

protected:
    //void init_fonts();
    void init_buttons();
    void init_volume_display();


private:
    bool _render = true;
    SortingLayer options_layer = SortingLayer::UI;
    char z_order = 0;
    sf::Texture background_texture;
    sf::RectangleShape background;
    std::map<std::string, GUI::Button*> buttons;
    std::map<std::string, GUI::DropDownList*> drop_downs;
   
    sf::Text options_text;

    std::map<std::string, GUI::Slider*> volume_sliders;
    std::map < std::string, 
        std::pair<GUI::Label*, GUI::Label*>> volume_displays;

    std::vector<sf::VideoMode> modes;

    void init_gui();
    void init_text();
    void init_drop_downs();
};
}
