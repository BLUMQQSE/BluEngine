#include "pch.h"
#include "Physics.h"
#include "Collisions.h"
#include "GameObject.h"
#include "Tile.h"
namespace bm98::core
{
std::unordered_map<GameObject*, Physics::GameObjects> Physics::game_objects;
std::vector<Tile*> Physics::collidable_tiles;
bool Physics::collision_matrix[(int)(Layer::LAST_DONT_REMOVE)][(int)(Layer::LAST_DONT_REMOVE)];
bool Physics::triggers_acknowledge_colliders = true;

void Physics::init()
{
	init_matrix();
}

void Physics::add_to_physics(GameObject* game_object)
{
	// Could move code to check for game_object parent and child here
	// to prevent them from ever being added to a game objects collisions list
	GameObjects object_to_add;

	object_to_add.game_object = game_object;

	std::unordered_map<GameObject*, Collision_State> collisions_to_add;

	for (auto& i : game_objects)
	{
		collisions_to_add[i.second.game_object] = Collision_State::NOTHING;

		i.second.collisions.insert({ game_object, Collision_State::NOTHING });
	}

	object_to_add.collisions = collisions_to_add;

	game_objects[game_object] = object_to_add;
}

void Physics::add_tiles_to_physics(std::vector<Tile*> tiles)
{
	remove_tiles_from_physics();
	collidable_tiles = tiles;
}

void Physics::remove_from_physics(GameObject* game_object)
{
	// to remove first we must remove reference from all game_objects collisions
	for (auto i : game_objects)
	{
		i.second.collisions.erase(game_object);
	}
	// then remove from game_objects itself
	game_objects.erase(game_object);
}

void Physics::remove_tiles_from_physics()
{
	collidable_tiles.clear();
}

void Physics::clear_objects()
{
	game_objects.clear();
}

void Physics::fixed_update()
{
	//update needs run first to ensure collision states dont immediatly get overwritten
	update_collisions();
	check_collisions();
}

bool Physics::raycast(Vector2f origin, Vector2f direction, float distance, LayerMask mask)
{
	return false;
}

void Physics::init_matrix()
{
	for (int i = 0; i < (int)Layer::LAST_DONT_REMOVE; i++)
	{
		for (int x = 0; x < (int)Layer::LAST_DONT_REMOVE; x++)
		{
			collision_matrix[i][x] = true;
		}
	}
}

void Physics::check_collisions()
{
	for (auto& active_object : game_objects)
	{
		if (active_object.first == nullptr)
			continue;
		if (!active_object.first->has_component<BoxColliderComponent>())
			continue;
		if (!active_object.first->is_active())
			continue;

		BoxColliderComponent* active_collider = &active_object.first->get_component<BoxColliderComponent>();
		RigidbodyComponent* active_rigid = &active_object.first->get_component<RigidbodyComponent>();

		if (active_rigid)
			active_rigid->unhalt();
		if (!active_collider->is_active())
			continue;
		if (!triggers_acknowledge_colliders && active_collider->is_trigger())
			continue;


		/*
		* Here can handle tile collisions
		*/
		if (!active_collider->is_trigger())
		{
			for (auto c : collidable_tiles)
			{
				if (active_collider->check_intersect(c->get_bounds()))
				{
					//collision
					if (!collision_matrix[(int)active_object.first->get_info().layer][(int)c->get_layer()])
						break;

					//here we need to move active so that its outer bounds is one off of c->get_bounds()

					if (active_rigid->get_velocity().x > 0 && active_rigid->get_velocity().y == 0)
					{
						active_object.first->set_position(
							c->get_bounds().left - active_collider->get_offset().x - active_collider->get_bounds().width,
							active_object.first->get_transform().position.y
						);
						active_rigid->halt_right();
					}
					if (active_rigid->get_velocity().x < 0 && active_rigid->get_velocity().y == 0)
					{
						active_object.first->set_position(
							c->get_bounds().left + c->get_bounds().width - active_collider->get_offset().x,
							active_object.first->get_transform().position.y
						);
						active_rigid->halt_left();
					}
					if (active_rigid->get_velocity().y > 0 && active_rigid->get_velocity().x == 0)
					{
						active_object.first->set_position(
							active_object.first->get_transform().position.x,
							c->get_bounds().top - active_collider->get_offset().y - active_collider->get_bounds().height
						);
						active_rigid->halt_down();
					}
					if (active_rigid->get_velocity().y < 0 && active_rigid->get_velocity().x == 0)
					{
						active_object.first->set_position(
							active_object.first->get_transform().position.x,
							c->get_bounds().top + c->get_bounds().height - active_collider->get_offset().y
						);
						active_rigid->halt_up();
					}
				}
			}
		}

		for (auto& checking_object : active_object.second.collisions)
		{
			if (checking_object.first == nullptr)
				continue;
			if (!checking_object.first->has_component<BoxColliderComponent>())
				continue;
			if (!checking_object.first->is_active())
				continue;
			if (!collision_matrix[(int)active_object.first->get_info().layer][(int)checking_object.first->get_info().layer])
				continue;

			// Safe gaurd to prevent somehow active containing self reference in checking
			if (active_object.first == checking_object.first)
				continue;
			// if checking the parent of active_object, ingore the parent
			if (active_object.first->get_parent() == checking_object.first)
				continue;
			// if checking a child of active_object, ignore the child
			if (active_object.first->check_for_child(checking_object.first))
				continue;

			BoxColliderComponent* checking_collider = &checking_object.first->get_component<BoxColliderComponent>();

			if (!checking_collider)
				continue;
			if (!checking_collider->is_active())
				continue;
			if (active_collider->is_trigger() && checking_collider->is_trigger())
				continue;

			if (checking_collider->is_trigger())
			{
				if (!active_collider->check_outer_intersect(checking_collider->get_outer_bounds()))
				{
					if (checking_object.second == Collision_State::TRIGGER)
					{
						checking_object.second = Collision_State::TRIGGER_EXIT;
						active_object.second.game_object->on_trigger_exit(Collider(checking_object.first));
					}
					continue;
				}
				if (checking_object.second == Collision_State::NOTHING)
				{
					checking_object.second = Collision_State::TRIGGER_ENTRY;
					active_object.second.game_object->on_trigger_enter(Collider(checking_object.first));
					continue;
				}

				active_object.second.game_object->on_trigger_stay(Collider(checking_object.first));
				continue;
			}

			if (!active_collider->check_outer_intersect(checking_collider->get_outer_bounds()))
			{
				if (checking_object.second == Collision_State::COLLISION)
				{
					checking_object.second = Collision_State::COLLISION_EXIT;
					active_object.second.game_object->on_collision_exit(Collision(checking_object.first,
						checking_collider));
				}
				continue;
			}

			//here prevent moving into the collider and ensure does not phase through the object
			if (active_rigid)
			{
				active_rigid->set_velocity(sf::Vector2f(0, 0));

				// TODO: move everything below outside of the if statement
				//we want to move the game_object so that its active(inner.left + inner.width) == checking(outer.left)
				sf::FloatRect checking_outer = checking_collider->get_outer_bounds();
				sf::FloatRect active_inner = active_collider->get_bounds();

				sf::Vector2f velocity = active_rigid->get_velocity();

				//want to set position of active so that
					//: active_inner == checking_outer
				active_object.first->set_position(
					checking_outer.left - active_collider->get_offset().x - active_collider->get_bounds().width,
					active_object.first->get_transform().position.y
				);

			}




			if (checking_object.second == Collision_State::NOTHING)
			{
				checking_object.second = Collision_State::COLLISION_ENTRY;
				active_object.second.game_object->on_collision_enter(Collision(checking_object.first,
					&checking_object.first->get_component<BoxColliderComponent>()));
				continue;
			}

			active_object.second.game_object->on_collision_stay(Collision(checking_object.first,
				checking_collider));
			continue;
		}
	}
}

void Physics::update_collisions()
{
	for (auto& i : game_objects)
	{
		for (auto& it : i.second.collisions)
		{
			if (it.second == Collision_State::COLLISION
				|| it.second == Collision_State::TRIGGER
				|| it.second == Collision_State::NOTHING
				)
			{
				continue;
			}
			if (it.second == Collision_State::COLLISION_ENTRY)
			{
				it.second = Collision_State::COLLISION;
				continue;
			}
			switch (it.second)
			{
			case Collision_State::COLLISION_ENTRY:
				it.second = Collision_State::COLLISION;
				break;
			case Collision_State::TRIGGER_ENTRY:
				it.second = Collision_State::TRIGGER;
				break;
			case Collision_State::COLLISION_EXIT:
				it.second = Collision_State::NOTHING;
				break;
			case Collision_State::TRIGGER_EXIT:
				it.second = Collision_State::NOTHING;
				break;
			default:
				break;
			}
		}
	}

}
}