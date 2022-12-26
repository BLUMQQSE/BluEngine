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
	camera_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
}

void CameraComponent::awake()
{
	
}

void CameraComponent::start()
{
	game_object->add_component<DontDestroyOnLoad>();
}

sf::View& CameraComponent::get_camera_view()
{
	return camera_view;
}

void CameraComponent::set_viewport(sf::FloatRect viewport)
{
	camera_view.setViewport(viewport);
}

void CameraComponent::set_position(Vector2f pos)
{
	camera_view.setCenter(pos);
}


}