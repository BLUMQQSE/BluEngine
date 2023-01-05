#include "pch.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "DontDestroyOnLoad.h"
#include "Renderer.h"
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
	camera_view.setCenter(game_object->get_transform().position + game_object->get_transform().local_position);
	camera_view.setSize(sf::Vector2f(Renderer::get_window_size()));
	//camera_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	camera_view.zoom(.5f);
	sf::Listener::setPosition(sf::Vector3f(game_object->get_transform().position.x, game_object->get_transform().position.y, 0));
}

void CameraComponent::awake()
{
	
}

void CameraComponent::start()
{
	game_object->add_component<DontDestroyOnLoad>();
}

void CameraComponent::update()
{
	sf::Listener::setPosition(sf::Vector3f(game_object->get_transform().position.x, game_object->get_transform().position.y, 0));
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

void CameraComponent::set_position(Vector2f pos)
{
	game_object->set_position(pos.x, pos.y);
	camera_view.setCenter(pos);
}


}