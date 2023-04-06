#include "pch.h"
#include "PlayerController.h"
#include "GameObject.h"
#include "Input.h"
#include "SceneManager.h"
#include "Collisions.h"
#include "SpriteComponent.h"
#include "ButtonComponent.h"
#include "Physics.h"
#include "CameraComponent.h"
#include "AudioSource.h"
#include "ResourceManager.h"
#include "DropDownComponent.h"
#include "ItemData.h"
#include "DataAsset.h"
#include "Inventory.h"
#include "CapsuleColliderComponent.h"
#include "EventSystem.h"
#include "Interactor.h"
#include "IInteractable.h"
#include "InventoryGUIController.h"
namespace bm98
{
using namespace core;
bm98::PlayerController::PlayerController()
{
	
}

bm98::PlayerController::~PlayerController()
{
	//delete inv;
}

void bm98::PlayerController::init()
{
	anim = &game_object->get_component<AnimationComponent>();
	rigid = &game_object->get_component<RigidbodyComponent>();
	interactor = &game_object->get_component<Interactor>();
	inventory = &game_object->get_component<InventoryGUIController>();

}

void PlayerController::awake()
{
	init_animations();
	camera = &SceneManager::Instance()->find_with_tag(Tags::Tag::CAMERA, this->game_object)->get_component<CameraComponent>();

}

void bm98::PlayerController::update()
{
	update_input();
	update_animations();

	interactor->interact();

	if (interactor->is_interacting())
		return;

	if (Input::Instance()->get_action_down("INTERACT"))
	{
		if (game_object->check_for_child("pants"))
		{
			SceneManager::Instance()->destroy_gameobject(SceneManager::Instance()->find("pants", game_object));
			return;
		}

		GameObject* pants = new GameObject();

		pants->info.name = "pants";
		pants->add_component<SpriteComponent>("pants.png");
		pants->add_component<AudioSource>();
		SpriteComponent* sc = &pants->get_component<SpriteComponent>();
		pants->add_component<ChildAnimationComponent>();
		sc->set_sorting_layer(Sorting::Layer::ACTOR);
		sc->set_z_order(game_object->get_component<SpriteComponent>().get_z_order() + 1);
		pants->set_parent(this->game_object);
		SceneManager::Instance()->instantiate_gameobject(pants);
		
	}

	Vector2f movement = game_object->get_center() - camera->get_game_object()->get_center();
	
	camera->get_game_object()->move(movement);
}

void bm98::PlayerController::late_update()
{
}

void bm98::PlayerController::fixed_update()
{
	if (interactor->is_interacting())
		return;
	
	rigid->apply_acceleration(movement_input);

	//Global::LayerMask mask = Global::LayerMask(true);
	//if (Physics::raycast(game_object->get_transform().position, movement_input.get_normalized(), 80,
		//mask))
		//std::cout << "raycast working...\n";
}

void bm98::PlayerController::add_to_buffer(sf::View*)
{
}

void bm98::PlayerController::on_collision_enter(Collision info)
{

	std::cout << "Player enter collision with: " << info.game_object->get_info().name << "\n";
}

void PlayerController::on_collision_stay(Collision info)
{
	std::cout << "Player stayed in collision with: " << info.game_object->get_info().name << "\n";

}

void PlayerController::on_collision_exit(Collision info)
{
	std::cout << "Player exit collision with: " << info.game_object->get_info().name << "\n";

}

void PlayerController::on_trigger_enter(Collider info)
{
	std::cout << "Player enter trigger of: " << info.game_object->get_info().name << "\n";
}

Json::Value PlayerController::serialize_json()
{
	Json::Value obj;


	return obj;
}

void PlayerController::unserialize_json(Json::Value obj)
{
}

void PlayerController::init_animations()
{
	anim->add_animation("IDLE_UP", 30.f, 0, 0, 0, 0, 64, 64, true);
	anim->add_animation("IDLE_LEFT", 30.f, 0, 1, 0, 0, 64, 64, true);
	anim->add_animation("IDLE_DOWN", 30.f, 0, 2, 0, 0, 64, 64, true);
	anim->add_animation("IDLE_RIGHT", 30.f, 0, 3, 0, 0, 64, 64, true);

	anim->add_animation("WALK_UP", 30.f, 0, 4, 2, 0, 64, 64, true);
	anim->add_animation("WALK_LEFT", 30.f, 0, 5, 2, 0, 64, 64, true);
	anim->add_animation("WALK_DOWN", 30.f, 0, 6, 2, 0, 64, 64, true);
	anim->add_animation("WALK_RIGHT", 30.f, 0, 7, 2, 0, 64, 64, true);

	anim->add_animation("ATTACK_UP", 30.f, 0, 16, 4, 0, 64, 64,
		false, true);

	anim->play("IDLE_DOWN");
}

void bm98::PlayerController::update_input()
{
	movement_input.x = 0;
	movement_input.y = 0;

	if (Input::Instance()->get_action_down("INTERACT"))
	{

	}
	if (Input::Instance()->get_action_down("INVENTORY"))
	{
		inventory->toggle_inventory(InventoryNS::WindowToggle::FULL_INVENTORY);
	}
	if (Input::Instance()->get_action("LEFT"))
		movement_input.x = -1;
	if (Input::Instance()->get_action("RIGHT"))
		movement_input.x = 1;
	if (Input::Instance()->get_action("UP"))
		movement_input.y = -1;
	if (Input::Instance()->get_action("DOWN"))
		movement_input.y = 1;

	if (Input::Instance()->get_mouse_up(Input::Mouse::LEFT))
	{
		attack = true;
	}

	if (movement_input != Vector2f::Zero())
	{
		game_object->get_component<AudioSource>().set_sound("footsteps.wav");
		game_object->get_component<AudioSource>().play();
	}
	else
	{
		game_object->get_component<AudioSource>().stop();
	}

}
void PlayerController::update_animations()
{
	//if (attack)
	//{
		//attack = anim->play("ATTACK_UP");
	//}
	Vector2f movement = rigid->get_velocity();
	if (movement.x == 0 && movement.y == 0
		&& rigid->get_orientation() == Orientation::Direction::UP)
		anim->play("IDLE_UP");
	else if (movement.x == 0 && movement.y == 0
		&& rigid->get_orientation() == Orientation::Direction::LEFT)
		anim->play("IDLE_LEFT");
	else if (movement.x == 0 && movement.y == 0
		&& rigid->get_orientation() == Orientation::Direction::DOWN)
		anim->play("IDLE_DOWN");
	else if (movement.x == 0 && movement.y == 0
		&& rigid->get_orientation() == Orientation::Direction::RIGHT)
		anim->play("IDLE_RIGHT");

	else if (movement.x < 0 && movement.y == 0
		&& rigid->get_orientation() == Orientation::Direction::LEFT)
		anim->play("WALK_LEFT", rigid->get_velocity().x,
			rigid->get_max_velocity());
	else if (movement.x > 0 && movement.y == 0
		&& rigid->get_orientation() == Orientation::Direction::RIGHT)
		anim->play("WALK_RIGHT", rigid->get_velocity().x,
			rigid->get_max_velocity());

	else if (movement.y < 0
		&& rigid->get_orientation() == Orientation::Direction::UP)
		anim->play("WALK_UP", rigid->get_velocity().y,
			rigid->get_max_velocity());
	else if (movement.y > 0
		&& rigid->get_orientation() == Orientation::Direction::DOWN)
		anim->play("WALK_DOWN", rigid->get_velocity().y,
			rigid->get_max_velocity());
}

}