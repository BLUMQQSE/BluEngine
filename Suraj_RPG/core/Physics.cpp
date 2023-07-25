#include "../pch.h"
#include "Physics.h"
#include "../Collisions.h"
#include "../GameObject.h"
#include "../tiles/Tile.h"
#include "../TilemapColliderComponent.h"
#include "../Light.h"
namespace bm98::core
{

sf::Color Gizmo::outline_color = sf::Color::White;
sf::Color Gizmo::fill_color = sf::Color::Transparent;
float Gizmo::outline_thickness = 1.f;
sf::View* Gizmo::view;

void Physics::init()
{
	init_matrix();
}

void Physics::add_to_physics(std::shared_ptr<GameObject> game_object)
{
	// check if already in physics
	for (std::size_t i = 0; i < objects.size(); i++)
	{
		// dont add again
		if (objects[i][0].first.lock() == game_object)
			return;
	}

	// Could move code to check for game_object parent and child here
	// to prevent them from ever being added to a game objects collisions list

	std::vector<std::pair<std::weak_ptr<GameObject>, CollisionState>> g;
	g.push_back(std::make_pair(game_object, CollisionState::NOTHING));
	for (std::size_t i = 0; i < objects.size(); i++)
	{
		// add other gameobjects to new gameobject list
		g.push_back(std::make_pair(objects[i][0].first, CollisionState::NOTHING));
		// add new gameobject to other objects lists
		objects[i].push_back(std::make_pair(game_object, CollisionState::NOTHING));
	}

	objects.push_back(g);


}

void Physics::remove_from_physics(std::shared_ptr<GameObject> game_object)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->at(0).first.lock() == game_object)
		{
			objects.erase(it);
			break;
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		for (auto it = objects[i].begin(); it != objects[i].end(); ++it)
		{
			if (it->first.lock() == game_object)
			{
				objects[i].erase(it);
				break;
			}
		}
	}

}

void Physics::clear_objects()
{
	objects.clear();
}

void Physics::fixed_update()
{
	int addon = 1;
	for (std::size_t i = 0; i < objects.size(); i++)
	{
		std::vector<std::weak_ptr<GameObject>> rel = objects[i][0].first.lock()->get_all_relatives();

		for (std::size_t x = addon; x < objects[i].size(); x++)
		{
			if (rel.size() != 0)
			{
				GameObject* go = objects[i][x].first.lock().get();
				std::vector<std::weak_ptr<GameObject>>::iterator it = std::find_if(rel.begin(), rel.end(),
																				   [go](std::weak_ptr<GameObject> const& i) { return i.lock().get() == go; });
				// Objects are related, so skip comparing
				if (it != rel.end())
				{
					continue;
				}
			}
			handle_collision(objects[x][i + 1], objects[i][x]);
		}
		addon++;
	}

}

