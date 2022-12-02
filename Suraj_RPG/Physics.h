#pragma once
#include "globals.h"
namespace bm98
{
class GameObject;
class Tile;
}
namespace bm98::core
{

class Physics
{
public:

	enum class Collision_State
	{
		NOTHING,
		COLLISION_ENTRY,
		COLLISION,
		COLLISION_EXIT,
		TRIGGER_ENTRY,
		TRIGGER,
		TRIGGER_EXIT
	};

	struct GameObjects
	{
		GameObject* game_object;
		std::unordered_map<GameObject*, Collision_State> collisions;
	};

	static void init();


	static void add_to_physics(GameObject* game_object);
	static void add_tiles_to_physics(std::vector<Tile*> tiles);
	static void remove_from_physics(GameObject* game_object);
	static void remove_tiles_from_physics();
	static void clear_objects();

	static void fixed_update();

private:

	static std::unordered_map<GameObject*, GameObjects> game_objects;
	static std::vector<Tile*> collidable_tiles;
	static bool collision_matrix[(int)(Layer::LAST_DONT_REMOVE)][(int)(Layer::LAST_DONT_REMOVE)];

	static bool triggers_acknowledge_colliders;

	static void init_matrix();
	/// <summary>
	/// Checks if any new collisions have occurred
	/// </summary>
	static void check_collisions();
	/// <summary>
	/// Checks all current active collisions and updates their state
	/// </summary>
	static void update_collisions();
};

}
