#pragma once
namespace bm98
{
class GameObject;
class ColliderComponent;
class RigidbodyComponent;

class Collisions
{
};

class Collision
{
public:
	Collision(std::shared_ptr<GameObject> game_object, std::shared_ptr<ColliderComponent> collider)
	{
		this->game_object = game_object;
		this->collider = collider;
	}

	std::weak_ptr<GameObject> game_object;
	std::weak_ptr<ColliderComponent> collider;
	int contact_count;
};

class Collider
{
public:
	Collider(std::shared_ptr<GameObject> game_object)
	{
		this->game_object = game_object;
	}
	std::weak_ptr<GameObject> game_object;

};
}
/*
/// <summary>
/// Currently not in use, and will need updated in future.
/// May be used later or cut
/// </summary>
class Contact
{
public:
	Contact(sf::Vector2f point, HitboxComponent* collider, RigidbodyComponent* rigidbody,
		HitboxComponent* other_collider)
	{
		this->collider = collider;
		this->point = point;
		this->rigidbody = rigidbody;
		this->other_collider = other_collider;
	}

	sf::Vector2f point;
	HitboxComponent* collider;
	RigidbodyComponent* rigidbody;
	HitboxComponent* other_collider;

};
*/