bool Physics::raycast(Vector2f origin, Vector2f direction, std::shared_ptr<GameObject> ignore, CollisionCheckType check_type,
					  float distance, PhysicsNS::LayerMask mask, std::shared_ptr<RayHit> hit)
{
	if (distance == INFINITY)
		distance = 10000.0f;
	
	FloatConvex ray = FloatConvex::Line(origin, origin + direction.get_normalized() * distance, 1);
	
	bool hit_something = false;
	Intersect current_intersect;
	
	std::vector<std::weak_ptr<GameObject>> rel = ignore->get_all_relatives();
	LineIntersect last_saved_intersect;
	for (std::size_t i = 0; i < objects.size(); i++)
	{
#pragma region Verify GameObject is valid to check
	
		if (!mask[objects[i][0].first.lock()->get_info().layer])
			continue;
		if (objects[i][0].first.lock() == ignore)
			continue;
		if (!objects[i][0].first.lock()->get_component_of_type<ColliderComponent>().lock()->is_active())
			continue;

		switch (check_type)
		{
			case bm98::core::Physics::CollisionCheckType::TRIGGER:
				if (!objects[i][0].first.lock()->get_component_of_type<ColliderComponent>().lock()->is_trigger())
					continue;
				break;
			case bm98::core::Physics::CollisionCheckType::COLLIDER:
				if (objects[i][0].first.lock()->get_component_of_type<ColliderComponent>().lock()->is_trigger())
					continue;
				break;
		}

		std::weak_ptr<ColliderComponent> c = objects[i][0].first.lock()->get_component_of_type<ColliderComponent>();
		if (!c.lock())
			continue;

		std::weak_ptr<GameObject> go = objects[i][0].first;

		std::vector<std::weak_ptr<GameObject>>::iterator it = std::find_if(rel.begin(), rel.end(),
																		   [go](std::weak_ptr<GameObject> const& i) { return i.lock().get() == go.lock().get(); });
		if (rel.size() != 0)
			if (it != rel.end())
				continue;
#pragma endregion
		// by here we know this object 1) exists : 2) has collider : 3) is not related to ignore
		
		
		if (!dynamic_cast<TilemapColliderComponent*>(c.lock().get()))
		{
			LineIntersect temp_intersect = c.lock().get()->get_collider_bounds().line_intersects(origin, origin + (direction.get_normalized() * distance));
			
			if (!temp_intersect.collision_exists)
				continue;

			if (last_saved_intersect.collision_exists)
			{
				if (Vector2f::SqrDistance(origin, temp_intersect.get_contact_center()) < Vector2f::SqrDistance(origin, last_saved_intersect.get_contact_center()))
				{
					last_saved_intersect = temp_intersect;
					if (hit)
					{
						hit->collider = c;
					}
				}
			}
			else
			{
				last_saved_intersect = temp_intersect;
				hit_something = true;
				if (hit)
				{
					hit->collider = c;
				}
			}
		}
		continue;
		/*
		Intersect intersect;
		Vector2f intersect_contact_center;
		if (dynamic_cast<TilemapColliderComponent*>(c.lock().get()))
		{
			//intersect = dynamic_cast<TilemapColliderComponent*>(c.lock().get())->intersects(ray, mask);
			//TilemapIntersect t = intersect;
			//intersect_contact_center = t.get_contact_center();
		}
		else
		{
			//ObjectIntersect o = intersect;
			intersect = FloatConvex::Intersection(ray, c.lock()->get_collider_bounds(), direction);
			intersect_contact_center = intersect.get_contact_center();
		}
		if (current_intersect.collision_exists)
		{
			if (intersect.collision_exists)
			{
				// We already found a collision, compare if the new collision is closer to origin
				if (Vector2f::SqrDistance(origin, intersect_contact_center) < Vector2f::SqrDistance(origin, current_intersect.get_contact_center()))
				{
					current_intersect = intersect;
					hit_something = true;
					if (hit)
					{
					
						hit->collider = c;
					}
					else
					{
						int x = 0;
					}
				}
				else
				{
					int y = 12;
				}
			}
		}
		else
		{
			if (intersect.collision_exists)
			{
				current_intersect = intersect;
				hit_something = true;
				if (hit)
				{
					hit->collider = c;
				}
			}
		}
		*/
	}

	return hit_something;
}


int Physics::OverlapCircle(Vector2f pos, float radius, PhysicsNS::LayerMask mask,
	std::shared_ptr<GameObject> object_to_ignore, CollisionCheckType check_type, std::vector<std::weak_ptr<ColliderComponent>>& collisions)
{
	FloatConvex circle = FloatConvex::Circle(pos, radius, 30);
	return OverlapConvex(circle, mask, object_to_ignore, check_type, collisions);
}

