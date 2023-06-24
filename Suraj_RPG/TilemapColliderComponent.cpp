#include "pch.h"
#include "TilemapColliderComponent.h"
#include "TilemapComponent.h"
#include "tiles/Tile.h"
#include "GameObject.h"
namespace bm98
{



TilemapColliderComponent::TilemapColliderComponent()
{
}

TilemapColliderComponent::~TilemapColliderComponent()
{
}

void TilemapColliderComponent::awake()
{
	tilemap = game_object->get_component<TilemapComponent>();
	for (int i = 0; i < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); i++)
	{
		colliders.push_back(std::vector<FloatConvex>());
	}
}

void TilemapColliderComponent::start()
{
	create_colliders();
}

void TilemapColliderComponent::on_draw_gizmos_selected()
{
	for (int h = 0; h < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); h++)
	{
		for (std::size_t i = 0; i < colliders[h].size(); i++)
		{
			Gizmo::outline_color = Color::Red;
			Gizmo::fill_color = Color::Transparent;
			Gizmo::draw_convex(colliders[h][i]);
			Gizmo::fill_color = Gizmo::outline_color;
			Gizmo::outline_color = Color::Transparent;
			Gizmo::draw_line(colliders[h][i].get_position(), colliders[h][i].get_position() + Vector2f(colliders[h][i].getGlobalBounds().width, colliders[h][i].getGlobalBounds().height));
		}
	}
}

TilemapIntersect TilemapColliderComponent::intersects(const FloatConvex collider, PhysicsNS::LayerMask mask)
{
	TilemapIntersect result;
	Vector2f resolution = Vector2f::Zero();
	for (int h = 0; h < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); h++)
	{
		if (!mask[h])
			continue;
		for (int i = 0; i < colliders[h].size(); i++)
		{
			ObjectIntersect temp = FloatConvex::Intersection(collider, colliders[h][i]);
			if (temp.collision_exists)
			{
				if (result.collision_exists)
				{
					result.penetration_vector += temp.penetration_vector;
					result.convex_2.push_back(temp.convex_2);
				}
				else
				{
					result = temp;
				}
			}
		}
	}

	return result;
}

Vector2f TilemapColliderComponent::get_closest_point(Vector2f pos)
{
	Vector2f closest = Vector2f::Infinity();
	for (int h = 0; h < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); h++)
	{
		for (int i = 0; i < colliders[h].size(); i++)
		{
			Vector2f temp = colliders[h][i].get_closest_point(pos);
			if (closest == Vector2f::Infinity())
				closest = temp;
			else
				if (Vector2f::Distance(temp, pos) < Vector2f::Distance(closest, pos))
					closest = temp;
		}
	}

	return closest;
}

void TilemapColliderComponent::create_colliders()
{
	std::vector<Tile*> t = tilemap.lock().get()->get_collidable_tiles();

	for (int i = 0; i < t.size(); i++)
	{
		colliders[static_cast<int>(t[i]->get_layer())].push_back(t[i]->get_collider());
	}

	/*
	std::vector<std::vector<std::vector<Tile*>>> map = tilemap->get_tiles();
	for (int z = 0; z < map[0][0].size(); z++)
	{
		for (int y = 0; y < map[0].size(); y++)
		{
			for (int x = 0; x < map.size(); x++)
			{
				if (map[x][y][z])
				{
					if (map[x][y][z]->get_collision())
					{
						FloatConvex g = helper(x, y, z, map);
						int layer = static_cast<int>(tilemap->get_layer(static_cast<SortingLayer>(z)));
						colliders[layer].push_back(g);
					}
				}
			}
		}
	}
	*/
	
}

FloatConvex TilemapColliderComponent::helper(int x, int y, int z, std::vector<std::vector<std::vector<Tile*>>> map)
{
	FloatConvex f;
	f = map[x][y][z]->get_collider();
	map[x][y][z] = nullptr;
	if (x > 0)
	{
		if (map[x-1][y][z])
		{
			if (map[x - 1][y][z]->get_collision())
			{
				map[x][y][z] = nullptr;
				FloatConvex::CombineShapes(f, helper(x-1, y, z, map));
			}
		}
	}
	if (y < map[x].size())
	{
		if (map[x][y + 1][z])
		{
			if (map[x][y + 1][z]->get_collision())
			{
				map[x][y+1][z] = nullptr;
				FloatConvex::CombineShapes(f, helper(x, y+1, z, map));
			}
		}
	}
	if (x < map.size())
	{
		if (map[x + 1][y][z])
		{
			if (map[x + 1][y][z]->get_collision())
			{
				map[x+1][y][z] = nullptr;
				FloatConvex::CombineShapes(f, helper(x+1, y, z, map));
			}
		}
	}
	if (y > 0)
	{
		if (map[x][y - 1][z])
		{
			if (map[x][y - 1][z]->get_collision())
			{
				map[x][y - 1][z] = nullptr;
				FloatConvex::CombineShapes(f, helper(x, y - 1, z, map));
			}
		}
	}

	return f;
}


}