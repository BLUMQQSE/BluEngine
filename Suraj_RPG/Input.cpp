#include "pch.h"
#include "Input.h"
#include "Time.h"
#include "Game.h"
namespace bm98::core
{

sf::Vector2f Input::mouse_position(sf::View* view)
{
    if (view)
    {
        window->setView(*view);
        sf::Vector2f temp = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        window->setView(window->getDefaultView());

        return temp;
    }

    return mouse_pos_view;
}

sf::Vector2i Input::mouse_position_screen()
{
    return mouse_pos_screen;
}

sf::Vector2i Input::mouse_position_window()
{
    return mouse_pos_window;
}

sf::Vector2i Input::mouse_position_grid(float& grid_size, sf::View* view)
{
    if (view)
    {
        window->setView(*view);
        sf::Vector2f temp = window->mapPixelToCoords(sf::Mouse::getPosition(*window));


        sf::Vector2i grid_view = sf::Vector2i(
            static_cast<int>(temp.x) / static_cast<int>(grid_size),
            static_cast<int>(temp.y) / static_cast<int>(grid_size)
        );

        window->setView(window->getDefaultView());

        return grid_view;
    }

    return sf::Vector2i(
        static_cast<int>(mouse_pos_view.x) / static_cast<int>(grid_size),
        static_cast<int>(mouse_pos_view.y) / static_cast<int>(grid_size)
    );
}

bool Input::get_action(std::string key)
{
    if (key_states.find(key) == key_states.end())
        return false;
    
    return key_states.at(key) == PressedState::PRESSED;
}

bool Input::get_action_down(std::string key)
{
    if (key_states.find(key) == key_states.end())
        return false;
    
    return key_states.at(key) == PressedState::PRESSED_FRAME;
}

bool Input::get_action_up(std::string key)
{
    if (key_states.find(key) == key_states.end())
        return false;
    
    return key_states.at(key) == PressedState::RELEASED_FRAME;
}

bool Input::get_mouse(Mouse mouse_button, float* duration)
{
    if (duration != nullptr)
    {
        switch ((int)mouse_button)
        {
        case 0:
            *duration = 
            *duration = mouse_left_timer.get_elapsed_time().asMilliseconds();
            return mouse_left == PressedState::PRESSED;
            break;
        case 1:
            *duration = mouse_right_timer.get_elapsed_time().asMilliseconds();
            return mouse_right == PressedState::PRESSED;
            break;
        case 2:
            *duration = mouse_middle_timer.get_elapsed_time().asMilliseconds();
            return mouse_middle == PressedState::PRESSED;
            break;
        default:
            *duration = -1;
            return false;
        }
    }

    switch ((int)mouse_button)
    {
    case 0:
        return mouse_left == PressedState::PRESSED;
        break;
    case 1:
        return mouse_right == PressedState::PRESSED;
        break;
    case 2:
        return mouse_middle == PressedState::PRESSED;
        break;
    default:
        return false;
    }
}

bool Input::get_mouse_down(Mouse mouse_button)
{
    switch ((int)mouse_button)
    {
    case 0:
        mouse_left_timer.restart();
        return mouse_left == PressedState::PRESSED_FRAME;
        break;
    case 1:
        mouse_right_timer.restart();
        return mouse_right == PressedState::PRESSED_FRAME;
        break;
    case 2:
        mouse_middle_timer.restart();
        return mouse_middle == PressedState::PRESSED_FRAME;
        break;
    default:
        return false;
    }
}

bool Input::get_mouse_up(Mouse mouse_button, float* duration)
{
    if (duration != nullptr)
    {
        switch ((int)mouse_button)
        {
            case 0:
                *duration = mouse_left_timer.get_elapsed_time().asMilliseconds();
                return mouse_left == PressedState::RELEASED_FRAME;
                break;
            case 1:
                *duration = mouse_right_timer.get_elapsed_time().asMilliseconds();
                return mouse_right == PressedState::RELEASED_FRAME;
                break;
            case 2:
                *duration = mouse_middle_timer.get_elapsed_time().asMilliseconds();
                return mouse_middle == PressedState::RELEASED_FRAME;
                break;
            default:
                *duration = -1;
                return false;
        }
    }
    switch ((int)mouse_button)
    {
        case 0:
            return mouse_left == PressedState::RELEASED_FRAME;
            break;
        case 1:
            return mouse_right == PressedState::RELEASED_FRAME;
            break;
        case 2:
            return mouse_middle == PressedState::RELEASED_FRAME;
            break;
        default:
            return false;
    }
}

bool Input::get_double_click()
{
    if (get_mouse_up())
    {
        if (time_since_left_clicked.restart().asSeconds() < .5f)
            return true;
    }
    return false;
}

float Input::get_mouse_scroll_delta()
{
    return mouse_scroll_delta;
}

void Input::load_keybinds_from_file(std::string keybind_state_name,
    std::string file_path)
{
    std::ifstream ifs(file_path);

    if (ifs.is_open())
    {
        std::string key = "";
        std::string key_value = "";
        std::unordered_map<std::string, int> new_map;
        while (ifs >> key >> key_value)
        {
            new_map[key] = supported_keys.at(key_value);
        }
        keybinds[keybind_state_name] = new_map;
    }

    ifs.close();
}

void Input::load_supported_keybinds_from_file(std::string file_path)
{
    std::ifstream ifs(file_path);

    if (ifs.is_open())
    {
        std::string key = "";
        int key_value = 0;
        while (ifs >> key >> key_value)
        {
            supported_keys[key] = key_value;
        }
    }

    ifs.close();
}


void Input::change_keybinds_state(std::string keybind)
{
    //we need to empty key_states
    key_states.clear();
    //then need to point current_keybinds to keybinds[keybinds]
    current_keybinds = &keybinds[keybind];
    //set all to released frame, will be updated next frame and set to released 
    for (auto it : *current_keybinds)
    {
        key_states[it.first] = PressedState::RESET;
    }
    mouse_left = PressedState::RESET;
    mouse_right = PressedState::RESET;
    mouse_middle = PressedState::RESET;
}

const bool Input::using_input_box()
{
    return in_input_box;
}

void Input::set_using_input_box(bool val)
{
    in_input_box = val;
}

const sf::RenderWindow* Input::get_window() const
{
    return window;
}

Input::Input()
{
    EventSystem::Instance()->subscribe(EventID::_SYSTEM_INPUT_INITIALIZE_, this);
    EventSystem::Instance()->subscribe(EventID::_SYSTEM_INPUT_UPDATE_, this);
    EventSystem::Instance()->subscribe(EventID::_SYSTEM_INPUT_RESET_UPDATE_, this);
    EventSystem::Instance()->subscribe(EventID::_SYSTEM_INPUT_UPDATE_SCROLL_, this);

}

void Input::update_mouse_input()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (mouse_left == PressedState::UNPRESSED)
            mouse_left = PressedState::PRESSED_FRAME;
        else if (mouse_left == PressedState::PRESSED_FRAME)
            mouse_left = PressedState::PRESSED;
    }
    else
    {
        if (mouse_left == PressedState::PRESSED)
            mouse_left = PressedState::RELEASED_FRAME;
        else if (mouse_left == PressedState::RELEASED_FRAME)
            mouse_left = PressedState::UNPRESSED;

        if (mouse_left == PressedState::RESET)
            mouse_left = PressedState::UNPRESSED;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (mouse_right == PressedState::UNPRESSED)
            mouse_right = PressedState::PRESSED_FRAME;
        else if (mouse_right == PressedState::PRESSED_FRAME)
            mouse_right = PressedState::PRESSED;
    }
    else
    {
        if (mouse_right == PressedState::PRESSED)
            mouse_right = PressedState::RELEASED_FRAME;
        else if (mouse_right == PressedState::RELEASED_FRAME)
            mouse_right = PressedState::UNPRESSED;

        if (mouse_right == PressedState::RESET)
            mouse_right = PressedState::UNPRESSED;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
    {
        if (mouse_middle == PressedState::UNPRESSED)
            mouse_middle = PressedState::PRESSED_FRAME;
        else if (mouse_middle == PressedState::PRESSED_FRAME)
            mouse_middle = PressedState::PRESSED;
    }
    else
    {
        if (mouse_middle == PressedState::PRESSED)
            mouse_middle = PressedState::RELEASED_FRAME;
        else if (mouse_middle == PressedState::RELEASED_FRAME)
            mouse_middle = PressedState::UNPRESSED;

        if (mouse_middle == PressedState::RESET)
            mouse_middle = PressedState::UNPRESSED;
    }

    // TODO: add mouse wheel scroll delta
}