int Physics::OverlapConvex(FloatConvex& shape, PhysicsNS::LayerMask mask, std::shared_ptr<GameObject> object_to_ignore,
						   CollisionCheckType check_type, std::vector<std::weak_ptr<ColliderComponent>>& collisions)
{
	std::vector<std::weak_ptr<GameObject>> rel = object_to_ignore->get_all_relatives();

	for (std::size_t i = 0; i < objects.size(); i++)
	{
		if (!mask[objects[i][0].first.lock()->get_info().layer])
			continue;
		if (objects[i][0].first.lock() == object_to_ignore)
			continue;
		std::weak_ptr<ColliderComponent> c = objects[i][0].first.lock()->get_component_of_type<ColliderComponent>();
		if (!c.lock())
			continue;
		if (!c.lock()->is_active())
			continue;
		switch (check_type)
		{
			case bm98::core::Physics::CollisionCheckType::TRIGGER:
			{	if (!c.lock()->is_trigger())
					continue;
			}
				break;
			case bm98::core::Physics::CollisionCheckType::COLLIDER:
			{
				if (c.lock()->is_trigger())
					continue;
			}	break;
		}

		std::weak_ptr<GameObject> go = objects[i][0].first;
		std::vector<std::weak_ptr<GameObject>>::iterator it = std::find_if(rel.begin(), rel.end(),
																		   [go](std::weak_ptr<GameObject> const& i) { return i.lock().get() == go.lock().get(); });

		if (rel.size() != 0)
			if (it != rel.end())
				continue;

		Intersect intersect;

		if (dynamic_cast<TilemapColliderComponent*>(c.lock().get()))
			intersect = dynamic_cast<TilemapColliderComponent*>(c.lock().get())->intersects(shape, mask);
		else
			intersect = FloatConvex::Intersection(shape, c.lock()->get_collider_bounds());
		
		if (intersect.collision_exists)
			collisions.push_back(c);


	}
	return collisions.size();
} 

Physics::Physics()
{
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_PHYSICS_INITIALIZE_, this);
	EventSystem::Instance()->subscribe(EventID::_SYSTEM_PHYSICS_FIXED_UPDATE_, this);
}

void Physics::init_matrix()
{
	for (int i = 0; i < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); i++)
	{
		for (int x = 0; x < (int)PhysicsNS::Layer::_LAST_DONT_REMOVE; x++)
		{
			if (i == static_cast<int>(PhysicsNS::Layer::PHYSICS_IGNORE) || x ==
				static_cast<int>(PhysicsNS::Layer::PHYSICS_IGNORE))
			{
				collision_matrix[i][x] = false;
				continue;
			}
			collision_matrix[i][x] = true;
		}
	}
}

