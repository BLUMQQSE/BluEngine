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

void Component::on_draw_gizmos()
{
}

void Component::on_draw_gizmos_selected()
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

void Component::set_world_position(Vector2f pos)
{
}

void Component::set_world_rotation(float rot)
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

void Component::set_game_object(std::shared_ptr<GameObject> go)
{
	this->game_object = go;
}

void Component::set_active(bool active)
{
	this->active = active;
}

//make sure to change to weak
std::shared_ptr<GameObject> Component::get_game_object()
{
	return game_object;
}

std::vector<Editor::SerializedVar> Component::get_editor_values()
{
	return std::vector<Editor::SerializedVar>();
}


}