void Input::update_key_input()
{
    for (auto it : *current_keybinds)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(it.second)))
        {
            if (key_states.at(it.first) == PressedState::UNPRESSED)
                key_states[it.first] = PressedState::PRESSED_FRAME;
            else if (key_states.at(it.first) == PressedState::PRESSED_FRAME)
                key_states[it.first] = PressedState::PRESSED;
        }
        else
        {
            if (key_states.at(it.first) == PressedState::PRESSED)
                key_states[it.first] = PressedState::RELEASED_FRAME;
            else if (key_states.at(it.first) == PressedState::RELEASED_FRAME)
                key_states[it.first] = PressedState::UNPRESSED;

            if (key_states.at(it.first) == PressedState::RESET)
                key_states[it.first] = PressedState::UNPRESSED;
        }
    }
    
}

void Input::update_mouse_states()
{
    switch (mouse_left)
    {
        case bm98::core::Input::PressedState::PRESSED_FRAME:
            mouse_left = PressedState::PRESSED;
            break;
        case bm98::core::Input::PressedState::RELEASED_FRAME:
            mouse_left = PressedState::UNPRESSED;
            break;
    }
    switch (mouse_right)
    {
        case bm98::core::Input::PressedState::PRESSED_FRAME:
            mouse_right = PressedState::PRESSED;
            break;
        case bm98::core::Input::PressedState::RELEASED_FRAME:
            mouse_right = PressedState::UNPRESSED;
            break;
    }
    switch (mouse_middle)
    {
        case bm98::core::Input::PressedState::PRESSED_FRAME:
            mouse_middle = PressedState::PRESSED;
            break;
        case bm98::core::Input::PressedState::RELEASED_FRAME:
            mouse_middle = PressedState::UNPRESSED;
            break;
    }
}