void Physics::handle_collision(std::pair<std::weak_ptr<GameObject>, CollisionState>& a,
	std::pair<std::weak_ptr<GameObject>, CollisionState>& b)
{
	update_collision_state(a, b);

	if (!a.first.lock()->is_active() || !b.first.lock()->is_active())
		return;

	std::weak_ptr<ColliderComponent> a_collider = a.first.lock()->get_component_of_type<ColliderComponent>();
	std::weak_ptr<ColliderComponent> b_collider = b.first.lock()->get_component_of_type<ColliderComponent>();

	if (!a_collider.lock() || !b_collider.lock())
		return;
	// Currently gonna go with triggers can trigger triggers
	if (a_collider.lock()->is_trigger() && b_collider.lock()->is_trigger())
		return;
	if (!a_collider.lock()->is_active() || !b_collider.lock()->is_active())
		return;

	if (dynamic_cast<TilemapColliderComponent*>(a_collider.lock().get()) && dynamic_cast<TilemapColliderComponent*>(b_collider.lock().get()))
		return;
	if (dynamic_cast<TilemapColliderComponent*>(a_collider.lock().get()))
	{
		//handle_tilemap_collision(b, a);
		return;
	}
	if (dynamic_cast<TilemapColliderComponent*>(b_collider.lock().get()))
	{
		//handle_tilemap_collision(a, b);
		return;
	}

	std::weak_ptr<GameObject> a_rigid_gameobject = a.first;
	std::weak_ptr<GameObject> b_rigid_gameobject = b.first;
	
	while (a_rigid_gameobject.lock()->get_parent().lock())
	{
		if (a_rigid_gameobject.lock()->has_component<RigidbodyComponent>())
			break;

		a_rigid_gameobject = a_rigid_gameobject.lock()->get_parent();
	}
	while (b_rigid_gameobject.lock()->get_parent().lock())
	{
		if (b_rigid_gameobject.lock()->has_component<RigidbodyComponent>())
			break;

		b_rigid_gameobject = b_rigid_gameobject.lock()->get_parent();
	}

	std::weak_ptr<RigidbodyComponent> a_rigid = a_rigid_gameobject.lock()->get_component<RigidbodyComponent>();
	std::weak_ptr<RigidbodyComponent> b_rigid = b_rigid_gameobject.lock()->get_component<RigidbodyComponent>();

	if (!a_rigid.lock() || !b_rigid.lock())
		return;

	Intersect collision = FloatConvex::Intersection(a_collider.lock()->get_collider_bounds(),
														   b_collider.lock()->get_collider_bounds());

	// Collision does not exist this update, check if a collider has exited and return
	if (!collision.collision_exists)
	{
		if (a.second == CollisionState::TRIGGER)
		{
			a.second = CollisionState::TRIGGER_EXIT;
			a.first.lock()->on_trigger_exit(Collider(b.first.lock()));
			b.first.lock()->on_other_trigger_exit(Collider(a.first.lock()));
		}
		if (a.second == CollisionState::COLLISION)
		{
			a.second = CollisionState::COLLISION_EXIT;
			a.first.lock()->on_collision_exit(Collision(b.first.lock(),
				b_collider.lock()));
		}
		if (b.second == CollisionState::TRIGGER)
		{
			b.second = CollisionState::TRIGGER_EXIT;
			b.first.lock()->on_trigger_exit(Collider(a.first.lock()));
			a.first.lock()->on_other_trigger_exit(Collider(b.first.lock()));
		}
		if (b.second == CollisionState::COLLISION)
		{
			b.second = CollisionState::COLLISION_EXIT;
			b.first.lock()->on_collision_exit(Collision(a.first.lock(),
				a_collider.lock()));
		}
		return;
	}
	else
	{

		// Here need all logic for switching from nothing to entry
		//WRONG, need to look at alternative to see if trigger entry

		
		//need a pre test to check if either are triggers
		if (a_collider.lock()->is_trigger())
		{
			if (a.second == CollisionState::NOTHING)
			{
				a.second = CollisionState::TRIGGER_ENTRY;
				a.first.lock()->on_trigger_enter(Collider(b.first.lock()));
				b.first.lock()->on_other_trigger_enter(Collider(a.first.lock()));
			}
			else
			{
				a.first.lock()->on_trigger_stay(Collider(b.first.lock()));
				b.first.lock()->on_other_trigger_stay(Collider(a.first.lock()));
			}
		}
		else if (b_collider.lock()->is_trigger())
		{
			if (b.second == CollisionState::NOTHING)
			{
				b.second = CollisionState::TRIGGER_ENTRY;
				b.first.lock()->on_trigger_enter(Collider(a.first.lock()));
				a.first.lock()->on_other_trigger_enter(Collider(b.first.lock()));
			}
			else
			{
				b.first.lock()->on_trigger_stay(Collider(a.first.lock()));
				a.first.lock()->on_other_trigger_stay(Collider(b.first.lock()));
			}
		}
		else
		{
			if (a.second == CollisionState::NOTHING)
			{
				a.second = CollisionState::COLLISION_ENTRY;
				a.first.lock()->on_collision_enter(Collision(b.first.lock(),
															b_collider.lock()));
			}
			else
				a.first.lock()->on_collision_stay(Collision(b.first.lock(),
												   b_collider.lock()));

			if (b.second == CollisionState::NOTHING)
			{
				b.second = CollisionState::COLLISION_ENTRY;
				b.first.lock()->on_collision_enter(Collision(a.first.lock(),
												   a_collider.lock()));
			}
			else
				b.first.lock()->on_collision_stay(Collision(a.first.lock(),
												   a_collider.lock()));

		}


	}
	
	if (a_collider.lock()->is_trigger() || b_collider.lock()->is_trigger())
		return;

	float a_velocity_abs = a_rigid.lock()->get_velocity() + a_rigid.lock()->get_external_velocity();
	float b_velocity_abs = b_rigid.lock()->get_velocity() + b_rigid.lock()->get_external_velocity();

	float total_velocity = a_velocity_abs + b_velocity_abs;

	if (total_velocity == 0)
		return;

	float a_velocity_percent = a_velocity_abs / total_velocity;
	float b_velocity_percent = 1 - a_velocity_percent;

	// a: uses collision_overlap as positive
	// b: uses collision_overlap as negative
	Vector2f a_movement = collision.penetration_vector * a_velocity_percent;
	Vector2f b_movement = -collision.penetration_vector * b_velocity_percent;

	((ObjectIntersect)collision).calculate_contacts(a_movement, b_movement);


	a_rigid_gameobject.lock()->move(a_movement);
	b_rigid_gameobject.lock()->move(b_movement);

	a_rigid.lock()->halt(-collision.penetration_vector.get_normalized());
	b_rigid.lock()->halt(collision.penetration_vector.get_normalized());

}

