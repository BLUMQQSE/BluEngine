#pragma once
#include "State.h"
namespace bm98
{

class PauseMenu;
class TilemapComponent;
class GameObject;
class PrefabEditor;
class SceneEditor;

class EditorState :
    public State
{
public:
    EditorState(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings);
    virtual ~EditorState();

    virtual void on_end_state() override;
    virtual void update_input() override;

    virtual void update_sfml(sf::Event sfEvent) override;
    virtual void update() override;
    virtual void late_update() override;
    virtual void fixed_update() override;
    virtual void render() override;


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

    enum class EditingState
    {
        TILEMAP,
        SCENE,
        PREFAB
    };

    struct TileModifier
    {
        bool collision;
        TileType tile_type;
    };

    EditingState current_state;

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

    TilemapComponent* tilemap;
    GameObject* tilemap_go;

    // contain scene which is saved or loaded
    SceneEditor* editing_scene;

    PrefabEditor* editing_prefab;


};

}