void Input::update_key_states()
{
    for (auto& it : key_states)
    {
        switch (it.second)
        {
            case PressedState::PRESSED_FRAME:
                it.second = PressedState::PRESSED;
                break;
            case PressedState::RELEASED_FRAME:
                it.second = PressedState::UNPRESSED;
                break;
        }
    }
}

void Input::handle_event(Event* event)
{
    switch (event->get_event_id())
    {
        case EventID::_SYSTEM_INPUT_INITIALIZE_:
            init(static_cast<sf::RenderWindow*>(event->get_parameter()));
            break;
        case EventID::_SYSTEM_INPUT_UPDATE_:
            update();
            break;
        case EventID::_SYSTEM_INPUT_RESET_UPDATE_:
            reset_update();
            break;
        case EventID::_SYSTEM_INPUT_UPDATE_SCROLL_:
            update_mouse_scroll(*static_cast<int*>(event->get_parameter()));
            break;
    }
}

void Input::init(sf::RenderWindow* window_ptr)
{
    window = window_ptr;
    
}

void Input::update()
{
    mouse_pos_screen = sf::Mouse::getPosition();
    mouse_pos_window = sf::Mouse::getPosition(*window);
    mouse_pos_view = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    update_mouse_input();
    update_key_input();

}

void Input::update_mouse_scroll(int scroll_delta)
{
    mouse_scroll_delta = scroll_delta;
}

void Input::reset_update()
{
    update_mouse_states();
    update_key_states();

    mouse_scroll_delta = 0.f;
}

}