#include "pch.h"
#include "Physics.h"
#include "Collisions.h"
#include "GameObject.h"
#include "Tile.h"
#include "Math.h"
#include "CapsuleColliderComponent.h"
#include "TilemapColliderComponent.h"
namespace bm98::core
{
std::vector<std::vector<std::pair<GameObject*, Physics::CollisionState>>> Physics::objects;

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

	std::vector<std::pair<GameObject*, CollisionState>> g;
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

void Physics::remove_from_physics(GameObject* game_object)
{
	for (std::size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i][0].first == game_object)
		{
			objects.erase(std::find(objects.begin(), objects.end(), objects[i]));
			break;
		}
	}

	for (std::size_t i = 0; i < objects.size(); i++)
	{
		for (std::size_t x = 0; x < objects[i].size(); x++)
		{
			if (objects[i][x].first == game_object)
			{
				objects[i].erase(std::find(objects[i].begin(), objects[i].end(), objects[i][x]));
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
		std::vector<GameObject*> rel = objects[i][0].first->get_all_relatives();

		for (std::size_t x = addon; x < objects[i].size(); x++)
		{
			if (rel.size() != 0)
			{
				if (std::find(rel.begin(), rel.end(), objects[i][x].first) != rel.end())
				{
					continue;
				}
			}
			handle_collision(objects[x][i + 1], objects[i][x]);
		}
		addon++;
	}

}

bool Physics::raycast(Vector2f origin, Vector2f direction, GameObject* ignore, float distance, Global::LayerMask mask, RayHit* hit)
{
	if (distance == INFINITY)
		distance = 10000.0f;
	FloatConvex ray = FloatConvex::line(origin, origin + direction.get_normalized() * distance, 3);
	ray.setFillColor(sf::Color::Cyan);
	bool hit_something = false;
	std::vector<GameObject*> rel = ignore->get_all_relatives();
	for (std::size_t i = 0; i < objects.size(); i++)
	{

		if (!mask.layers[static_cast<int>(objects[i][0].first->get_info().layer)])
			continue;
		
		if (objects[i][0].first == ignore)
			continue;

		ColliderComponent* c = objects[i][0].first->get_component_of_type<ColliderComponent>();
		if (!c)
			continue;

		if (rel.size() != 0)
			if (std::find(rel.begin(), rel.end(), objects[i][0].first) != rel.end())
				continue;
		
		// TODO: change later to return distance from intersection point between origin and contact with collider
		if (Vector2f::sqr_distance(origin, objects[i][0].first->get_world_position())
			< hit->distance * hit->distance)
		{
			if (dynamic_cast<TilemapColliderComponent*>(c))
			{
				if (dynamic_cast<TilemapColliderComponent*>(c)->intersects(ray, mask))
				{
					hit->distance = Vector2f::distance(origin, objects[i][0].first->get_world_position());
					hit_something = true;
					hit->collider = c;
				}
			}
			else
			{
				if (ray.intersects(c->get_collider_bounds()))
				{
					hit->distance = Vector2f::distance(origin, objects[i][0].first->get_world_position());
					hit_something = true;
					hit->collider = c;
				}
			}
		}
	}

	Renderer::add_gizmo(ray);
	return hit_something;
}


int Physics::OverlapCircle(Vector2f pos, float radius, Global::LayerMask mask, GameObject* object_to_ignore, std::vector<ColliderComponent*> collisions)
{

	FloatConvex circle = FloatConvex::circle(pos, radius, 10);
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color::Cyan);
	circle.setOutlineThickness(2);

	std::vector<GameObject*> rel = object_to_ignore->get_all_relatives();

	for (std::size_t i = 0; i < objects.size(); i++)
	{
		if (!mask.layers[static_cast<int>(objects[i][0].first->get_info().layer)])
			continue;
			
		if (objects[i][0].first == object_to_ignore)
			continue;

		ColliderComponent* c = objects[i][0].first->get_component_of_type<ColliderComponent>();
		
		if (!c)
			continue;

		if (rel.size() != 0)
			if (std::find(rel.begin(), rel.end(), objects[i][0].first) != rel.end())
				continue;
		
		if (dynamic_cast<TilemapColliderComponent*>(c))
		{
			if (dynamic_cast<TilemapColliderComponent*>(c)->intersects(circle, mask))
			{
				collisions.push_back(c);
			}
		}
		else
		{
			if (circle.intersects(c->get_collider_bounds()))
			{
				collisions.push_back(c);
			}
		}

	}
	Renderer::add_gizmo(circle);
	return collisions.size();
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

void Physics::handle_collision(std::pair<GameObject*, CollisionState>& a,
	std::pair<GameObject*, CollisionState>& b)
{

	update_collision_state(a, b);



	if (!a.first->is_active() || !b.first->is_active())
		return;

	if (!a.first->has_component<BoxColliderComponent>() && !a.first->has_component<CapsuleColliderComponent>() ||
		!b.first->has_component<BoxColliderComponent>() && !b.first->has_component<CapsuleColliderComponent>())
		return;

	ColliderComponent* a_collider = a.first->get_component_of_type<ColliderComponent>();
	ColliderComponent* b_collider = b.first->get_component_of_type<ColliderComponent>();

	RigidbodyComponent* a_rigid = &a.first->get_component<RigidbodyComponent>();
	RigidbodyComponent* b_rigid = &b.first->get_component<RigidbodyComponent>();

	if (a_collider->is_trigger() && b_collider->is_trigger())
		return;
	if (!a_collider->is_active() || !b_collider->is_active())
		return;

	Vector2f collision_overlap = FloatConvex::intersection(a_collider->get_collider_bounds(), b_collider->get_collider_bounds());

	// Collision does not exist this update, check if a collider has exited and return
	if (collision_overlap == Vector2f::infinity())
	{
		if (a.second == CollisionState::TRIGGER)
		{
			a.second = CollisionState::TRIGGER_EXIT;
			a.first->on_trigger_exit(Collider(b.first));
		}
		if (a.second == CollisionState::COLLISION)
		{
			a.second = CollisionState::COLLISION_EXIT;
			a.first->on_collision_exit(Collision(b.first,
				b_collider));
		}
		if (b.second == CollisionState::TRIGGER)
		{
			b.second = CollisionState::TRIGGER_EXIT;
			b.first->on_trigger_exit(Collider(a.first));
		}
		if (b.second == CollisionState::COLLISION)
		{
			b.second = CollisionState::COLLISION_EXIT;
			b.first->on_collision_exit(Collision(a.first,
				a_collider));
		}
		return;
	}

	// a: uses collision_overlap as positive
	// b: uses collision_overlap as negative
	

	
}


void Physics::update_collision_state(std::pair<GameObject*, CollisionState>& a, std::pair<GameObject*, CollisionState>& b)
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
}