void Physics::handle_tilemap_collision(std::pair<std::weak_ptr<GameObject>, CollisionState>& a, std::pair<std::weak_ptr<GameObject>, CollisionState>& tilemap)
{
	std::weak_ptr<ColliderComponent> a_collider = a.first.lock()->get_component_of_type<ColliderComponent>();
	std::weak_ptr<ColliderComponent> tilemap_collider = tilemap.first.lock()->get_component_of_type<ColliderComponent>();

	std::weak_ptr<GameObject> a_rigid_gameobject = a.first;

	while (a_rigid_gameobject.lock()->get_parent().lock())
	{
		if (a_rigid_gameobject.lock()->has_component<RigidbodyComponent>())
			break;

		a_rigid_gameobject = a_rigid_gameobject.lock()->get_parent();
	}
	
	std::weak_ptr<RigidbodyComponent> a_rigid = a_rigid_gameobject.lock()->get_component<RigidbodyComponent>();
	if (!a_rigid.lock())
		return;

	PhysicsNS::LayerMask mask {true};

	Vector2f collision_overlap = tilemap.first.lock()->get_component<TilemapColliderComponent>().lock()->intersects(
		a_collider.lock()->get_collider_bounds(), mask).penetration_vector;

	// Collision does not exist this update, check if a collider has exited and return
	if (collision_overlap == Vector2f::Infinity())
	{
		if (a.second == CollisionState::TRIGGER)
		{
			a.second = CollisionState::TRIGGER_EXIT;
			a.first.lock()->on_trigger_exit(Collider(tilemap.first.lock()));
			tilemap.first.lock()->on_other_trigger_exit(Collider(a.first.lock()));
		}
		if (a.second == CollisionState::COLLISION)
		{
			a.second = CollisionState::COLLISION_EXIT;
			a.first.lock()->on_collision_exit(Collision(tilemap.first.lock(),
											  tilemap_collider.lock()));
		}
		if (tilemap.second == CollisionState::TRIGGER)
		{
			tilemap.second = CollisionState::TRIGGER_EXIT;
			tilemap.first.lock()->on_trigger_exit(Collider(a.first.lock()));
			a.first.lock()->on_other_trigger_exit(Collider(tilemap.first.lock()));
		}
		if (tilemap.second == CollisionState::COLLISION)
		{
			tilemap.second = CollisionState::COLLISION_EXIT;
			tilemap.first.lock()->on_collision_exit(Collision(a.first.lock(),
											  a_collider.lock()));
		}
		return;
	}

	if (a_collider.lock()->is_trigger() || tilemap_collider.lock()->is_trigger())
		return;

	a_rigid_gameobject.lock()->move(collision_overlap);

	a_rigid.lock()->halt(-collision_overlap.get_normalized());
}

void Physics::update_collision_state(std::pair<std::weak_ptr<GameObject>, CollisionState>& a,
									 std::pair<std::weak_ptr<GameObject>, CollisionState>& b)
{
	switch (a.second)
	{
	case CollisionState::COLLISION_ENTRY:
		a.second = CollisionState::COLLISION;
		break;
	case CollisionState::TRIGGER_ENTRY:
		a.second = CollisionState::TRIGGER;
		break;
	case CollisionState::COLLISION_EXIT:
		a.second = CollisionState::NOTHING;
		break;
	case CollisionState::TRIGGER_EXIT:
		a.second = CollisionState::NOTHING;
		break;
	default:
		break;
	}
	switch (b.second)
	{
	case CollisionState::COLLISION_ENTRY:
		b.second = CollisionState::COLLISION;
		break;
	case CollisionState::TRIGGER_ENTRY:
		b.second = CollisionState::TRIGGER;
		break;
	case CollisionState::COLLISION_EXIT:
		b.second = CollisionState::NOTHING;
		break;
	case CollisionState::TRIGGER_EXIT:
		b.second = CollisionState::NOTHING;
		break;
	default:
		break;
	}
}
void Physics::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
	case EventID::_SYSTEM_PHYSICS_INITIALIZE_:
		init();
		break;
	case EventID::_SYSTEM_PHYSICS_FIXED_UPDATE_:
		fixed_update();
		break;
	}
}

}