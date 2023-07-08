#include "../pch.h"
#include "PlayerController.h"
#include "../GameObject.h"
#include "../Collisions.h"
#include "../SpriteComponent.h"
#include "../CameraComponent.h"
#include "../AudioSource.h"
#include "../data_assets/DataAssets.h"
#include "Inventory.h"
#include "../CapsuleColliderComponent.h"
#include "../scripts/interaction/Interactor.h"
#include "../scripts/interaction/IInteractable.h"
#include "InventoryGUIController.h"

#include "../core/EventSystem.h"
#include "../core/UITagSystem.h"
#include "../core/Physics.h"
#include "../core/ResourceManager.h"
#include "../core/Input.h"
#include "../core/SceneManager.h"
#include "damage/Damager.h"
namespace bm98
{
using namespace core;
bm98::PlayerController::PlayerController()
{
	EventSystem::Instance()->subscribe(EventID::ANIMATION_COMPLETE, this);
	EventSystem::Instance()->subscribe(EventID::ANIMATION_FRAME_CHANGE, this);
}

bm98::PlayerController::~PlayerController()
{
}

void bm98::PlayerController::init()
{
	anim = game_object->get_component<AnimationComponent>();

	rigid = game_object->get_component<RigidbodyComponent>();
	interactor = game_object->get_component<Interactor>();
	inventory = game_object->get_component<InventoryGUIController>();
}

void PlayerController::awake()
{
	init_animations();

	camera = SceneManager::Instance()->find_with_tag(Tags::Tag::CAMERA,
											game_object->self()).lock()->get_component<CameraComponent>();
}

void bm98::PlayerController::update()
{
	update_input();
	update_animations();

	interactor.lock()->interact();

	if (interactor.lock()->is_interacting())
		return;

	Vector2f movement = (game_object->get_visual_center() - camera.lock()->get_game_object()->get_visual_center());

	//camera.lock()->get_game_object()->move(movement);
	camera.lock()->move(movement);
}

void bm98::PlayerController::late_update()
{
	dialog_tag_pos = game_object->get_visual_center() + Vector2f(-35, -60);
}

void bm98::PlayerController::fixed_update()
{
	if (interactor.lock()->is_interacting())
		return;

	//raycast = Physics::Instance()->raycast(game_object->get_center(), Vector2f(0, -1), game_object->self(), 80);

	if (!attack)
		rigid.lock()->move(movement_input);
		//rigid.lock()->apply_acceleration(movement_input);

}

void bm98::PlayerController::add_to_buffer(sf::View*)
{
}

void PlayerController::on_draw_gizmos()
{
}

void PlayerController::on_draw_gizmos_selected()
{

}

void bm98::PlayerController::on_collision_enter(Collision info)
{
}

void PlayerController::on_collision_stay(Collision info)
{
	game_object->get_component<Damager>().lock()->apply_damage();
}

void PlayerController::on_collision_exit(Collision info)
{

}

void PlayerController::on_trigger_enter(Collider info)
{
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
	anim.lock()->add_animation("IDLE_UP", 30.f, 0, 0, 1, 0, 64, 64, true);
	anim.lock()->add_animation("IDLE_LEFT", 30.f, 0, 1, 1, 0, 64, 64, true);
	anim.lock()->add_animation("IDLE_DOWN", 30.f, 0, 2, 1, 0, 64, 64, true);
	anim.lock()->add_animation("IDLE_RIGHT", 30.f, 0, 3, 1, 0, 64, 64, true);

	anim.lock()->add_animation("WALK_UP", 30.f, 0, 4, 3, 0, 64, 64, true);
	anim.lock()->add_animation("WALK_LEFT", 30.f, 0, 5, 3, 0, 64, 64, true);
	anim.lock()->add_animation("WALK_DOWN", 30.f, 0, 6, 3, 0, 64, 64, true);
	anim.lock()->add_animation("WALK_RIGHT", 30.f, 0, 7, 3, 0, 64, 64, true);

	anim.lock()->add_animation("ATTACK_UP",  5.f, 0, 16, 5, 0, 64, 64, false, true);
	anim.lock()->add_animation("ATTACK_LEFT", 5.f, 0, 17, 5, 0, 64, 64, false, true);
	anim.lock()->add_animation("ATTACK_DOWN", 5.f, 0, 18, 5, 0, 64, 64, false, true);
	anim.lock()->add_animation("ATTACK_RIGHT", 5.f, 0, 19, 5, 0, 64, 64, false, true);

	anim.lock()->play("IDLE_DOWN");
}

void bm98::PlayerController::update_input()
{
	movement_input.x = 0;
	movement_input.y = 0;

	if (Input::Instance()->get_action_down("INTERACT"))
	{
		std::weak_ptr<TextUITag> tag = UITagSystem::Instance()->create_text_tag(game_object->get_world_position(), "Boy I could use a snack!", 16, 5);
		tag.lock()->add_action(UITag::Action::FOLLOW, 0, (void*)&dialog_tag_pos);
		
		//ImageUITag* tag2 = new ImageUITag(game_object->get_world_position() + Vector2f(100, 100), Vector2f(300,200), 
			//							  &ResourceManager::Instance()->get_texture("player.png"), 5);

		/*
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
		*/
	}
	if (Input::Instance()->get_action_down("INVENTORY"))
	{
		inventory.lock()->toggle_inventory(InventoryNS::WindowToggle::FULL_INVENTORY);
	}
	if (Input::Instance()->get_action("LEFT"))
		movement_input.x = -1;
	if (Input::Instance()->get_action("RIGHT"))
		movement_input.x = 1;
	if (Input::Instance()->get_action("UP"))
		movement_input.y = -1;
	if (Input::Instance()->get_action("DOWN"))
		movement_input.y = 1;

	if (Input::Instance()->get_mouse_up())
	{
		attack = true;
	}

	if (movement_input != Vector2f::Zero())
	{
		game_object->get_component<AudioSource>().lock()->set_sound("footsteps.wav");
		game_object->get_component<AudioSource>().lock()->play();
	}
	else
	{
		game_object->get_component<AudioSource>().lock()->stop();
	}

}
void PlayerController::update_animations()
{
	if (attack)
	{
		if (rigid.lock()->get_orientation() == Orientation::Direction::UP)
			anim.lock()->play("ATTACK_UP");
		else if (rigid.lock()->get_orientation() == Orientation::Direction::LEFT)
			anim.lock()->play("ATTACK_LEFT");
		else if (rigid.lock()->get_orientation() == Orientation::Direction::DOWN)
			anim.lock()->play("ATTACK_DOWN");
		else if (rigid.lock()->get_orientation() == Orientation::Direction::RIGHT)
			anim.lock()->play("ATTACK_RIGHT");

		return;
	}

	Vector2f movement = rigid.lock()->get_movement_dir();
	bool moving = movement != Vector2f::Zero();
	Orientation::Direction dir = rigid.lock()->get_orientation();
	switch (dir)
	{
		case bm98::Orientation::Direction::UP:
		{
			if(!moving)
				anim.lock()->play("IDLE_UP");
			else
				anim.lock()->play("WALK_UP");
				
		}
			break;
		case bm98::Orientation::Direction::DOWN:
		{
			if (!moving)
				anim.lock()->play("IDLE_DOWN");
			else
				anim.lock()->play("WALK_DOWN");
		}
			break;
		case bm98::Orientation::Direction::LEFT:
		{
			if (!moving)
				anim.lock()->play("IDLE_LEFT");
			else
				anim.lock()->play("WALK_LEFT");
		}
			break;
		case bm98::Orientation::Direction::RIGHT:
		{
			if(!moving)
				anim.lock()->play("IDLE_RIGHT");
			else
				anim.lock()->play("WALK_RIGHT");
		}
			break;
	}
}

void PlayerController::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::ANIMATION_FRAME_CHANGE:
	{
		if (event->get_parameter())
		{
			// need to ensure this is a AnimationComponent and NOT a ChildAnimationComponent
			if (event->get_caller().name != Caller::Name::ANIMATION_COMPONENT)
				return;

			AnimationComponent* anim = static_cast<AnimationComponent*>(event->get_caller().pointer);
			if (this->game_object != anim->get_game_object())
				return;

		}
		break;
	}
	case EventID::ANIMATION_COMPLETE:
	{

		if (event->get_parameter())
		{
			// need to ensure this is a AnimationComponent and NOT a ChildAnimationComponent
			if (event->get_caller().name != Caller::Name::ANIMATION_COMPONENT)
				return;

			AnimationComponent* anim = static_cast<AnimationComponent*>(event->get_caller().pointer);
			if (this->game_object != anim->get_game_object())
				return;

			std::string anim_name = *static_cast<std::string*>(event->get_parameter());
			if (anim_name == "ATTACK_UP")
			{
				attack = false;
				this->anim.lock()->play("IDLE_UP");
			}
			else if (anim_name == "ATTACK_LEFT")
			{
				attack = false;
				this->anim.lock()->play("IDLE_LEFT");
			}
			else if (anim_name == "ATTACK_DOWN")
			{
				attack = false;
				this->anim.lock()->play("IDLE_DOWN");
			}
			else if (anim_name == "ATTACK_RIGHT")
			{
				attack = false;
				this->anim.lock()->play("IDLE_RIGHT");
			}
			// then check if the parameter = ATTACK_UP
				// play("idle_up")
		}
		break;
	}
	default:
		break;
	}
}

}