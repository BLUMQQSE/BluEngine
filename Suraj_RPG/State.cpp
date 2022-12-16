#include "pch.h"
#include "State.h"
//#include "Game.h"
#include "GraphicsSettings.h"
#include "Input.h"
#include "Debug.h"
#include "Gui.h"
#include "Physics.h"

namespace bm98
{
using namespace core;
#pragma region PUBLIC

State::State(sf::RenderWindow* window, std::stack<State*>* states, GraphicsSettings* graphics_settings)
{
    Renderer::clear();
    this->window = window;
    this->graphics_settings = graphics_settings;
    this->states = states;
    isRequestingQuit = false;
    paused = false;
    init_state();
    init_fonts();
}

State::~State()
{
    Renderer::clear();
}

void State::on_end_state()
{
    Physics::clear_objects();
}

void State::pause_state()
{
    paused = true;
}

void State::unpause_state()
{
    paused = false;
}

const bool& State::get_quit() const { return isRequestingQuit; }

void State::init_state()
{

}

void State::init_variables()
{
}

void State::update_sfml(sf::Event sfEvent)
{
}

void State::update()
{

}

void State::fixed_update()
{

}

void State::late_update()
{
}

std::string State::get_state_name()
{
    return state_name;
}


void State::init_fonts_from_file(std::string file_path)
{
    if (!font.loadFromFile(file_path))
    {
        throw("ERROR::STATE::COULD NOT LOAD FONT FROM:: " + file_path);
    }
}

void State::init_fonts()
{
    init_fonts_from_file("Fonts/calibri-regular.ttf");
}

void State::init_buttons()
{
}

#pragma endregion
}