#pragma once
#include "IData.h"

namespace bm98
{
class GameObject;
class Entity;
class Collision;
class Collider;

class Component : public IData
{
public:
	Component();
	~Component();

	/// <summary>
	/// First function called when a game object is being initialized in a scene.
	///  Will initialize Component with all other components it needs.
	/// </summary>
	virtual void init();
	/// <summary>
	/// Second function called when a game object is being initialized in a scene.
	/// Will wrap up any initialization needed for dedicated components. This function
	/// should be used by scripts to initialize their data.
	/// </summary>
	virtual void awake();
	/// <summary>
	/// Third and final start up function for game objects initialized. This function should
	/// solely be used by scripts, and is used to link with other scripts and dedicated components.
	/// </summary>
	virtual void start();
	virtual void update();
	virtual void late_update();
	virtual void fixed_update();
	virtual void add_to_buffer(sf::View* view = nullptr);

	virtual void on_collision_enter(Collision info);
	virtual void on_collision_stay(Collision info);
	virtual void on_collision_exit(Collision info);
	virtual void on_trigger_enter(Collider info);
	virtual void on_trigger_stay(Collider info);
	virtual void on_trigger_exit(Collider info);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

	void set_game_object(GameObject* go);
	GameObject* get_game_object();
	std::string get_name();


protected:
	bool active;
	std::string name;
	GameObject* game_object;



};

}