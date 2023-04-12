#pragma once
#include "State.h"
#include "TextureSelector.h"
namespace bm98
{

class PauseMenu;
class TilemapComponent;
class GameObject;
class PrefabEditor;
class SceneEditorView;
class Scene;

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
    virtual void render() override;
    virtual void pause_state() override;
    virtual void unpause_state() override;

    //void set_texture_rect();

protected:
    void init_variables();
    void init_views();
    //void init_fonts();
    void init_gui();
    void update_gui();
    void render_gui();

    void init_tilemap();

private:
    sf::View* object_heir_view;
    sf::View* component_view;
    sf::View* prefab_view;
    sf::View* inspector_view;

    GameObject* selected_gameobject = nullptr;

    float camera_move_speed;
    bool outline_render = true;
    Sorting::Layer outline_layer = Sorting::Layer::UI;
    bool text_select_render = false;
    Sorting::Layer text_select_layer = Sorting::Layer::UI;
    char z_order = 0;

    sf::Texture background_texture;
    sf::RectangleShape background;

    sf::RectangleShape selector_rect;
    sf::IntRect texture_rect;

    sf::RectangleShape sidebar;

    std::map<std::string, GUI::Button*> buttons;
    PauseMenu* pmenu;

    GUI::TextureSelector* texture_selector;

    //TilemapComponent* tilemap;
    //GameObject* tilemap_go;

    SceneEditorView* editor_view;
    Scene* active_scene;
    sf::View* scene_view;

    std::string active_scene_name;

};

}