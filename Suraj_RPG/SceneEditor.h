#pragma once
namespace bm98
{
namespace GUI
{
class Button;
class Panel;
class InputBox;
}
class GameObject;
class Scene;
class Component;

using namespace GUI;

class SceneEditor
{
    
public:
    SceneEditor(sf::Font* font);
    virtual ~SceneEditor();

    virtual void update_sfml(sf::Event sfEvent);
    virtual void update();
    virtual void render(sf::View* view = nullptr);

private:
    sf::Font& font;
    Scene* scene = nullptr;

    GameObject* selected_gameobject = nullptr;

    // Gameobject heirarchy window
    Panel* gameobject_heirarchy = nullptr;
    //std::unordered_map<Button*, std::vector<GameObject*>> gameobjects_in_scene;
    // TODO: figure out which of these two should do
    std::unordered_map < Button*, std::vector<std::pair<Button*, GameObject*>>>
        gameobjects_in_scene;
    bool gameobject_held = false;
    Button* selected_button = nullptr;

    // Gameobject editor window
    Panel* gameobject_editor = nullptr;
    std::unordered_map<std::string, Button*> editor_buttons;
    std::map<std::string, Panel*> editor_panels;
    std::vector < std::pair<int, std::string>> editor_panel_order;

    // Gameobject selector window
    Panel* gameobject_selector = nullptr;
    std::unordered_map<std::string, Button*> addable_components;
    std::unordered_map<std::string, Button*> addable_gameobjects;



    InputBox* scene_name;
    
    void create_gameobject();

    void remove_component(std::string key);

    void create_info_panel();
    void create_transform_panel();

    void create_sprite_component_panel();
    void create_boxcollider_component_panel();
    void create_animation_component_panel();
    void create_childanimation_component_panel();
    void create_animatedsprite_component_panel();

    void update_info_panel();
    void update_transform_panel();

    void update_sprite_component_panel();
    void update_boxcollider_component_panel();
    void update_animation_component_panel();
    void update_childanimation_component_panel();
    void update_animatedsprite_component_panel();

    
    
    //clears editor of all panels
    void clear_editor();
    // should fill editor with its info
    void populate_editor();
    
    // gets next y position for an added component
    float get_next_component_y();
    //gets next y position for an added gameobject button
    float get_next_gameobject_y();

    /// <summary>
    /// update active gameobject with all info
    /// </summary>
    void update_editor();
    void update_selector();
    void update_heirarchy();
    void update_heirarchy_positions();

    void create_empty_gameobject_editor();
    void create_gameobject_selector();
    void create_gameobject_heirarchy();
    void save_scene();
    
    //currently not needed
    std::vector<std::pair<Button*, GameObject*>> 
        insert_sort(std::vector<std::pair<Button*, GameObject*>> vec);

};

}