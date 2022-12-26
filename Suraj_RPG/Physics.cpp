#include "pch.h"
#include "Physics.h"
#include "Collisions.h"
#include "GameObject.h"
#include "Tile.h"
namespace bm98::core
{
std::unordered_map<GameObject*, Physics::GameObjects> Physics::game_objects;
std::vector<Tile*> Physics::collidable_tiles;
bool Physics::collision_matrix[(int)(Layer::_LAST_DONT_REMOVE)][(int)(Layer::_LAST_DONT_REMOVE)];
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

	//std::cout << "gameobjects before adding " << game_objects.size() <<"\n";
	game_objects[game_object] = object_to_add;
	//std::cout << "gameobjects after adding " << game_objects.size() << "\n";
	
}

void Physics::add_tiles_to_physics(std::vector<Tile*> tiles)
{
	remove_tiles_from_physics();
	collidable_tiles = tiles;
}

void Physics::remove_from_physics(GameObject* game_object)
{
	for (auto& i : game_objects)
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

bool Physics::raycast(Vector2f origin, Vector2f direction, float distance, Global::LayerMask mask)
{
	return false;
}

std::vector<BoxColliderComponent*> Physics::OverlapCircle(Vector2f pos, float radius, Global::LayerMask mask, GameObject* object_to_ignore)
{
	std::vector<BoxColliderComponent*> colliders;
	for (auto& obj : game_objects)
	{
		if (obj.first == nullptr)
			continue;
		if (!obj.first->has_component<BoxColliderComponent>())
			continue;
		if (obj.first == object_to_ignore || !mask.layers[static_cast<int>(obj.first->get_info().layer)] ||
			obj.first->get_info().layer == Layer::PHYSICS_IGNORE)
			continue;

		if (Vector2f::distance((obj.first->get_transform().position + obj.first->get_transform().local_position), pos) <= radius &&
			obj.first->has_component<BoxColliderComponent>())
			colliders.push_back(&obj.first->get_component<BoxColliderComponent>());
	}
	return colliders;
}

void Physics::init_matrix()
{
	for (int i = 0; i < static_cast<int>(Layer::_LAST_DONT_REMOVE); i++)
	{
		for (int x = 0; x < (int)Layer::_LAST_DONT_REMOVE; x++)
		{
			if (i == static_cast<int>(Layer::PHYSICS_IGNORE) || x == static_cast<int>(Layer::PHYSICS_IGNORE))
			{
				collision_matrix[i][x] = false;
				continue;
			}
			collision_matrix[i][x] = true;
		}
	}
}

void Physics::check_collisions()
{
	// need to work out how I'll iterate over this
	for (auto& active_object : game_objects)
	{
		if (active_object.first == nullptr)
		{
			std::cout << "null object as active object\n";
			continue;
		}
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


		// Here can handle tile collisions
		
		/*
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
						active_rigid->set_velocity(Vector2f(0, 0));
						active_rigid->halt_right();
					}
					else if (active_rigid->get_velocity().x < 0 && active_rigid->get_velocity().y == 0)
					{
						active_object.first->set_position(
							c->get_bounds().left + c->get_bounds().width - active_collider->get_offset().x,
							active_object.first->get_transform().position.y
						);
						active_rigid->set_velocity(Vector2f(0, 0));
						active_rigid->halt_left();
					}
					else if (active_rigid->get_velocity().y > 0 && active_rigid->get_velocity().x == 0)
					{
						active_object.first->set_position(
							active_object.first->get_transform().position.x,
							c->get_bounds().top - active_collider->get_offset().y - active_collider->get_bounds().height
						);
						active_rigid->set_velocity(Vector2f(0, 0));
						active_rigid->halt_down();
					}
					else if (active_rigid->get_velocity().y < 0 && active_rigid->get_velocity().x == 0)
					{
						active_object.first->set_position(
							active_object.first->get_transform().position.x,
							c->get_bounds().top + c->get_bounds().height - active_collider->get_offset().y
						);
						active_rigid->set_velocity(Vector2f(0, 0));
						active_rigid->halt_up();
					}
				}
			}
		}
		*/

		for (auto& checking_object : active_object.second.collisions)
		{
			if (checking_object.first == nullptr)
			{
				std::cout << "checking object is null\n";
				continue;
			}
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

			// when it comes to freezing position, should freeze by parent most object
			/*
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
			*/

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