#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Collisions.h"
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

void Component::awake()
{
}

void Component::start()
{
}

void Component::on_destroy()
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

void Component::on_collision_enter(Collision info)
{
}

void Component::on_collision_stay(Collision info)
{
}

void Component::on_collision_exit(Collision info)
{
}

void Component::on_trigger_enter(Collider info)
{
}

void Component::on_trigger_stay(Collider info)
{
}

void Component::on_trigger_exit(Collider info)
{
}

#pragma region IData

Json::Value Component::serialize_json()
{
	return Json::Value();
}

void Component::unserialize_json(Json::Value obj)
{
}

#pragma endregion

void Component::set_game_object(GameObject* go)
{
	this->game_object = go;
}

void Component::set_active(bool active)
{
	this->active = active;
}

GameObject* Component::get_game_object()
{
	return game_object;
}



}