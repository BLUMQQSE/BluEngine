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
	tilemap = game_object->get_component<TilemapComponent>().lock().get();
	for (int i = 0; i < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); i++)
	{
		colliders.push_back(std::vector<FloatConvex>());
	}
}

void TilemapColliderComponent::start()
{
	create_colliders();
}

bool TilemapColliderComponent::intersects(const FloatConvex collider, PhysicsNS::LayerMask mask)
{
	for (int h = 0; h < static_cast<int>(PhysicsNS::Layer::_LAST_DONT_REMOVE); h++)
	{
		if (mask != h)
			continue;
		for (int i = 0; i < colliders[h].size(); i++)
		{
			if (FloatConvex::Intersection(colliders[h][i], collider) != Vector2f::Infinity())
				return true;
		}
	}
	return false;
}

void TilemapColliderComponent::create_colliders()
{
	std::vector<Tile*> t = tilemap->get_collidable_tiles();

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