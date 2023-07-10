#pragma once
#include "State.h"
#include "../Math.h"
#include "../core/EventSystem.h"
namespace bm98
{
using namespace core;
class SettingsState :
    public State, public Listener
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

    virtual void init_state() override;
    virtual void init_variables() override;

    virtual void update_input() override;
    virtual void on_end_state() override;
    virtual void update() override;
    virtual void update_sfml(sf::Event sfEvent) override;
    virtual void late_update() override;
    virtual void fixed_update() override;
    virtual void render() override;

    FloatConvex circle = FloatConvex::Circle(Vector2f(300, 300), 60);

//    FloatConvex circle2 = FloatConvex::circle(Vector2f(300, 600), 40);

    std::unique_ptr<GUI::ProgressBar> pb;

    FloatConvex square = FloatConvex::Polygon(Vector2f(300, 600),
        { Vector2f(0,0), Vector2f(100, 0), Vector2f(100, 100), Vector2f(0, 100) });;
    
    bool holding = false;
    Vector2f offset = Vector2f::Zero();

    FloatConvex concave = FloatConvex::Polygon(Vector2f(100, 20),
        {Vector2f(0,0), Vector2f(200, 0), 
        Vector2f(200, 200), Vector2f(0, 200)});

protected:
    //void init_fonts();
    void init_buttons();
    void init_volume_display();

    void draw_gizmos();

    void get_rays();
    float max_dist = 300;
    FloatConvex light_bounds = FloatConvex::Circle(Vector2f::Zero(), 600, 30);
    std::vector<std::tuple<Vector2f, Vector2f, float>> rays;
    std::vector<Vector2f> ray_dirs;
    bool draw_lines = true;
    bool draw_vertex_arrays = false;

    std::vector<std::pair<FloatConvex*, IRenderable>> shapes;
    

private:
    bool _render = true;
    Sorting::Layer options_layer = Sorting::Layer::UI;
    char z_order = 0;
    sf::Texture background_texture;
    sf::RectangleShape background;
    std::map<std::string, std::unique_ptr<GUI::Button>> buttons;

    std::map<std::string, std::unique_ptr<GUI::DropDownList>> drop_downs;
   
    std::unique_ptr<GUI::FlagDropDownList> flag_drop;

    sf::Text options_text;

    std::map<std::string, std::unique_ptr<GUI::Slider>> volume_sliders;
    std::map <std::string, std::pair<std::unique_ptr<GUI::Label>,
        std::unique_ptr<GUI::Label>>> volume_displays;

    std::vector<sf::VideoMode> modes;

    void init_gui();
    void init_text();
    void init_drop_downs();

    // Inherited via Listener
    virtual void handle_event(Event* event) override;
};
}
