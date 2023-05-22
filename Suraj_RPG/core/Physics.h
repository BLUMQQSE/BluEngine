#pragma once
#include "../globals.h"
#include "../Math.h"
#include "EventSystem.h"
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

class Physics: public Listener
{
public:
	static Physics* Instance()
	{
		static Physics instance;
		return &instance;
	}

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

	void add_to_physics(std::shared_ptr<GameObject> game_object);
	void remove_from_physics(std::shared_ptr<GameObject> game_object);
	void clear_objects();

	/// <summary>
	/// Creates a ray from origin in provided direction for provided distance.
	/// This function should be reserved for fixed_update and limited use in updates do to noticable
	/// performance drop.
	/// </summary>
	/// <returns>
	/// The closest collider which is not on a gameobject related to the calling gameobject.</returns>
	bool raycast(Vector2f origin, Vector2f direction, std::shared_ptr<GameObject> ignore,
		float distance = INFINITY, PhysicsNS::LayerMask mask = PhysicsNS::LayerMask(),
		std::shared_ptr<RayHit> hit = std::shared_ptr<RayHit>(nullptr));

	/// <summary>
	/// Creates a circle from origin with provided radius.
	/// This function should be reserved for fixed_update and limited use in updates do to noticable
	/// performance drop.
	/// </summary>
	/// <returns>
	/// All colliders within the circle not related to the object calling the functions.</returns>
	int OverlapCircle(Vector2f pos, float radius, PhysicsNS::LayerMask mask,
		std::shared_ptr<GameObject> object_to_ignore, std::vector<std::weak_ptr<ColliderComponent>>& collisions);
	int OverlapConvex(FloatConvex& shape, PhysicsNS::LayerMask mask,
		std::shared_ptr<GameObject> object_to_ignore, std::vector<std::weak_ptr<ColliderComponent>>& collisions);

private:

	Physics();
	~Physics() { }
	Physics(const Physics& rhs)
	{

	}
	Physics& operator=(const Physics& rhs) {}

	std::vector<std::vector<std::pair<std::weak_ptr<GameObject>, CollisionState>>> objects;

	bool collision_matrix[(int)(PhysicsNS::Layer::_LAST_DONT_REMOVE)][(int)
		(PhysicsNS::Layer::_LAST_DONT_REMOVE)];

	void init();
	void fixed_update();

	/// <summary>
	/// Initialize matrix double array with bool flags determining if layers acknowledge each other.
	/// </summary>
	void init_matrix();
	/// <summary>
	/// Takes in two pairs of gameobjects and their collision states with each other. 
	/// These objects are compared for intersection and collisions are handled.
	/// </summary>
	void handle_collision(std::pair<std::weak_ptr<GameObject>, CollisionState>& a,
		std::pair<std::weak_ptr<GameObject>, CollisionState>& b);
	void update_collision_state(std::pair<std::weak_ptr<GameObject>, CollisionState>& a,
		std::pair<std::weak_ptr<GameObject>, CollisionState>& b);



	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}
