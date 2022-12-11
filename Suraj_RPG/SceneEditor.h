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
    Component* selected_component = nullptr;

    Panel* gameobject_heirarchy = nullptr;
    Panel* gameobject_selector = nullptr;
    Panel* gameobject_editor = nullptr;

    std::unordered_map<std::string, Button*> editor_buttons;
    std::map<std::string, Panel*> editor_panels;
    std::vector < std::pair<int, std::string>> editor_panel_order;

    std::unordered_map<std::string, Button*> addable_components;
    std::unordered_map<std::string, Button*> addable_gameobjects;

    InputBox* scene_name;

    void remove_component(std::string key);

    void create_sprite_component_panel();
    void create_boxcollider_component_panel();
    void update_sprite_component_panel();
    void update_boxcollider_component_panel();

    void create_empty_gameobject_editor();
    // should fill editor with its info
    void populate_editor();
    // gets next y position for an added component
    float get_next_y();
    /// <summary>
    /// update active gameobject with all info
    /// </summary>
    void update_editor();
    void update_selector();

    void create_gameobject_selector();
    void save_scene();
    


};

}