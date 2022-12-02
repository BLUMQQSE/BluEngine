#pragma once
#include "State.h"
#include "PauseMenu.h"
#include "Tilemap.h"
namespace bm98
{

class EditorState :
    public State
{
public:
    EditorState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings);
    virtual ~EditorState();

    virtual void on_end_state();
    virtual void update_input();

    virtual void update();
    virtual void late_update();
    virtual void fixed_update();
    virtual void render();


    //void set_texture_rect();

protected:
    void init_variables();
    void init_views();
    void init_background();
    //void init_fonts();
    void init_gui();
    void update_gui();
    void render_gui();
    void init_buttons();

    void init_tilemap();

private:

    struct TileModifier
    {
        bool collision;
        TileType tile_type;
    };

    sf::View main_view;
    float camera_move_speed;

    sf::Texture background_texture;
    sf::RectangleShape background;

    sf::RectangleShape selector_rect;
    sf::IntRect texture_rect;

    sf::RectangleShape sidebar;

    std::map<std::string, GUI::Button*> buttons;
    PauseMenu* pmenu;

    GUI::TextureSelector* texture_selector;
    TileModifier tile_modifier;




    Tilemap* tilemap;

};

}