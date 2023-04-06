#include "pch.h"
#include "Game.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "ConsoleManager.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "Time.h"
#include "Input.h"
#include "Physics.h"
#include "Debug.h"
#include <windows.h>
#include "EventSystem.h"
#include "SceneManager.h"
namespace bm98::core
{

bool DEBUG_MODE = true;

GameSettings Game::game_settings;

Game::Game()
{
    init_singletons();

    unserialize_json(FileManager::Instance()->load_from_file("Data/game.json"));

    init_variables();
    init_graphics_settings();
    init_window();

    // WISH I could figure out issue with this call
    //EventSystem::Instance()->push_event(EventID::_SYSTEM_RENDERER_INITIALIZE_, static_cast<void*>(window));
    Renderer::Instance()->init(window);

    EventSystem::Instance()->push_event(EventID::_SYSTEM_RESOURCEMANAGER_LOAD_RESOURCES_);
    EventSystem::Instance()->push_event(EventID::_SYSTEM_INPUT_INITIALIZE_, static_cast<void*>(window));
    EventSystem::Instance()->push_event(EventID::_SYSTEM_PHYSICS_INITIALIZE_);

    Debug::init();

    //Set up input
    Input::Instance()->load_supported_keybinds_from_file("Config/supported_keys.ini");
    Input::Instance()->load_keybinds_from_file("Main_Menu_State", "Config/mainmenustate_keybinds.ini");
    Input::Instance()->load_keybinds_from_file("Editor_State", "Config/editorstate_keybinds.ini");
    Input::Instance()->load_keybinds_from_file("Game_State", "Config/gamestate_keybinds.ini");
    Input::Instance()->load_keybinds_from_file("Settings_State", "Config/settingsstate_keybinds.ini");
    
    Input::Instance()->load_keybinds_from_file("Dev_Keybinds", "Config/dev_keybinds.ini");

    Input::Instance()->change_keybinds_state("Main_Menu_State");

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
    FileManager::Instance()->save_to_file_styled(serialize_json(), "Data/game.json");
}

void Game::run()
{
    while (window->isOpen())
    {
        update_delta_time();
        check_fixed_update();
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
            EventSystem::Instance()->push_event(EventID::_SYSTEM_INPUT_UPDATE_SCROLL_, static_cast<void*>(&sfevent.mouseWheel.delta));
        }

        if (sfevent.type == sf::Event::TextEntered)
        {
            if (states.size() > 0)
                states.top()->update_sfml(sfevent);
        }
    }

}

void Game::update_delta_time()
{
    float dt = deltaClock.restart().asSeconds();
    // Update delta time in Time class
    EventSystem::Instance()->push_event(EventID::_SYSTEM_TIME_UPDATE_, static_cast<void*>(&dt));
    
    // Apply time scale in Time class
    EventSystem::Instance()->push_event(EventID::_SYSTEM_TIME_APPLY_SCALE_);
    if (fps_averager == 1500)
    {
        fps_averager = 0; 
        fps = fps_col /1500;
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
        fps_col += 1000 / (Time::Instance()->delta_time() * 1000);
    }
}

void Game::update()
{
    update_sfml_events();

    EventSystem::Instance()->push_event(EventID::_SYSTEM_INPUT_UPDATE_);

    EventSystem::Instance()->push_event(EventID::_SYSTEM_EVENTSYSTEM_PROCESS_EVENTS_);
    if (!states.empty())
    {
        
        states.top()->update();
        states.top()->late_update();

        EventSystem::Instance()->push_event(EventID::_SYSTEM_INPUT_LATE_UPDATE_);

        if (states.top()->get_quit())
        {
            states.top()->on_end_state();
            delete states.top();
            states.pop();
        }
        if (states.size() != state_count && !states.empty())
        {
            state_count = (int)states.size();
            Input::Instance()->change_keybinds_state(states.top()->get_state_name());
            states.top()->init_state();
            // Reset time since state has changed in game
            EventSystem::Instance()->push_event(EventID::_SYSTEM_TIME_RESET_SINCE_STATE_CHANGE_);
        }

    }
    else
    {
        end_application();
        window->close();
    }

}

void Game::check_fixed_update()
{
    if (!states.empty())
    {

        if (Time::Instance()->fixed_delta_time() >= .01f)
        {
            EventSystem::Instance()->push_event(EventID::_SYSTEM_RENDERER_CLEAR_GIZMOS_);
            states.top()->fixed_update();
            EventSystem::Instance()->push_event(EventID::_SYSTEM_PHYSICS_FIXED_UPDATE_);
            EventSystem::Instance()->push_event(EventID::_SYSTEM_RENDERER_FIXED_UPDATE_);
            EventSystem::Instance()->push_event(EventID::_SYSTEM_TIME_RESET_FIXED_);
        }
    }
}

void Game::render()
{
    window->clear();

    if (!states.empty())
        states.top()->render();
   
    EventSystem::Instance()->push_event(EventID::_SYSTEM_RENDERER_RENDER_);

    window->display();

}

void Game::init_singletons()
{
    Input::Instance();
    Renderer::Instance();
    SceneManager::Instance();
    EventSystem::Instance();
    FileManager::Instance();
    Time::Instance();
    ResourceManager::Instance();
    Physics::Instance();
    ConsoleManager::Instance();
}

void Game::init_variables()
{
    successful_shutdown = false;
    window = nullptr;
}

void Game::init_graphics_settings()
{
    graphics_settings.load_from_file("Config/graphics.ini");
}

void Game::init_window()
{

    HWND console = GetConsoleWindow();
    SetWindowPos(console, 0, 20, 400, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    
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

    states.push(new MainMenuState(window, &states, &graphics_settings));
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

    obj["Time"]["total-real-time"] = Time::Instance()->total_real_time();

    return obj;
}
void Game::unserialize_json(Json::Value obj)
{
    float t = obj["Time"]["total-real-time"].asFloat();
    // Initialize time in Time class
    EventSystem::Instance()->push_event(EventID::_SYSTEM_TIME_INITIALIZE_, static_cast<void*>(&t));
}

}