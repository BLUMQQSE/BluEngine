#include "pch.h"
#include "Player.h"
#include "Dummy.h"
#include "SceneManager.h"
#include "Input.h"
#include "Debug.h"
#include "Collisions.h"

#include "RigidbodyComponent.h"
#include "BoxColliderComponent.h"
#include "AnimationComponent.h"
#include "ChildAnimationComponent.h"
namespace bm98
{
using namespace core;
#pragma region PUBLIC

Player::Player(float x, float y)
{
	info.id = 101;
	info.tag = Tag::PLAYER;
	info.layer = Layer::PLAYER;
	info.name = "Player";
	if (!this->texture_sheet.loadFromFile("Resources/Images/Sprites/Player/player_sprite_sheet.png"))
	{
		throw("ERROR::PLAYER::COULD NOT LOAD texture_sheet");
	};
	//file_path = "Data/player.json";

	init_variables();
	set_position(x, y);
	set_sprite_texture(texture_sheet);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(32, 32)));

	add_component<RigidbodyComponent>(sprite, 200.f, 15.f, 50.f);
	add_component<BoxColliderComponent>(sprite, 16.f, -14.f, 32.f, 50.f, false);

	add_component<AnimationComponent>(sprite, texture_sheet);
	get_component<AnimationComponent>().add_animation("IDLE_UP", 30.f, 0, 0, 0, 0, 64, 64, true);
	get_component<AnimationComponent>().add_animation("IDLE_LEFT", 30.f, 0, 1, 0, 0, 64, 64, true);
	get_component<AnimationComponent>().add_animation("IDLE_DOWN", 30.f, 0, 2, 0, 0, 64, 64, true);
	get_component<AnimationComponent>().add_animation("IDLE_RIGHT", 30.f, 0, 3, 0, 0, 64, 64, true);

	get_component<AnimationComponent>().add_animation("WALK_UP", 30.f, 0, 4, 2, 0, 64, 64, true);
	get_component<AnimationComponent>().add_animation("WALK_LEFT", 30.f, 0, 5, 2, 0, 64, 64, true);
	get_component<AnimationComponent>().add_animation("WALK_DOWN", 30.f, 0, 6, 2, 0, 64, 64, true);
	get_component<AnimationComponent>().add_animation("WALK_RIGHT", 30.f, 0, 7, 2, 0, 64, 64, true);

	get_component<AnimationComponent>().add_animation("ATTACK_UP", 30.f, 0, 16, 4, 0, 64, 64,
		false, true);

	get_component<AnimationComponent>().play("IDLE_DOWN");

	dummy = new Dummy(1, 1);
	dummy->set_parent(this);
	dummy->init_anim();
	
	//dummy->add_child_animation_component();
	SceneManager::instantiate_gameobject(dummy, true);

	render_object = new Renderer::RenderObject(&sprite, SortingLayer::ACTOR);


	std::cout << "component count on player: " << components.size();
}

Player::~Player()
{
}

void Player::update_input()
{
	movement_input.x = 0;
	movement_input.y = 0;

	if (Input::get_action_down("INTERACT"))
	{
		SceneManager::destroy_gameobject(dummy);
		dummy = nullptr;
	}
	if (Input::get_action("LEFT"))
		movement_input.x = -1;
	if (Input::get_action("RIGHT"))
		movement_input.x = 1;
	if (Input::get_action("UP"))
		movement_input.y = -1;
	if (Input::get_action("DOWN"))
		movement_input.y = 1;

	if (Input::get_mouse_down(Input::Mouse::LEFT))
		std::cout << "";

	if (Input::get_mouse_up(Input::Mouse::LEFT))
	{
		attack = true;
	}
}

void Player::update_animations()
{
	if (attack)
	{
		attack = get_component<AnimationComponent>().play("ATTACK_UP");
	}
	else if (movement_input.x == 0 && movement_input.y == 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::UP)
		get_component<AnimationComponent>().play("IDLE_UP");
	else if (movement_input.x == 0 && movement_input.y == 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::LEFT)
		get_component<AnimationComponent>().play("IDLE_LEFT");
	else if (movement_input.x == 0 && movement_input.y == 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::DOWN)
		get_component<AnimationComponent>().play("IDLE_DOWN");
	else if (movement_input.x == 0 && movement_input.y == 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::RIGHT)
		get_component<AnimationComponent>().play("IDLE_RIGHT");

	else if (movement_input.x < 0 && movement_input.y == 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::LEFT)
		get_component<AnimationComponent>().play("WALK_LEFT", get_component<RigidbodyComponent>().get_velocity().x,
			get_component<RigidbodyComponent>().get_max_velocity());
	else if (movement_input.x > 0 && movement_input.y == 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::RIGHT)
		get_component<AnimationComponent>().play("WALK_RIGHT", get_component<RigidbodyComponent>().get_velocity().x,
			get_component<RigidbodyComponent>().get_max_velocity());

	else if (movement_input.y < 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::UP)
		get_component<AnimationComponent>().play("WALK_UP", get_component<RigidbodyComponent>().get_velocity().y,
			get_component<RigidbodyComponent>().get_max_velocity());
	else if (movement_input.y > 0
		&& get_component<RigidbodyComponent>().get_orientation() == Orientation::DOWN)
		get_component<AnimationComponent>().play("WALK_DOWN", get_component<RigidbodyComponent>().get_velocity().y,
			get_component<RigidbodyComponent>().get_max_velocity());
}

void Player::update()
{
	update_input();

	update_animations();

	GameObject::update();
}

void Player::fixed_update()
{
	//rigidbody_component->apply_acceleration(movement_input.x, movement_input.y);
	get_component<RigidbodyComponent>().apply_acceleration(movement_input.x, movement_input.y);
	GameObject::fixed_update();
}

void Player::on_collision_enter(Collision info)
{
	GameObject::on_collision_enter(info);
	Debug::Log("Player enter collision with: " + info.game_object->get_info().name);
}

void Player::on_collision_stay(Collision info)
{
	GameObject::on_collision_stay(info);
	Debug::Log("Player stay in collision with: " + info.game_object->get_info().name);
}

void Player::on_collision_exit(Collision info)
{
	GameObject::on_collision_exit(info);
	Debug::Log("Player exit collision with: " + info.game_object->get_info().name);
}

void Player::on_trigger_enter(Collider info)
{
	GameObject::on_trigger_enter(info);
}

void Player::init_components()
{

}

#pragma endregion

#pragma region PROTECTED

Json::Value Player::serialize_json()
{
	Json::Value obj;
	return obj;
}

void Player::unserialize_json(Json::Value obj)
{

}

#pragma endregion


#pragma region PRIVATE

void Player::init_variables()
{

}


#pragma endregion
}