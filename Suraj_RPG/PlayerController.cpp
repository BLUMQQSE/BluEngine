#include "pch.h"
#include "PlayerController.h"
#include "GameObject.h"
#include "Input.h"
#include "SceneManager.h"
#include "Collisions.h"
#include "SpriteComponent.h"
#include "ButtonComponent.h"
namespace bm98
{
using namespace core;
bm98::PlayerController::PlayerController()
{

}

bm98::PlayerController::~PlayerController()
{
}

void bm98::PlayerController::init()
{
	anim = &game_object->get_component<AnimationComponent>();
	rigid = &game_object->get_component<RigidbodyComponent>();
}

void PlayerController::awake()
{
	init_animations();
}

void bm98::PlayerController::update()
{
	update_input();
	update_animations();

	if (Input::get_action_down("INTERACT"))
	{
		//GameObject* pants = new GameObject();
		//pants->add_component<SpriteComponent>("Items/pants.png");
		//pants->add_component<ChildAnimationComponent>();
		//pants->set_parent(this->game_object);
		//this->game_object->add_child(pants);
		//SceneManager::instantiate_gameobject(pants);
	}

}

void bm98::PlayerController::late_update()
{
	
}

void bm98::PlayerController::fixed_update()
{
	rigid->apply_acceleration(movement_input.x, movement_input.y);
	
}

void bm98::PlayerController::add_to_buffer(sf::View*)
{
}

void bm98::PlayerController::on_collision_enter(Collision info)
{

	std::cout << "Player enter collision with: " << info.game_object->get_info().name << "\n";
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

	if (Input::get_action_down("INTERACT"))
	{
		//SceneManager::destroy_gameobject(dummy);
		//dummy = nullptr;
	}
	if (Input::get_action("LEFT"))
		movement_input.x = -1;
	if (Input::get_action("RIGHT"))
		movement_input.x = 1;
	if (Input::get_action("UP"))
		movement_input.y = -1;
	if (Input::get_action("DOWN"))
		movement_input.y = 1;

	if (Input::get_mouse_up(Input::Mouse::LEFT))
	{
		attack = true;
	}
}
void PlayerController::update_animations()
{
	if (attack)
	{
		attack = anim->play("ATTACK_UP");
	}
	else if (movement_input.x == 0 && movement_input.y == 0
		&& rigid->get_orientation() == Orientation::UP)
		anim->play("IDLE_UP");
	else if (movement_input.x == 0 && movement_input.y == 0
		&& rigid->get_orientation() == Orientation::LEFT)
		anim->play("IDLE_LEFT");
	else if (movement_input.x == 0 && movement_input.y == 0
		&& rigid->get_orientation() == Orientation::DOWN)
		anim->play("IDLE_DOWN");
	else if (movement_input.x == 0 && movement_input.y == 0
		&& rigid->get_orientation() == Orientation::RIGHT)
		anim->play("IDLE_RIGHT");

	else if (movement_input.x < 0 && movement_input.y == 0
		&& rigid->get_orientation() == Orientation::LEFT)
		anim->play("WALK_LEFT", rigid->get_velocity().x,
			rigid->get_max_velocity());
	else if (movement_input.x > 0 && movement_input.y == 0
		&& rigid->get_orientation() == Orientation::RIGHT)
		anim->play("WALK_RIGHT", rigid->get_velocity().x,
			rigid->get_max_velocity());

	else if (movement_input.y < 0
		&& rigid->get_orientation() == Orientation::UP)
		anim->play("WALK_UP", rigid->get_velocity().y,
			rigid->get_max_velocity());
	else if (movement_input.y > 0
		&& rigid->get_orientation() == Orientation::DOWN)
		anim->play("WALK_DOWN", rigid->get_velocity().y,
			rigid->get_max_velocity());
}

}