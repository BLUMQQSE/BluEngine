#include "pch.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "DontDestroyOnLoad.h"
#include "core/Renderer.h"
#include "CameraComponent.h"
namespace bm98
{
CameraComponent::CameraComponent()
{	

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::init()
{
	camera_view.setCenter(game_object->get_world_position());
	camera_view.setSize(sf::Vector2f(core::Renderer::Instance()->get_window_size()));
	//camera_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	sf::Listener::setPosition(sf::Vector3f(game_object->get_physical_center().x, game_object->get_physical_center().y, 0));
}

void CameraComponent::awake()
{
	camera_view.zoom(.75f);
}

void CameraComponent::start()
{
	game_object->add_component<DontDestroyOnLoad>();
}

void CameraComponent::update()
{
	sf::Listener::setPosition(sf::Vector3f(game_object->get_physical_center().x, game_object->get_physical_center().y, 0));
	
}

void CameraComponent::move(Vector2f offset)
{
	game_object->move(offset * camera_speed * Time::Instance()->delta_time());
}

sf::View& CameraComponent::get_camera_view()
{
	return camera_view;
}

sf::FloatRect CameraComponent::get_view_bounds()
{
	return sf::FloatRect
	(
		sf::Vector2f(camera_view.getCenter().x - camera_view.getSize().x / 2,
			camera_view.getCenter().y - camera_view.getSize().y / 2),
		camera_view.getSize()
	);
}

void CameraComponent::set_viewport(sf::FloatRect viewport)
{
	camera_view.setViewport(viewport);
}

void CameraComponent::set_world_position(Vector2f pos)
{
	camera_view.setCenter(pos);
}


}