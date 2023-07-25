#include "pch.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "DontDestroyOnLoad.h"
#include "core/Renderer.h"
#include "CameraComponent.h"
#include "core/SceneManager.h"
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
	camera_view.zoom(.55f);
}

void CameraComponent::start()
{
	game_object->add_component<DontDestroyOnLoad>();
}

void CameraComponent::update()
{
	sf::Listener::setPosition(sf::Vector3f(game_object->get_physical_center().x, game_object->get_physical_center().y, 0));
	if (Input::Instance()->get_mouse_scroll_delta() < 0)
	{
		camera_view.zoom(1.1f);
	}
	if (Input::Instance()->get_mouse_scroll_delta() > 0)
	{
		camera_view.zoom(.9f);
	}
}

void CameraComponent::move(Vector2f offset)
{
	Vector2f movement;
	movement.x = offset.x * Time::Instance()->delta_time() * camera_speed;
	movement.y = offset.y * Time::Instance()->delta_time() * camera_speed;
	game_object->move(movement);
}

void CameraComponent::set_position(Vector2f pos)
{
	game_object->set_world_position(pos);

	// deal with x axis
	if (std::abs(camera_view.getCenter().x - pos.x) > allowed_offset.x)
	{
		// need to move x
		if (camera_view.getCenter().x > pos.x)
			camera_view.setCenter(Vector2f(pos.x + allowed_offset.x , camera_view.getCenter().y));
		else
			camera_view.setCenter(Vector2f(pos.x - allowed_offset.x, camera_view.getCenter().y));
		
	}
	if (std::abs(camera_view.getCenter().y - pos.y) > allowed_offset.y)
	{
		// need to move y
		if (camera_view.getCenter().y < pos.y)
			camera_view.setCenter(Vector2f(camera_view.getCenter().x, pos.y - allowed_offset.y));
		else
			camera_view.setCenter(Vector2f(camera_view.getCenter().x, pos.y + allowed_offset.y));
	}


	return;
	camera_view.setCenter(
		pos.x + (static_cast<float>(Input::Instance()->mouse_position_window().x - static_cast<float>(gfx_settings->resolution.width / 2.f))) * 0.2f,
		pos.y + (static_cast<float>(Input::Instance()->mouse_position_window().y - static_cast<float>(gfx_settings->resolution.height / 2.f))) * 0.2f
	
	);
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

}