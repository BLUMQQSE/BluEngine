#pragma once
#include "globals.h"
#include "Math.h"
namespace bm98
{
class Collider;
class GameObject;
class Tile;
class LayerMask;
class ColliderComponent;
class FloatConvex;
}

namespace bm98::core
{

class Physics
{
public:

	struct RayHit
	{
		ColliderComponent* collider = nullptr;
		float distance = INFINITY;
	};

	enum class CollisionState
	{
		NOTHING,
		COLLISION_ENTRY,
		COLLISION,
		COLLISION_EXIT,
		TRIGGER_ENTRY,
		TRIGGER,
		TRIGGER_EXIT
	};

	static void init();


	static void add_to_physics(GameObject* game_object);
	static void remove_from_physics(GameObject* game_object);
	static void clear_objects();

	static void fixed_update();
	/// <summary>
	/// Creates a ray from origin in provided direction for provided distance.
	/// This function should be reserved for fixed_update and limited use in updates do to noticable
	/// performance drop.
	/// </summary>
	/// <returns>
	/// The closest collider which is not on a gameobject related to the calling gameobject.</returns>
	static bool raycast(Vector2f origin, Vector2f direction, GameObject* ignore,
		float distance = INFINITY, Global::LayerMask mask = Global::LayerMask(),
		RayHit* hit = nullptr);

	/// <summary>
	/// Creates a circle from origin with provided radius.
	/// This function should be reserved for fixed_update and limited use in updates do to noticable
	/// performance drop.
	/// </summary>
	/// <returns>
	/// All colliders within the circle not related to the object calling the functions.</returns>
	static int OverlapCircle(Vector2f pos, float radius, Global::LayerMask mask, 
		GameObject* object_to_ignore, std::vector<ColliderComponent*> collisions);

private:

	static std::vector<std::vector<std::pair<GameObject*, CollisionState>>> objects;

	static bool collision_matrix[(int)(Layer::_LAST_DONT_REMOVE)][(int)(Layer::_LAST_DONT_REMOVE)];

	static bool triggers_acknowledge_colliders;

	/// <summary>
	/// Initialize matrix double array with bool flags determining if layers acknowledge each other.
	/// </summary>
	static void init_matrix();
	/// <summary>
	/// Takes in two pairs of gameobjects and their collision states with each other. 
	/// These objects are compared for intersection and collisions are handled.
	/// </summary>
	static void handle_collision(std::pair<GameObject*, CollisionState>& a,
		std::pair<GameObject*, CollisionState>& b);
	static void update_collision_state(std::pair<GameObject*, CollisionState>& a,
		std::pair<GameObject*, CollisionState>& b);
};

}
