#include "pch.h"
#include "Component.h"
#include "GameObject.h"
namespace bm98
{

Component::Component()
{
	
}

Component::~Component()
{
}

void Component::init()
{
}

void Component::update()
{
}

void Component::late_update()
{
}

void Component::fixed_update()
{
}

void Component::add_to_buffer(sf::View* view)
{
}

GameObject* Component::get_game_object()
{
	return game_object;
}

}