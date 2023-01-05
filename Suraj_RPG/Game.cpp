#include "pch.h"
#include "Game.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "Time.h"
#include "Input.h"
#include "Physics.h"
#include "Debug.h"
#include <windows.h>
namespace bm98::core
{

GameSettings Game::game_settings;

Game::Game()
{
    //restart_on_launch = true;
    //file_path = "Data/game.json";
    unserialize_json(FileManager::load_from_file("Data/game.json"));

    init_variables();
    init_graphics_settings();
    init_window();

    ResourceManager::load_resources();
    //renderer = new Renderer(window);
    Renderer::init(window, dev_window);
    Input::init(window);
    Physics::init();
    Debug::init();


    //Set up input
    Input::load_supported_keybinds_from_file("Config/supported_keys.ini");
    Input::load_keybinds_from_file("Main_Menu_State", "Config/mainmenustate_keybinds.ini");
    Input::load_keybinds_from_file("Editor_State", "Config/editorstate_keybinds.ini");
    Input::load_keybinds_from_file("Game_State", "Config/gamestate_keybinds.ini");
    Input::load_keybinds_from_file("Settings_State", "Config/settingsstate_keybinds.ini");
    
    Input::load_keybinds_from_file("Dev_Window", "Config/dev_window_keybinds.ini");

    Input::change_keybinds_state("Main_Menu_State");

    init_states();


}

Game::~Game()
{
    if (!successful_shutdown)
    {
        std::cout << "end_application was called in ~Game()\n";
        end_application();
    }

    delete window;
    //delete dev_window;
    FileManager::save_to_file_styled(serialize_json(), "Data/game.json");
}

void Game::run()
{
    while (window->isOpen())
    {
        update_delta_time();
        update();
        render();
    }
}

GameSettings& Game::get_game_settings()
{
    return game_settings;
}

void Game::update_sfml_events()
{
    while (window->pollEvent(sfevent))
    {
        if (sfevent.type == sf::Event::Closed)
        {
            end_application();
            window->close();
        }
        if (sfevent.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, sfevent.size.width, sfevent.size.height);
            window->setView(sf::View(visibleArea));
        }
        if (sfevent.type == sf::Event::MouseWheelMoved)
        {
            Input::update_mouse_scroll(sfevent.mouseWheel.delta);
        }

        if (sfevent.type == sf::Event::GainedFocus)
            if(states.size() > 0)
                Input::change_keybinds_state(states.top()->get_state_name());
        
    }
    while (dev_window->pollEvent(sfevent))
    {
        if (sfevent.type == sf::Event::GainedFocus)
            Input::change_keybinds_state("Dev_Window");
    }

}

void Game::update_delta_time()
{
    Time::update_delta(deltaClock.restart().asSeconds());
    
    if (!states.empty())
    {
        if (Time::fixed_delta_time() >= .02f)
        {
            Physics::fixed_update();
            states.top()->fixed_update();
            Time::reset_fixed_delta();
        }
        else
        {
            fixed_delta_timer += Time::delta_time();
        }
    }

    Time::apply_time_scale();
    if (fps_averager == 700)
    {
        fps_averager = 0; 
        fps = fps_col /700;
        fps_col = 0;


        std::stringstream s;
        s << std::fixed << std::setprecision(0) << fps;
        s << "       " << graphics_settings.game_title;
        window->setTitle(s.str());
        s.str("");
    }
    else
    {
        fps_averager++;
        fps_col += 1000 / (Time::delta_time() * 1000);
    }
}


void Game::update()
{
    update_sfml_events();
    Input::update();

    if (!states.empty())
    {
        
        states.top()->update();
        states.top()->late_update();

        Input::late_update();

        if (states.top()->get_quit())
        {
            states.top()->on_end_state();
            delete states.top();
            states.pop();
        }
        if (states.size() != state_count && !states.empty())
        {
            state_count = (int)states.size();
            Input::change_keybinds_state(states.top()->get_state_name());
            states.top()->init_state();
            Time::reset_time_since_state_change();
        }

    }
    else
    {
        end_application();
        window->close();
    }

    if (in_dev_window)
    {
        //here handle moving in dev view movement
    }

}


void Game::render()
{
    window->clear();
    dev_window->clear();

    if (!states.empty())
        states.top()->render();
   
    Renderer::render();

    window->display();
    dev_window->display();

}

void Game::init_variables()
{
    successful_shutdown = false;
    window = nullptr;
    fixed_delta_timer = 0.f;
}

void Game::init_graphics_settings()
{
    graphics_settings.load_from_file("Config/graphics.ini");
    dev_graphics_settings.load_from_file("Config/dev_window_graphics.ini");
}

void Game::init_window()
{

    
    if (dev_graphics_settings.full_screen)
    {
        dev_window = new sf::RenderWindow(dev_graphics_settings.resolution, dev_graphics_settings.game_title,
            sf::Style::Fullscreen, dev_graphics_settings.context_settings);
    }
    else
        dev_window = new sf::RenderWindow(dev_graphics_settings.resolution, dev_graphics_settings.game_title,
            sf::Style::Titlebar | sf::Style::Close, dev_graphics_settings.context_settings);

    dev_window->setFramerateLimit(dev_graphics_settings.framerate_limit);
    dev_window->setVerticalSyncEnabled(dev_graphics_settings.vertical_sync);
    dev_window->setKeyRepeatEnabled(false);
    dev_window->setPosition(sf::Vector2i(-2500, 1200));

    dev_window->setView(dev_window->getDefaultView());

    HWND console = GetConsoleWindow();
    SetWindowPos(console, 0, -2500 + dev_window->getSize().x, 1200, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    if (!open_dev_window)
    {
        dev_window->close();
    }
    if (graphics_settings.full_screen)
    {
        window = new sf::RenderWindow(graphics_settings.resolution, graphics_settings.game_title,
            sf::Style::Fullscreen, graphics_settings.context_settings);
    }
    else
        window = new sf::RenderWindow(graphics_settings.resolution, graphics_settings.game_title,
            sf::Style::Titlebar | sf::Style::Close, graphics_settings.context_settings);

    window->setFramerateLimit(graphics_settings.framerate_limit);
    window->setVerticalSyncEnabled(graphics_settings.vertical_sync);
    window->setKeyRepeatEnabled(false);

    
}

void Game::init_states()
{
    state_count = 1;

    //    state_data.window = window;
      //  state_data.graphics_settings = &graphics_settings;
        //state_data.grid_size = 32.f;
        //state_data.renderer = renderer;
        //state_data.states = &states;

    states.push(new MainMenuState(window, &states, &graphics_settings));
    //states.push(new GameState(window, &supported_keys));
}

void Game::end_application()
{
    while (!states.empty())
    {
        states.top()->on_end_state();
        delete states.top();
        states.pop();
    }

    successful_shutdown = true;
    std::cout << "Shutting down the game";
}

#pragma region IDATA

Json::Value Game::serialize_json()
{
    Json::Value obj;

    obj["Time"]["total-real-time"] = Time::total_real_time();

    return obj;
}
void Game::unserialize_json(Json::Value obj)
{
    Time::init(obj["Time"]["total-real-time"].asFloat());
}
/*
void Game::check_first_launch()
{
    Json::Value obj;
    obj = FileManager::load_from_file(file_path);
    if (obj.empty() || restart_on_launch)
    {

        Debug::Log("first");
        FileManager::save_to_file_styled(serialize_json(), file_path);
    }
}
*/

}